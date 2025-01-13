cmake_minimum_required(VERSION 3.13)

# Define the library name here.
project(appmodules)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)

# Configure libwebsockets with minimal features for our use case
set(LWS_WITH_SSL OFF CACHE BOOL "Disable SSL - we'll handle encryption differently")
set(LWS_WITHOUT_TESTAPPS ON CACHE BOOL "Don't build test apps")
set(LWS_WITHOUT_TEST_SERVER ON CACHE BOOL "Don't build test server")
set(LWS_WITHOUT_TEST_CLIENT ON CACHE BOOL "Don't build test client")
set(LWS_WITHOUT_EXTENSIONS ON CACHE BOOL "Don't build extensions")
set(LWS_WITH_MINIMAL_EXAMPLES OFF CACHE BOOL "Don't build examples")

# Include FetchContent before ReactNative-application.cmake
include(FetchContent)

FetchContent_Declare(
    libwebsockets
    GIT_REPOSITORY https://github.com/warmcat/libwebsockets.git
    GIT_TAG v4.3.2
)

FetchContent_Declare(
    simdjson
    GIT_REPOSITORY https://github.com/simdjson/simdjson.git
    GIT_TAG v3.6.3
)

FetchContent_MakeAvailable(libwebsockets simdjson)

# Set up include directories early
set(SHARED_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../shared)
include_directories(
    ${SHARED_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# This file includes all the necessary to let you build your React Native application
include(${REACT_ANDROID_DIR}/cmake-utils/ReactNative-application.cmake)

# Add NostrClient source files
target_sources(${CMAKE_PROJECT_NAME} 
    PRIVATE 
    ${SHARED_DIR}/NostrClient.cpp
)

# Include directories for header files
target_include_directories(${CMAKE_PROJECT_NAME} 
    PUBLIC 
    ${SHARED_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Append our libraries to the existing target_link_libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    websockets
    simdjson
    log
)