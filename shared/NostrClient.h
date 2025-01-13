#pragma once

#include <jsi/jsi.h>
#include <ReactCommon/TurboModule.h>
#include <vector>
#include <string>
#include <memory>
#include <libwebsockets.h>

using namespace facebook;

class NostrClient : public react::TurboModule {
public:
    static constexpr auto kModuleName = "NostrClient";
    
    NostrClient(std::shared_ptr<react::CallInvoker> jsInvoker)
        : react::TurboModule(kModuleName, jsInvoker) {
        struct lws_context_creation_info info;
        memset(&info, 0, sizeof info);
        info.port = CONTEXT_PORT_NO_LISTEN;
        info.gid = -1;
        info.uid = -1;
        info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
        context = lws_create_context(&info);
    }
    
    ~NostrClient() {
        for (auto conn : connections) {
            lws_callback_on_writable(conn);
        }
        if (context) {
            lws_context_destroy(context);
        }
    }
    
private:
    struct lws_context* context = nullptr;
    std::vector<struct lws*> connections;
    
    static int callback(struct lws *wsi, enum lws_callback_reasons reason,
                       void *user, void *in, size_t len);
    
public:
    void connectToRelay(const std::string& url);
    void subscribeToEvents(const std::string& filter);
    
    std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& rt) override;
    jsi::Value get(jsi::Runtime& rt, const jsi::PropNameID& name) override;
    void set(jsi::Runtime& rt, const jsi::PropNameID& name, const jsi::Value& value) override;
};