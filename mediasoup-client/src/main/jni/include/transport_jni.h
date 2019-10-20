#ifndef MEDIASOUP_CLIENT_ANDROID_TRANSPORT_JNI_H
#define MEDIASOUP_CLIENT_ANDROID_TRANSPORT_JNI_H

#include <jni.h>
#include "common_jni.h"
#include "Transport.hpp"

namespace mediasoupclient {

class OwnedSendTransport {
public:
    OwnedSendTransport(SendTransport *transport, SendTransport::Listener *listener)
            : transport_(transport), listener_(listener) {
    }

    ~OwnedSendTransport() = default;

    SendTransport *transport() const { return transport_.get(); }

private:
    std::unique_ptr<SendTransport> transport_;
    std::unique_ptr<SendTransport::Listener> listener_;
};

class SendTransportListenerJni : public SendTransport::Listener {

public:
    SendTransportListenerJni(JNIEnv *jni, const JavaRef<jobject> &j_listener_);

    std::future<void> OnConnect(Transport *transport, const json &dtlsParameters) override;

    void OnConnectionStateChange(Transport *transport, const std::string &connectionState) override;

    std::future<std::string> OnProduce(
            SendTransport *transport,
            const std::string &kind,
            json rtpParameters,
            const json &appData) override;

public:
    void setTransport(JNIEnv *jni, const JavaRef<jobject> &j_transport) {
        j_transport_ = ScopedJavaLocalRef<jobject>(jni, j_transport);
    }

private:
    const ScopedJavaLocalRef<jobject> j_listener_;
    ScopedJavaLocalRef<jobject> j_transport_;
};

class OwnedRecvTransport {

public:
    OwnedRecvTransport(RecvTransport *peer_connection, RecvTransport::Listener *listener)
            : transport_(peer_connection), listener_(listener) {
    }

    ~OwnedRecvTransport() = default;

    RecvTransport *transport() const { return transport_.get(); }

private:
    std::unique_ptr<RecvTransport> transport_;
    std::unique_ptr<RecvTransport::Listener> listener_;
};

class RecvTransportListenerJni : public RecvTransport::Listener {

public:
    RecvTransportListenerJni(JNIEnv *jni, const JavaRef<jobject> &j_listener_);

    ~RecvTransportListenerJni() = default;

    std::future<void> OnConnect(Transport *transport, const json &dtlsParameters) override;

    void OnConnectionStateChange(Transport *transport, const std::string &connectionState) override;

public:
    void setTransport(JNIEnv *jni, const JavaRef<jobject> &j_transport) {
        j_transport_ = ScopedJavaLocalRef<jobject>(jni, j_transport);
    }

private:
    const ScopedJavaLocalRef<jobject> j_listener_;
    ScopedJavaLocalRef<jobject> j_transport_;
};

}

#endif //MEDIASOUP_CLIENT_ANDROID_TRANSPORT_JNI_H
