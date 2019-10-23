#define MSC_CLASS "peerConnection_jni"

#include <jni.h>
#include "peerconnection_jni.h"
#include "Logger.hpp"
#include "PeerConnection.hpp"
#include "api/rtc_error.h"
#include <sdk/android/src/jni/pc/peer_connection.h>
#include <sdk/android/src/jni/pc/peer_connection_factory.h>
#include <sdk/android/src/jni/pc/session_description.h>
#include <sdk/android/src/jni/pc/rtp_sender.h>
#include <sdk/android/src/jni/pc/media_stream_track.h>
#include "sdk/android/jni/peerconnection_jni.h"

namespace mediasoupclient {

void JavaToNativeOptions(
        JNIEnv *env, const JavaRef<jobject> &j_options, PeerConnection::Options *options) {
    if (j_options.is_null()) {
        return;
    }

    auto j_rtc_config = Java_Mediasoup_PeerConnection_Options_getRTCConfig(env, j_options);
    auto j_factory = Java_Mediasoup_PeerConnection_Options_getNativeFactory(env, j_options);
    if (!j_rtc_config.is_null()) {
        webrtc::PeerConnectionInterface::RTCConfiguration rtc_config(
                webrtc::PeerConnectionInterface::RTCConfigurationType::kAggressive);
        webrtc::jni::JavaToNativeRTCConfiguration(env, j_rtc_config, &rtc_config);
        options->config = rtc_config;
    }
    options->factory = reinterpret_cast<webrtc::PeerConnectionFactoryInterface *>(j_factory);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeNewPeerConnection(
        JNIEnv *env,
        jclass /* j_type */,
        jobject j_listener,
        jobject j_options) {
    MSC_TRACE();

    auto listener = new PrivateListenerJNI(env, JavaParamRef<jobject>(j_listener));

    PeerConnection::Options options;
    JavaToNativeOptions(env, JavaParamRef<jobject>(j_options), &options);

    auto *pc = new PeerConnection(listener, &options);
    return NativeToJavaPointer(new OwnedPeerConnection(pc, listener));
}

PeerConnection *ExtractNativePC(JNIEnv *env, const JavaRef<jobject> &j_pc) {
    auto *pc = reinterpret_cast<OwnedPeerConnection *>(
            Java_Mediasoup_PeerConnection_getNativeOwnedPeerConnection(env, j_pc));
    MSC_ASSERT(pc != nullptr, "native peerConnection pointer null");
    return pc->pc();
}

extern "C"
JNIEXPORT void JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeFreeOwnedPeerConnection(
        JNIEnv */* env */,
        jclass /* j_type */,
        jlong j_p) {
    MSC_TRACE();

    delete reinterpret_cast<OwnedPeerConnection *>(j_p);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeGetNativePeerConnection(
        JNIEnv *env,
        jobject j_pc) {
    MSC_TRACE();

    auto result = ExtractNativePC(env, JavaParamRef<jobject>(j_pc));
    return NativeToJavaPointer(result);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeSetConfiguration(
        JNIEnv *env,
        jobject j_pc,
        jobject j_rtc_config) {
    MSC_TRACE();

    webrtc::PeerConnectionInterface::RTCConfiguration rtc_config(
            webrtc::PeerConnectionInterface::RTCConfigurationType::kAggressive);
    webrtc::jni::JavaToNativeRTCConfiguration(env, JavaParamRef<jobject>(j_rtc_config),
                                              &rtc_config);
    bool result = ExtractNativePC(env, JavaParamRef<jobject>(j_pc))->SetConfiguration(rtc_config);
    return static_cast<jboolean>(result);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeCreateOffer(
        JNIEnv *env,
        jobject j_pc,
        jobject j_constraints) {
    MSC_TRACE();

    std::unique_ptr<webrtc::MediaConstraints> constraints =
            webrtc::jni::JavaToNativeMediaConstraints(env, JavaParamRef<jobject>(j_constraints));
    webrtc::PeerConnectionInterface::RTCOfferAnswerOptions options;
    webrtc::CopyConstraintsIntoOfferAnswerOptions(constraints.release(), &options);

    auto offer = ExtractNativePC(env, JavaParamRef<jobject>(j_pc))->CreateOffer(options);
    return NativeToJavaString(env, offer).Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeCreateAnswer(
        JNIEnv *env,
        jobject j_pc,
        jobject j_constraints) {
    MSC_TRACE();

    std::unique_ptr<webrtc::MediaConstraints> constraints =
            webrtc::jni::JavaToNativeMediaConstraints(env, JavaParamRef<jobject>(j_constraints));
    webrtc::PeerConnectionInterface::RTCOfferAnswerOptions options;
    webrtc::CopyConstraintsIntoOfferAnswerOptions(constraints.release(), &options);

    try {
        auto answer = ExtractNativePC(env, JavaParamRef<jobject>(j_pc))->CreateAnswer(options);
        return NativeToJavaString(env, answer).Release();
    } catch (const std::exception &e) {
        MSC_ERROR("%s", e.what());
        jclass clazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(clazz, e.what());
        env->DeleteLocalRef(clazz);
        return nullptr;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeSetLocalDescription(
        JNIEnv *env,
        jobject j_pc,
        jstring j_type,
        jstring j_desc) {
    MSC_TRACE();

    auto std_type = JavaToNativeString(env, JavaParamRef<jstring>(j_type));
    auto std_description = JavaToNativeString(env, JavaParamRef<jstring>(j_desc));

    try {
        ExtractNativePC(env, JavaParamRef<jobject>(j_pc))
                ->SetLocalDescription(std_type, std_description);
    } catch (const std::exception &e) {
        MSC_ERROR("%s", e.what());
        jclass clazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(clazz, e.what());
        env->DeleteLocalRef(clazz);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeSetRemoteDescription(
        JNIEnv *env,
        jobject j_pc,
        jstring j_type,
        jstring j_desc) {
    MSC_TRACE();

    auto std_type = JavaToNativeString(env, JavaParamRef<jstring>(j_type));
    auto std_description = JavaToNativeString(env, JavaParamRef<jstring>(j_desc));

    try {
        ExtractNativePC(env, JavaParamRef<jobject>(j_pc))
                ->SetRemoteDescription(std_type, std_description);
    } catch (const std::exception &e) {
        MSC_ERROR("%s", e.what());
        jclass clazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(clazz, e.what());
        env->DeleteLocalRef(clazz);
    }
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeGetLocalDescription(
        JNIEnv *env,
        jobject j_pc) {
    MSC_TRACE();

    auto desc = ExtractNativePC(env, JavaParamRef<jobject>(j_pc))->GetLocalDescription();
    return NativeToJavaString(env, desc).Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeGetRemoteDescription(
        JNIEnv *env,
        jobject j_pc) {
    MSC_TRACE();

    auto desc = ExtractNativePC(env, JavaParamRef<jobject>(j_pc))->GetRemoteDescription();
    return NativeToJavaString(env, desc).Release();
}

extern "C"
JNIEXPORT jobject JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeGetSenders(
        JNIEnv *env,
        jobject j_pc) {
    MSC_TRACE();

    auto senders = ExtractNativePC(env, JavaParamRef<jobject>(j_pc))->GetSenders();
    return NativeToJavaList(env, senders, &webrtc::jni::NativeToJavaRtpSender).Release();
}

extern "C"
JNIEXPORT jobject JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeGetTransceivers(
        JNIEnv *env,
        jobject j_pc) {
    MSC_TRACE();

    auto trans = ExtractNativePC(env, JavaParamRef<jobject>(j_pc))->GetTransceivers();
    return NativeToJavaList(env, trans, &webrtc::jni::NativeToJavaRtpTransceiver).Release();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeRemoveTrack(
        JNIEnv *env,
        jobject j_pc,
        jlong native_sender) {
    MSC_TRACE();

    auto sender = reinterpret_cast<webrtc::RtpSenderInterface *>(native_sender);
    auto result = ExtractNativePC(env, JavaParamRef<jobject>(j_pc))->RemoveTrack(sender);
    return static_cast<jboolean>(result);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeAddTransceiverWithTrack(
        JNIEnv *env,
        jobject j_pc,
        jlong native_track) {
    MSC_TRACE();

    webrtc::RTCErrorOr<rtc::scoped_refptr<webrtc::RtpTransceiverInterface>> result =
            ExtractNativePC(env, JavaParamRef<jobject>(j_pc))->AddTransceiver(
                    reinterpret_cast<webrtc::MediaStreamTrackInterface *>(native_track));
    if (!result.ok()) {
        MSC_ERROR("Failed to add transceiver: %s", result.error().message());
        return nullptr;
    } else {
        return webrtc::jni::NativeToJavaRtpTransceiver(env, result.MoveValue()).Release();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeAddTransceiverOfType(
        JNIEnv *env,
        jobject j_pc,
        jobject j_media_type) {
    MSC_TRACE();

    auto media_type = webrtc::jni::JavaToNativeMediaType(env, JavaParamRef<jobject>(j_media_type));
    webrtc::RTCErrorOr<rtc::scoped_refptr<webrtc::RtpTransceiverInterface>> result =
            ExtractNativePC(env, JavaParamRef<jobject>(j_pc))->AddTransceiver(media_type);
    if (!result.ok()) {
        MSC_ERROR("Failed to add transceiver: %s", result.error().message());
        return nullptr;
    } else {
        return webrtc::jni::NativeToJavaRtpTransceiver(env, result.MoveValue()).Release();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeClose(
        JNIEnv *env,
        jobject j_pc) {
    MSC_TRACE();

    ExtractNativePC(env, JavaParamRef<jobject>(j_pc))->Close();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeGetStats(
        JNIEnv *env,
        jobject j_pc) {
    MSC_TRACE();

    auto stats = ExtractNativePC(env, JavaParamRef<jobject>(j_pc))->GetStats().dump();
    return NativeToJavaString(env, stats).Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeGetStatsForRtpSender(
        JNIEnv *env,
        jobject j_pc,
        jlong j_selector) {
    MSC_TRACE();

    auto selector = reinterpret_cast<webrtc::RtpSenderInterface *>(j_selector);
    auto stats = ExtractNativePC(env, JavaParamRef<jobject>(j_pc))->GetStats(
            rtc::scoped_refptr<webrtc::RtpSenderInterface>(selector)).dump();
    return NativeToJavaString(env, stats).Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeGetStatsForRtpReceiver(
        JNIEnv *env,
        jobject j_pc,
        jlong j_selector) {
    MSC_TRACE();

    auto selector = reinterpret_cast<webrtc::RtpReceiverInterface *>(j_selector);
    auto stats = ExtractNativePC(env, JavaParamRef<jobject>(j_pc))->GetStats(
            rtc::scoped_refptr<webrtc::RtpReceiverInterface>(selector)).dump();
    return NativeToJavaString(env, stats).Release();
}

}

