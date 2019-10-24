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

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Producer_getNativeId(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_producer) {
    MSC_TRACE();

    auto result = reinterpret_cast<OwnedProducer *>(j_producer)->producer()->GetId();
    return NativeToJavaString(env, result).Release();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_org_mediasoup_droid_Producer_isNativeClosed(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_producer) {
    MSC_TRACE();

    auto result = reinterpret_cast<OwnedProducer *>(j_producer)->producer()->IsClosed();
    return static_cast<jboolean>(result);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Producer_getNativeKind(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_producer) {
    MSC_TRACE();

    auto result = reinterpret_cast<OwnedProducer *>(j_producer)->producer()->GetKind();
    return NativeToJavaString(env, result).Release();
}

extern "C"
JNIEXPORT jlong JNICALL
Java_org_mediasoup_droid_Producer_getNativeTrack(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_producer) {
    MSC_TRACE();

    auto result = reinterpret_cast<OwnedProducer *>(j_producer)->producer()->GetTrack();
    return NativeToJavaPointer(result);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_org_mediasoup_droid_Producer_isNativePaused(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_producer) {
    MSC_TRACE();

    auto result = reinterpret_cast<OwnedProducer *>(j_producer)->producer()->IsPaused();
    return static_cast<jboolean>(result);
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_mediasoup_droid_Producer_getNativeMaxSpatialLayer(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_producer) {
    MSC_TRACE();

    auto result = reinterpret_cast<OwnedProducer *>(j_producer)->producer()->GetMaxSpatialLayer();
    return result;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Producer_getNativeAppData(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_producer) {
    MSC_TRACE();

    auto result = reinterpret_cast<OwnedProducer *>(j_producer)->producer()->GetAppData();
    return NativeToJavaString(env, result.dump()).Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Producer_getNativeRtpParameters(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_producer) {
    MSC_TRACE();

    auto result = reinterpret_cast<OwnedProducer *>(j_producer)->producer()->GetRtpParameters();
    return NativeToJavaString(env, result.dump()).Release();
}


extern "C"
JNIEXPORT void JNICALL
Java_org_mediasoup_droid_Producer_nativeResume(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_producer) {
    MSC_TRACE();

    reinterpret_cast<OwnedProducer *>(j_producer)->producer()->Resume();
}

extern "C"
JNIEXPORT void JNICALL
Java_org_mediasoup_droid_Producer_setNativeMaxSpatialLayer(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_producer,
        jint j_layer) {
    MSC_TRACE();

    reinterpret_cast<OwnedProducer *>(j_producer)->producer()->SetMaxSpatialLayer(
            static_cast<uint8_t>(j_layer));
}

}
