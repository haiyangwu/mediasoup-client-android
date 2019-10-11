#define MSC_CLASS "device_jni"

#include <jni.h>
#include "Device.hpp"
#include "Logger.hpp"

#include "sdk/android/src/jni/jni_helpers.h"
#include "sdk/android/native_api/jni/java_types.h"

using json = nlohmann::json;

namespace mediasoupclient {

extern "C"
JNIEXPORT jlong JNICALL
Java_org_mediasoup_droid_Device_nativeNewDevice(
        JNIEnv */* env */,
        jclass /* j_type */) {
    MSC_TRACE();

    auto *device = new Device();
    return webrtc::jni::jlongFromPointer(device);
}

extern "C"
JNIEXPORT void JNICALL
Java_org_mediasoup_droid_Device_nativeFreeDevice(
        JNIEnv * /* env */,
        jclass /* j_type */,
        jlong j_device) {
    MSC_TRACE();

    auto *device = reinterpret_cast<Device *>(j_device);
    MSC_ASSERT(device != nullptr, "native device pointer null");

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

    auto *device = reinterpret_cast<Device *>(j_device);
    MSC_ASSERT(device != nullptr, "native device pointer null");

    try {
        std::string routerRtpCapabilities = webrtc::JavaToStdString(env, j_routerRtpCapabilities);
        device->Load(json::parse(routerRtpCapabilities));
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
        jobject /* j_object */,
        jlong j_device) {
    MSC_TRACE();

    auto *device = reinterpret_cast<Device *>(j_device);
    MSC_ASSERT(device != nullptr, "native device pointer null");

    return static_cast<jboolean>(device->IsLoaded());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Device_nativeGetRtpCapabilities(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_device) {
    MSC_TRACE();

    auto *device = reinterpret_cast<Device *>(j_device);
    MSC_ASSERT(device != nullptr, "native device pointer null");

    try {
        std::string rtpCapabilities = device->GetRtpCapabilities().dump();
        MSC_DEBUG("rtpCapabilities %s", rtpCapabilities.c_str());

        auto j_rtpCapabilities = webrtc::NativeToJavaString(env, rtpCapabilities);
        return j_rtpCapabilities.Release();
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

    auto *device = reinterpret_cast<Device *>(j_device);
    MSC_ASSERT(device != nullptr, "native device pointer null");

    try {
        std::string kind = webrtc::JavaToStdString(env, j_kind);
        return static_cast<jboolean>(device->CanProduce(kind));
    } catch (const std::exception &e) {
        MSC_ERROR("%s", e.what());
        jclass clazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(clazz, e.what());
        env->DeleteLocalRef(clazz);
        return JNI_FALSE;
    }
}

}
