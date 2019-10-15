#define MSC_CLASS "device_jni"

#include <jni.h>
#include "common_jni.h"
#include "transport_jni.h"
#include "Device.hpp"
#include "Logger.hpp"

namespace mediasoupclient {

Device *ExtractNativeDevice(JNIEnv *env, jlong j_device) {
    auto *device = reinterpret_cast<Device *>(j_device);
    MSC_ASSERT(device != nullptr, "native device pointer null");
    return device;
}

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

    auto *device = ExtractNativeDevice(env, j_device);
    delete device;
}

extern "C"
JNIEXPORT void JNICALL
Java_org_mediasoup_droid_Device_nativeLoad(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_device,
        jstring j_routerRtpCapabilities) {
    MSC_TRACE();

    try {
        auto capabilities = JavaToNativeString(env, JavaParamRef<jstring>(j_routerRtpCapabilities));
        ExtractNativeDevice(env, j_device)->Load(json::parse(capabilities));
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
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_device) {
    MSC_TRACE();

    return static_cast<jboolean>(ExtractNativeDevice(env, j_device)->IsLoaded());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Device_nativeGetRtpCapabilities(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_device) {
    MSC_TRACE();

    try {
        std::string rtpCap = ExtractNativeDevice(env, j_device)->GetRtpCapabilities().dump();
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
        jobject /* j_object */,
        jlong j_device,
        jstring j_kind) {
    MSC_TRACE();

    try {
        std::string kind = JavaToNativeString(env, JavaParamRef<jstring>(j_kind));
        return static_cast<jboolean>( ExtractNativeDevice(env, j_device)->CanProduce(kind));
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
        jobject /* j_object */,
        jlong j_device,
        jobject j_listener,
        jstring id,
        jstring iceParameters,
        jstring iceCandidates,
        jstring dtlsParameters) {
    MSC_TRACE();

    try {
        auto listener = new SendTransportListenerJni(env, JavaParamRef<jobject>(j_listener));
        auto device = ExtractNativeDevice(env, j_device);

        auto transport = device->CreateSendTransport(listener
                , JavaToNativeString(env, JavaParamRef<jstring>(id))
                , JavaToNativeString(env, JavaParamRef<jstring>(iceParameters))
                , JavaToNativeString(env, JavaParamRef<jstring>(iceCandidates))
                , JavaToNativeString(env, JavaParamRef<jstring>(dtlsParameters))
        );
        // TODO: new Java Transport Object
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
        jobject /* j_object */,
        jlong j_device,
        jobject j_listener,
        jstring id,
        jstring iceParameters,
        jstring iceCandidates,
        jstring dtlsParameters) {
    MSC_TRACE();

    try {
        auto listener = new RecvTransportListenerJni(env, JavaParamRef<jobject>(j_listener));
        auto device = ExtractNativeDevice(env, j_device);

        auto transport = device->CreateRecvTransport(listener
                , JavaToNativeString(env, JavaParamRef<jstring>(id))
                , JavaToNativeString(env, JavaParamRef<jstring>(iceParameters))
                , JavaToNativeString(env, JavaParamRef<jstring>(iceCandidates))
                , JavaToNativeString(env, JavaParamRef<jstring>(dtlsParameters))
        );
        // TODO: new Java Transport Object
    } catch (const std::exception &e) {
        MSC_ERROR("%s", e.what());
        jclass clazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(clazz, e.what());
        env->DeleteLocalRef(clazz);
    }
    return nullptr;
}

}
