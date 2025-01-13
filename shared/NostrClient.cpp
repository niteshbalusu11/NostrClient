#include "NostrClient.h"
#include <android/log.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "NostrClient", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "NostrClient", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "NostrClient", __VA_ARGS__)

int NostrClient::callback(struct lws *wsi, enum lws_callback_reasons reason,
                         void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            LOGI("Connection established");
            return 0;

        case LWS_CALLBACK_CLIENT_RECEIVE:
            if (in && len) {
                std::string message(static_cast<char *>(in), len);
                LOGI("Received: %s", message.c_str());
            }
            return 0;

        case LWS_CALLBACK_CLIENT_WRITEABLE: {
            std::string msg = "[\"REQ\", \"sub1\", {\"kinds\":[1],\"limit\":100}]";
            unsigned char buf[LWS_PRE + msg.length()];
            memcpy(&buf[LWS_PRE], msg.c_str(), msg.length());
            
            int sent = lws_write(wsi, &buf[LWS_PRE], msg.length(), LWS_WRITE_TEXT);
            if (sent < 0) {
                LOGE("Error writing to socket");
                return -1;
            }
            LOGI("Successfully sent message");
            return 0;
        }

        case LWS_CALLBACK_CLIENT_CLOSED:
            LOGI("Connection closed");
            return 0;

        default:
            return 0;
    }
}

void NostrClient::connectToRelay(const std::string& url) {
    LOGI("Connecting to relay: %s", url.c_str());
    
    static struct lws_protocols protocols[] = {
        {
            "nostr",
            &NostrClient::callback,
            0,
            4096,
        },
        { nullptr, nullptr, 0, 0 }
    };
    
    struct lws_client_connect_info ccinfo;
    memset(&ccinfo, 0, sizeof ccinfo);
    
    ccinfo.context = context;
    ccinfo.address = url.c_str();
    ccinfo.port = 443;
    ccinfo.path = "/";
    ccinfo.host = url.c_str();
    ccinfo.origin = url.c_str();
    ccinfo.protocol = protocols[0].name;
    
    struct lws* conn = lws_client_connect_via_info(&ccinfo);
    if (conn) {
        LOGI("Successfully created connection");
        connections.push_back(conn);
        lws_callback_on_writable(conn);
    } else {
        LOGE("Failed to create connection");
    }
}

void NostrClient::subscribeToEvents(const std::string& filter) {
    LOGI("Subscribing with filter: %s", filter.c_str());
    for (auto conn : connections) {
        lws_callback_on_writable(conn);
    }
}

std::vector<jsi::PropNameID> NostrClient::getPropertyNames(jsi::Runtime& rt) {
    std::vector<jsi::PropNameID> result;
    result.push_back(jsi::PropNameID::forAscii(rt, "connect"));
    result.push_back(jsi::PropNameID::forAscii(rt, "subscribe"));
    return result;
}

jsi::Value NostrClient::get(jsi::Runtime& rt, const jsi::PropNameID& name) {
    auto nameStr = name.utf8(rt);
    
    if (nameStr == "connect") {
        return jsi::Function::createFromHostFunction(rt, name, 1,
            [this](jsi::Runtime& rt, const jsi::Value& thisValue, const jsi::Value* args, size_t count) {
                if (count < 1) {
                    throw jsi::JSError(rt, "connect requires 1 argument");
                }
                std::string url = args[0].getString(rt).utf8(rt);
                this->connectToRelay(url);
                return jsi::Value::undefined();
            });
    }
    
    if (nameStr == "subscribe") {
        return jsi::Function::createFromHostFunction(rt, name, 1,
            [this](jsi::Runtime& rt, const jsi::Value& thisValue, const jsi::Value* args, size_t count) {
                if (count < 1) {
                    throw jsi::JSError(rt, "subscribe requires 1 argument");
                }
                std::string filter = args[0].getString(rt).utf8(rt);
                this->subscribeToEvents(filter);
                return jsi::Value::undefined();
            });
    }
    
    return jsi::Value::undefined();
}

void NostrClient::set(jsi::Runtime& rt, const jsi::PropNameID& name, const jsi::Value& value) {
    // Read-only object
}