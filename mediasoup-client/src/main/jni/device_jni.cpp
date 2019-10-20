#define MSC_CLASS "device_jni"

#include <jni.h>
#include <sdk/android/native_api/jni/scoped_java_ref.h>
#include <sdk/android/src/jni/jni_generator_helper.h>
#include <sdk/android/src/jni/pc/peer_connection.h>
#include "common_jni.h"
#include "transport_jni.h"
#include "Device.hpp"
#include "Logger.hpp"
#include "PeerConnection.hpp"

extern base::android::ScopedJavaLocalRef<jobject> Java_Mediasoup_SendTransport_Constructor(
        JNIEnv *env, jlong nativeSendTransport);

extern base::android::ScopedJavaLocalRef<jobject> Java_Mediasoup_RecvTransport_Constructor(
        JNIEnv *env, jlong nativeRecvTransport);

namespace mediasoupclient {

extern void JavaToNativeOptions(
        JNIEnv *env, const JavaRef<jobject> &j_options, PeerConnection::Options *options);

extern "C"
JNIEXPORT jlong JNICALL
Java_org_mediasoup_droid_Device_nativeNewDevice(
        JNIEnv */* env */,
        jclass /* j_type */) {
    MSC_TRACE();

    auto *device = new Device();
    return NativeToJavaPointer(device);
}

extern "C"
JNIEXPORT void JNICALL
Java_org_mediasoup_droid_Device_nativeFreeDevice(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_device) {
    MSC_TRACE();

    delete reinterpret_cast<Device *>(j_device);
}

extern "C"
JNIEXPORT void JNICALL
Java_org_mediasoup_droid_Device_nativeLoad(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_device,
        jstring j_routerRtpCapabilities) {
    MSC_TRACE();

    try {
        auto capabilities = JavaToNativeString(env, JavaParamRef<jstring>(j_routerRtpCapabilities));
        reinterpret_cast<Device *>(j_device)->Load(json::parse(capabilities));
    } catch (const std::exception &e) {
        MSC_ERROR("%s", e.what());
        jclass clazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(clazz, e.what());
        env->DeleteLocalRef(clazz);
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_org_mediasoup_droid_Device_nativeIsLoaded(
        JNIEnv */* env */,
        jclass /* j_type */,
        jlong j_device) {
    MSC_TRACE();

    auto result = reinterpret_cast<Device *>(j_device)->IsLoaded();
    return static_cast<jboolean>(result);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Device_nativeGetRtpCapabilities(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_device) {
    MSC_TRACE();

    try {
        auto rtpCap = reinterpret_cast<Device *>(j_device)->GetRtpCapabilities().dump();
        return NativeToJavaString(env, rtpCap).Release();
    } catch (const std::exception &e) {
        MSC_ERROR("%s", e.what());
        jclass clazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(clazz, e.what());
        env->DeleteLocalRef(clazz);
        return nullptr;
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_org_mediasoup_droid_Device_nativeCanProduce(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_device,
        jstring j_kind) {
    MSC_TRACE();

    try {
        std::string kind = JavaToNativeString(env, JavaParamRef<jstring>(j_kind));
        auto result = reinterpret_cast<Device *>(j_device)->CanProduce(kind);
        return static_cast<jboolean>(result);
    } catch (const std::exception &e) {
        MSC_ERROR("%s", e.what());
        jclass clazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(clazz, e.what());
        env->DeleteLocalRef(clazz);
        return JNI_FALSE;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_org_mediasoup_droid_Device_nativeCreateSendTransport(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_device,
        jobject j_listener,
        jstring j_id,
        jstring j_iceParameters,
        jstring j_iceCandidates,
        jstring j_dtlsParameters,
        jobject j_options,
        jstring j_appData) {
    MSC_TRACE();

    try {
        auto listener = new SendTransportListenerJni(env, JavaParamRef<jobject>(j_listener));
        auto iceParameters = JavaToNativeString(env, JavaParamRef<jstring>(j_iceParameters));
        auto iceCandidates = JavaToNativeString(env, JavaParamRef<jstring>(j_iceCandidates));
        auto dtlsParameters = JavaToNativeString(env, JavaParamRef<jstring>(j_dtlsParameters));

        PeerConnection::Options options;
        JavaToNativeOptions(env, JavaParamRef<jobject>(j_options), &options);

        json appData = nlohmann::json::object();
        if (j_appData != nullptr) {
            appData = json::parse(JavaToNativeString(env, JavaParamRef<jstring>(j_appData)));
        }

        auto transport = reinterpret_cast<Device *>(j_device)
                ->CreateSendTransport(
                        listener,
                        JavaToNativeString(env, JavaParamRef<jstring>(j_id)),
                        json::parse(iceParameters),
                        json::parse(iceCandidates),
                        json::parse(dtlsParameters),
                        &options,
                        appData
                );
        auto ownedSendTransport = new OwnedSendTransport(transport, listener);
        auto j_transport = Java_Mediasoup_SendTransport_Constructor(
                env,
                NativeToJavaPointer(ownedSendTransport)
        );
        listener->setTransport(env, JavaParamRef<jobject>(j_transport.obj()));
        return j_transport.Release();
    } catch (const std::exception &e) {
        MSC_ERROR("%s", e.what());
        jclass clazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(clazz, e.what());
        env->DeleteLocalRef(clazz);
    }
    return nullptr;
}

extern "C"
JNIEXPORT jobject JNICALL
Java_org_mediasoup_droid_Device_nativeCreateRecvTransport(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_device,
        jobject j_listener,
        jstring j_id,
        jstring j_iceParameters,
        jstring j_iceCandidates,
        jstring j_dtlsParameters,
        jobject j_options,
        jstring j_appData) {
    MSC_TRACE();

    try {
        auto listener = new RecvTransportListenerJni(env, JavaParamRef<jobject>(j_listener));
        auto iceParameters = JavaToNativeString(env, JavaParamRef<jstring>(j_iceParameters));
        auto iceCandidates = JavaToNativeString(env, JavaParamRef<jstring>(j_iceCandidates));
        auto dtlsParameters = JavaToNativeString(env, JavaParamRef<jstring>(j_dtlsParameters));

        PeerConnection::Options options;
        JavaToNativeOptions(env, JavaParamRef<jobject>(j_options), &options);

        json appData = nlohmann::json::object();
        if (j_appData != nullptr) {
            appData = json::parse(JavaToNativeString(env, JavaParamRef<jstring>(j_appData)));
        }

        auto transport = reinterpret_cast<Device *>(j_device)
                ->CreateRecvTransport(
                        listener,
                        JavaToNativeString(env, JavaParamRef<jstring>(j_id)),
                        json::parse(iceParameters),
                        json::parse(iceCandidates),
                        json::parse(dtlsParameters),
                        &options,
                        appData
                );
        auto ownedRecvTransport = new OwnedRecvTransport(transport, listener);
        auto j_transport = Java_Mediasoup_RecvTransport_Constructor(
                env,
                NativeToJavaPointer(ownedRecvTransport)
        );
        listener->setTransport(env, JavaParamRef<jobject>(j_transport.obj()));
        return j_transport.Release();
    } catch (const std::exception &e) {
        MSC_ERROR("%s", e.what());
        jclass clazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(clazz, e.what());
        env->DeleteLocalRef(clazz);
    }
    return nullptr;
}

}
