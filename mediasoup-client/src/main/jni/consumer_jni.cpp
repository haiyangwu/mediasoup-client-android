#define MSC_CLASS "consumer_jni"

#include <jni.h>
#include "consumer_jni.h"
#include "Logger.hpp"
#include "sdk/android/jni/consumer_jni.h"

namespace mediasoupclient {

ConsumerListenerJNI::ConsumerListenerJNI(JNIEnv *env, const JavaRef<jobject> &j_listener_)
        : j_listener_(env, j_listener_) {
    MSC_TRACE();
}

void ConsumerListenerJNI::OnTransportClose(Consumer *consumer) {
    MSC_TRACE();

    JNIEnv *env = webrtc::AttachCurrentThreadIfNeeded();
    Java_Mediasoup_Consumer_Listener_onTransportClose(
            env,
            j_listener_,
            JavaParamRef<jobject>(j_consumer_));
}

}
