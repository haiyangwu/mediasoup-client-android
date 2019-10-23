#ifndef MEDIASOUP_CLIENT_ANDROID_PRODUCER_JNI_H
#define MEDIASOUP_CLIENT_ANDROID_PRODUCER_JNI_H

#include <jni.h>
#include "common_jni.h"
#include "Producer.hpp"

namespace mediasoupclient {

class OwnedProducer {
public:
    OwnedProducer(Producer *peer_connection, Producer::Listener *listener)
            : producer_(peer_connection), listener_(listener) {
    }

    ~OwnedProducer() = default;

    Producer *producer() const { return producer_.get(); }

private:
    std::unique_ptr<Producer> producer_;
    std::unique_ptr<Producer::Listener> listener_;
};

class ProducerListenerJNI : public Producer::Listener {

public:
    ProducerListenerJNI(JNIEnv *env, const JavaRef<jobject> &j_listener_);
    ~ProducerListenerJNI() = default;

    void OnTransportClose(Producer *producer) override;

private:
    const ScopedJavaGlobalRef<jobject> j_listener_;
};

}

#endif //MEDIASOUP_CLIENT_ANDROID_PRODUCER_JNI_H
