#define MSC_CLASS "transport_jni"

#include <jni.h>
#include "transport_jni.h"
#include "Logger.hpp"
#include "sdk/android/native_api/jni/java_types.h"

namespace mediasoupclient {

SendTransportListenerJni::SendTransportListenerJni(JNIEnv *jni, const JavaRef<jobject> &j_listener_)
        : listenerJni(jni, j_listener_) {
}

std::future<void>
SendTransportListenerJni::OnConnect(Transport *transport, const json &dtlsParameters) {
    return std::future<void>();
}

void
SendTransportListenerJni::OnConnectionStateChange(Transport *transport,
                                                  const std::string &connectionState) {

}

std::future<std::string>
SendTransportListenerJni::OnProduce(SendTransport *transport, const std::string &kind,
                                    json rtpParameters, const json &appData) {
    return std::future<std::string>();
}


RecvTransportListenerJni::RecvTransportListenerJni(JNIEnv *jni,
                                                   const JavaRef<jobject> &j_listener_)
        : listenerJni(jni, j_listener_) {
}

std::future<void>
RecvTransportListenerJni::OnConnect(Transport *transport, const json &dtlsParameters) {
    return std::future<void>();
}

void
RecvTransportListenerJni::OnConnectionStateChange(Transport *transport,
                                                  const std::string &connectionState) {
}

Transport *ExtractNativeTransport(JNIEnv *env,
                                  jlong j_transport) {
    auto *transport = reinterpret_cast<Transport *>(j_transport);
    MSC_ASSERT(transport != nullptr, "native transport pointer null");
    return transport;
}

SendTransport *ExtractNativeSendTransport(JNIEnv *env,
                                          jlong j_transport) {
    auto *transport = reinterpret_cast<SendTransport *>(j_transport);
    MSC_ASSERT(transport != nullptr, "native send transport pointer null");
    return transport;
}

RecvTransport *ExtractNativeRecvTransport(JNIEnv *env,
                                          jlong j_transport) {
    auto *transport = reinterpret_cast<RecvTransport *>(j_transport);
    MSC_ASSERT(transport != nullptr, "native receive transport pointer null");
    return transport;
}


extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Transport_getNativeId(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_transport) {
    MSC_TRACE();

    auto id = ExtractNativeTransport(env, j_transport)->GetId();
    return NativeToJavaString(env, id).Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Transport_getNativeConnectionState(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_transport) {
    MSC_TRACE();

    auto state = ExtractNativeTransport(env, j_transport)->GetStats().dump();
    return NativeToJavaString(env, state).Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Transport_getNativeAppData(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_transport) {
    MSC_TRACE();

    auto appData = ExtractNativeTransport(env, j_transport)->GetAppData().dump();
    return NativeToJavaString(env, appData).Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Transport_getNativeStats(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_transport) {
    MSC_TRACE();

    auto stats = ExtractNativeTransport(env, j_transport)->GetStats().dump();
    return NativeToJavaString(env, stats).Release();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_org_mediasoup_droid_Transport_isNativeClosed(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_transport) {
    MSC_TRACE();

    auto closed = ExtractNativeTransport(env, j_transport)->IsClosed();
    return static_cast<jboolean>(closed);
}

extern "C"
JNIEXPORT void JNICALL
Java_org_mediasoup_droid_Transport_nativeRestartIce(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_transport,
        jstring j_iceParameters) {
    MSC_TRACE();

    try {
        auto iceParameters = JavaToNativeString(env, JavaParamRef<jstring>(j_iceParameters));
        ExtractNativeTransport(env, j_transport)->RestartIce(iceParameters);
    } catch (const std::exception &e) {
        MSC_ERROR("%s", e.what());
        jclass clazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(clazz, e.what());
        env->DeleteLocalRef(clazz);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_org_mediasoup_droid_Transport_nativeUpdateIceServers(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_transport,
        jstring j_iceServers) {
    MSC_TRACE();

    try {
        auto iceServers = JavaToNativeString(env, JavaParamRef<jstring>(j_iceServers));
        ExtractNativeTransport(env, j_transport)->UpdateIceServers(iceServers);
    } catch (const std::exception &e) {
        MSC_ERROR("%s", e.what());
        jclass clazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(clazz, e.what());
        env->DeleteLocalRef(clazz);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_org_mediasoup_droid_Transport_nativeClose(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_transport) {
    MSC_TRACE();

    ExtractNativeTransport(env, j_transport)->Close();
}

extern "C"
JNIEXPORT jobject JNICALL
Java_org_mediasoup_droid_Transport_nativeProduce(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_transport,
        jobject j_listener,
        jlong j_track,
        jobject j_parameters,
        jstring j_codecOptions,
        jstring j_appData) {
    MSC_TRACE();

    try {
        // TODO: parse parameters.
        auto track = reinterpret_cast<webrtc::MediaStreamTrackInterface *>(j_track);
        json codecOptions = JavaToNativeString(env, JavaParamRef<jstring>(j_codecOptions));
        json appData = JavaToNativeString(env, JavaParamRef<jstring>(j_appData));
        auto producer = ExtractNativeSendTransport(env, j_transport)->Produce(
                nullptr,
                track,
                nullptr,
                &codecOptions,
                appData);
        // TODO: generate Java producer.
    } catch (const std::exception &e) {
        MSC_ERROR("%s", e.what());
        jclass clazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(clazz, e.what());
        env->DeleteLocalRef(clazz);
    }
    return nullptr;
}


}
