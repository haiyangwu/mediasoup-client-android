#include <jni.h>
#include "Transport.hpp"
#include "sdk/android/native_api/jni/java_types.h"


namespace mediasoupclient {

class SendTransportListenerJni : public SendTransport::Listener {
public:
    SendTransportListenerJni(JNIEnv *jni, const webrtc::JavaRef<jobject> &j_listener_)
            : listenerJni(jni, j_listener_) {
    }

    std::future<void>
    OnConnect(Transport *transport, const nlohmann::json &dtlsParameters) override {
        return std::future<void>();
    }

    void
    OnConnectionStateChange(Transport *transport, const std::string &connectionState) override {

    }

    std::future<std::string>
    OnProduce(SendTransport *transport, const std::string &kind, nlohmann::json rtpParameters,
              const nlohmann::json &appData) override {
        return std::future<std::string>();
    }

private:
    const const webrtc::ScopedJavaGlobalRef<jobject> listenerJni;
};

class RecvTransportListenerJni : public RecvTransport::Listener {
public:
    RecvTransportListenerJni(JNIEnv *jni, const webrtc::JavaRef<jobject> &j_listener_)
            : listenerJni(jni, j_listener_) {
    }

    std::future<void>
    OnConnect(Transport *transport, const nlohmann::json &dtlsParameters) override {
        return std::future<void>();
    }

    void
    OnConnectionStateChange(Transport *transport, const std::string &connectionState) override {
    }

private:
    const const webrtc::ScopedJavaGlobalRef<jobject> listenerJni;

};

}