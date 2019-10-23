#define MSC_CLASS "producer_jni"

#include <jni.h>
#include "producer_jni.h"
#include "Logger.hpp"
#include "sdk/android/jni/producer_jni.h"

namespace mediasoupclient {

ProducerListenerJNI::ProducerListenerJNI(JNIEnv *env, const JavaRef<jobject> &j_listener_)
        : j_listener_(env, j_listener_) {
    MSC_TRACE();
}

void ProducerListenerJNI::OnTransportClose(Producer *producer) {
    MSC_TRACE();

    JNIEnv *env = webrtc::AttachCurrentThreadIfNeeded();
    Java_Mediasoup_Producer_Listener_onTransportClose(env, j_listener_);
}

}
