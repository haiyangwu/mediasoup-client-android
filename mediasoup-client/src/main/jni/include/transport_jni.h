#ifndef MEDIASOUP_CLIENT_ANDROID_TRANSPORT_JNI_H
#define MEDIASOUP_CLIENT_ANDROID_TRANSPORT_JNI_H

#include <jni.h>
#include "common_jni.h"
#include "Transport.hpp"

namespace mediasoupclient {

class SendTransportListenerJni : public SendTransport::Listener {
public:
    SendTransportListenerJni(JNIEnv *jni, const JavaRef<jobject> &j_listener_);

    std::future<void>
    OnConnect(Transport *transport, const json &dtlsParameters) override;

    void
    OnConnectionStateChange(Transport *transport, const std::string &connectionState) override;

    std::future<std::string>
    OnProduce(SendTransport *transport, const std::string &kind, json rtpParameters,
              const json &appData);

private:
    const ScopedJavaGlobalRef<jobject> listenerJni;
};

class RecvTransportListenerJni : public RecvTransport::Listener {
public:
    RecvTransportListenerJni(JNIEnv *jni, const JavaRef<jobject> &j_listener_);

    std::future<void>
    OnConnect(Transport *transport, const json &dtlsParameters) override;

    void
    OnConnectionStateChange(Transport *transport, const std::string &connectionState) override;

private:
    const ScopedJavaGlobalRef<jobject> listenerJni;
};

}

#endif //MEDIASOUP_CLIENT_ANDROID_TRANSPORT_JNI_H
