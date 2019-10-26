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

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Consumer_getNativeId(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_consumer) {
    MSC_TRACE();

    auto result = reinterpret_cast<OwnedConsumer *>(j_consumer)->consumer()->GetId();
    return NativeToJavaString(env, result).Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Consumer_getNativeProducerId(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_consumer) {
    MSC_TRACE();

    auto result = reinterpret_cast<OwnedConsumer *>(j_consumer)->consumer()->GetProducerId();
    return NativeToJavaString(env, result).Release();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_org_mediasoup_droid_Consumer_isNativeClosed(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_consumer) {
    MSC_TRACE();

    auto result = reinterpret_cast<OwnedConsumer *>(j_consumer)->consumer()->IsClosed();
    return static_cast<jboolean>(result);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_org_mediasoup_droid_Consumer_isNativePaused(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_consumer) {
    MSC_TRACE();

    auto result = reinterpret_cast<OwnedConsumer *>(j_consumer)->consumer()->IsPaused();
    return static_cast<jboolean>(result);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Consumer_getNativeKind(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_consumer) {
    MSC_TRACE();

    auto result = reinterpret_cast<OwnedConsumer *>(j_consumer)->consumer()->GetKind();
    return NativeToJavaString(env, result).Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Consumer_getNativeRtpParameters(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_consumer) {
    MSC_TRACE();

    auto result = reinterpret_cast<OwnedConsumer *>(j_consumer)->consumer()->GetRtpParameters();

    return NativeToJavaString(env, result.dump()).Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Consumer_getNativeAppData(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_consumer) {
    MSC_TRACE();

    auto result = reinterpret_cast<OwnedConsumer *>(j_consumer)->consumer()->GetAppData();

    return NativeToJavaString(env, result.dump()).Release();
}

}
