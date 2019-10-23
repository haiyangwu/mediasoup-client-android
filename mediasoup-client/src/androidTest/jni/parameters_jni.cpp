#include <jni.h>
#include "json.hpp"
#include <sdk/android/native_api/jni/java_types.h>
#include "../../../deps/libmediasoupclient/test/data/parameters.hpp"

using json = nlohmann::json;
using namespace webrtc;

namespace mediasoupclient {
namespace test {

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_data_Parameters_generateRouterRtpCapabilities(
        JNIEnv *env,
        jclass /* j_type */) {
    json routerRtpCapabilities = generateRouterRtpCapabilities();
    ScopedJavaLocalRef<jstring> result = NativeToJavaString(env, routerRtpCapabilities.dump());
    return result.Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_data_Parameters_generateRouterRtpCapabilitiesExclude(
        JNIEnv *env,
        jclass /* j_type */,
        jstring j_exclude) {
    std::string exclude = webrtc::JavaToNativeString(env, webrtc::JavaParamRef<jstring>(j_exclude));
    json routerRtpCapabilities = generateRouterRtpCapabilities();
    for (auto& codec : routerRtpCapabilities["codecs"])
        codec.erase(exclude);

    ScopedJavaLocalRef<jstring> result = NativeToJavaString(env, routerRtpCapabilities.dump());
    return result.Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_data_Parameters_generateRtpParametersByKind(
        JNIEnv *env,
        jclass /* j_type */) {
    json rtpParametersByKind = generateRtpParametersByKind();
    ScopedJavaLocalRef<jstring> result = NativeToJavaString(env, rtpParametersByKind.dump());
    return result.Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_data_Parameters_generateLocalDtlsParameters(
        JNIEnv *env,
        jclass /* j_type */) {
    json localDtlsParameters = generateLocalDtlsParameters();
    ScopedJavaLocalRef<jstring> result = NativeToJavaString(env, localDtlsParameters.dump());
    return result.Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_data_Parameters_generateTransportRemoteParameters(
        JNIEnv *env,
        jclass /* j_type */) {
    json transportRemoteParameters = generateTransportRemoteParameters();
    ScopedJavaLocalRef<jstring> result = NativeToJavaString(env, transportRemoteParameters.dump());
    return result.Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_data_Parameters_generateProducerRemoteId(
        JNIEnv *env,
        jclass /* j_type */) {
    std::string producerRemoteId = generateProducerRemoteId();
    ScopedJavaLocalRef<jstring> result = NativeToJavaString(env, producerRemoteId);
    return result.Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_data_Parameters_generateConsumerRemoteParameters(
        JNIEnv *env,
        jclass /* j_type */,
        jstring j_codecMimeType) {
    std::string codecMimeType = JavaToStdString(env, j_codecMimeType);
    json consumerRemoteParameters = generateConsumerRemoteParameters(codecMimeType);
    ScopedJavaLocalRef<jstring> result = NativeToJavaString(env, consumerRemoteParameters.dump());
    return result.Release();
}

}
}
