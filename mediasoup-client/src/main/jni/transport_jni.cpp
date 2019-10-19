#define MSC_CLASS "transport_jni"

#include <jni.h>
#include "transport_jni.h"
#include "Logger.hpp"
#include "sdk/android/native_api/jni/java_types.h"
#include "sdk/android/jni/transport_jni.h"
#include "sdk/android/jni/send_transport_jni.h"
#include "sdk/android/jni/rec_transport_jni.h"

namespace mediasoupclient {

SendTransportListenerJni::SendTransportListenerJni(JNIEnv *jni, const JavaRef<jobject> &j_listener_)
        : j_listener_(jni, j_listener_) {
}

std::future<void>
SendTransportListenerJni::OnConnect(Transport */*transport*/, const json &dtlsParameters) {
    JNIEnv *env = webrtc::AttachCurrentThreadIfNeeded();
    Java_Mediasoup_Listener_OnConnect(env, j_listener_, j_transport_,
                                      NativeToJavaString(env, dtlsParameters.dump()));
    std::promise<void> promise;
    promise.set_value();
    return promise.get_future();
}

void
SendTransportListenerJni::OnConnectionStateChange(
        Transport */*transport*/,
        const std::string &connectionState) {
    JNIEnv *env = webrtc::AttachCurrentThreadIfNeeded();
    Java_Mediasoup_Listener_OnConnectionStateChange(env, j_listener_, j_transport_,
                                                    NativeToJavaString(env, connectionState));
}

std::future<std::string>
SendTransportListenerJni::OnProduce(
        SendTransport */*transport*/,
        const std::string &kind,
        json rtpParameters,
        const json &appData) {
    JNIEnv *env = webrtc::AttachCurrentThreadIfNeeded();
    auto result = Java_Mediasoup_Listener_OnProduce(env, j_listener_, j_transport_,
                                                    NativeToJavaString(env, kind),
                                                    NativeToJavaString(env, rtpParameters.dump()),
                                                    NativeToJavaString(env, appData.dump()));
    std::promise<std::string> promise;
    // promise.set_value();
    return promise.get_future();
}


RecvTransportListenerJni::RecvTransportListenerJni(JNIEnv *jni,
                                                   const JavaRef<jobject> &j_listener_)
        : j_listener_(jni, j_listener_) {
}

std::future<void>
RecvTransportListenerJni::OnConnect(Transport */*transport*/, const json &dtlsParameters) {
    JNIEnv *env = webrtc::AttachCurrentThreadIfNeeded();
    Java_Mediasoup_Listener_OnConnect(env, j_listener_, j_transport_,
                                      NativeToJavaString(env, dtlsParameters.dump()));

    std::promise<void> promise;
    promise.set_value();
    return promise.get_future();
}

void
RecvTransportListenerJni::OnConnectionStateChange(
        Transport */*transport*/,
        const std::string &connectionState) {
    JNIEnv *env = webrtc::AttachCurrentThreadIfNeeded();
    Java_Mediasoup_Listener_OnConnectionStateChange(env, j_listener_, j_transport_,
                                                    NativeToJavaString(env, connectionState));
}

Transport *ExtractNativeTransport(JNIEnv *env, const JavaRef<jobject> &j_transport) {
    auto *pc = reinterpret_cast<Transport *>(
            Java_Mediasoup_Transport_getNativeTransport(env, j_transport));
    MSC_ASSERT(pc != nullptr, "native transport pointer null");
    return pc;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Transport_getNativeId(
        JNIEnv *env,
        jobject j_transport) {
    MSC_TRACE();

    auto id = ExtractNativeTransport(env, JavaParamRef<jobject>(j_transport))->GetId();
    return NativeToJavaString(env, id).Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Transport_getNativeConnectionState(
        JNIEnv *env,
        jobject j_transport) {
    MSC_TRACE();

    auto state = ExtractNativeTransport(env,
                                        JavaParamRef<jobject>(j_transport))->GetConnectionState();
    return NativeToJavaString(env, state).Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Transport_getNativeAppData(
        JNIEnv *env,
        jobject j_transport) {
    MSC_TRACE();

    auto appData = ExtractNativeTransport(env,
                                          JavaParamRef<jobject>(j_transport))->GetAppData().dump();
    return NativeToJavaString(env, appData).Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Transport_getNativeStats(
        JNIEnv *env,
        jobject j_transport) {
    MSC_TRACE();

    auto stats = ExtractNativeTransport(env, JavaParamRef<jobject>(j_transport))->GetStats().dump();
    return NativeToJavaString(env, stats).Release();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_org_mediasoup_droid_Transport_isNativeClosed(
        JNIEnv *env,
        jobject j_transport) {
    MSC_TRACE();

    auto closed = ExtractNativeTransport(env, JavaParamRef<jobject>(j_transport))->IsClosed();
    return static_cast<jboolean>(closed);
}

extern "C"
JNIEXPORT void JNICALL
Java_org_mediasoup_droid_Transport_nativeRestartIce(
        JNIEnv *env,
        jobject j_transport,
        jstring j_iceParameters) {
    MSC_TRACE();

    try {
        auto iceParameters = JavaToNativeString(env, JavaParamRef<jstring>(j_iceParameters));
        ExtractNativeTransport(env, JavaParamRef<jobject>(j_transport))
                ->RestartIce(iceParameters);
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
        jobject j_transport,
        jstring j_iceServers) {
    MSC_TRACE();

    try {
        auto iceServers = JavaToNativeString(env, JavaParamRef<jstring>(j_iceServers));
        ExtractNativeTransport(env, JavaParamRef<jobject>(j_transport))
                ->UpdateIceServers(iceServers);
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
        jobject j_transport) {
    MSC_TRACE();

    ExtractNativeTransport(env, JavaParamRef<jobject>(j_transport))->Close();
}

extern "C"
JNIEXPORT jlong JNICALL
Java_org_mediasoup_droid_SendTransport_nativeGetNativeTransport(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_transport) {
    MSC_TRACE();

    return NativeToJavaPointer(reinterpret_cast<OwnedSendTransport *>(j_transport)->transport());
}

extern "C"
JNIEXPORT jobject JNICALL
Java_org_mediasoup_droid_SendTransport_nativeProduce(
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
        auto transport = (reinterpret_cast<OwnedSendTransport *>(j_transport))->transport();
        auto producer = transport->Produce(
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

extern "C"
JNIEXPORT void JNICALL
Java_org_mediasoup_droid_SendTransport_nativeFreeTransport(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_transport) {
    MSC_TRACE();

    delete reinterpret_cast<OwnedSendTransport *>(j_transport);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_org_mediasoup_droid_RecvTransport_nativeGetNativeTransport(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_transport) {
    MSC_TRACE();

    return NativeToJavaPointer(reinterpret_cast<OwnedRecvTransport *>(j_transport)->transport());
}

extern "C"
JNIEXPORT void JNICALL
Java_org_mediasoup_droid_RecvTransport_nativeFreeTransport(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_transport) {
    MSC_TRACE();

    delete reinterpret_cast<OwnedRecvTransport *>(j_transport);
}

}
