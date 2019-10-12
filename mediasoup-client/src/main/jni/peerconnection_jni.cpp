#define MSC_CLASS "peerConnection_jni"

#include <jni.h>
#include "Logger.hpp"
#include "PeerConnection.hpp"
#include "api/rtc_error.h"
#include "sdk/android/src/jni/jvm.h"
#include "sdk/android/src/jni/jni_helpers.h"
#include "sdk/android/src/jni/pc/peer_connection.h"
#include "sdk/android/src/jni/pc/peer_connection_factory.h"
#include "sdk/android/src/jni/pc/session_description.h"
#include "sdk/android/src/jni/pc/rtp_sender.h"
#include "sdk/android/src/jni/pc/media_stream_track.h"
#include "sdk/android/native_api/jni/scoped_java_ref.h"
#include "sdk/android/native_api/jni/java_types.h"

namespace mediasoupclient {

// Just like OwnedPeerConnection in "sdk/android/src/jni/pc/peer_connection.h"
//
// PeerConnection doesn't take ownership of the observer. In Java API, we don't
// want the client to have to manually dispose the observer. To solve this, this
// wrapper class is used for object ownership.
//
// Also stores reference to the deprecated PeerConnection constraints for now.
class OwnedPeerConnection {
public:
    OwnedPeerConnection(PeerConnection *peer_connection, PeerConnection::PrivateListener *observer)
            : peer_connection_(peer_connection), observer_(observer) {
    }

    ~OwnedPeerConnection() = default;

    PeerConnection *pc() const { return peer_connection_.get(); }

private:
    std::unique_ptr<PeerConnection> peer_connection_;
    std::unique_ptr<PeerConnection::PrivateListener> observer_;
};

class PrivateListenerJNI : public PeerConnection::PrivateListener {

private:
    webrtc::jni::PeerConnectionObserverJni observerJni;

public:
    PrivateListenerJNI(JNIEnv *jni, const webrtc::JavaRef<jobject> &j_observer)
            : observerJni(jni, j_observer) {
    }

    void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState newState) override {
        PrivateListener::OnSignalingChange(newState);
        observerJni.OnSignalingChange(newState);
    }

    void OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override {
        PrivateListener::OnAddStream(stream);
        observerJni.OnAddStream(stream);
    }

    void OnRemoveStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override {
        PrivateListener::OnRemoveStream(stream);
        observerJni.OnRemoveStream(stream);
    }

    void OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> dataChannel) override {
        PrivateListener::OnDataChannel(dataChannel);
        observerJni.OnDataChannel(dataChannel);
    }

    void OnRenegotiationNeeded() override {
        PrivateListener::OnRenegotiationNeeded();
        observerJni.OnRenegotiationNeeded();
    }

    void
    OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState newState) override {
        PrivateListener::OnIceConnectionChange(newState);
        observerJni.OnIceConnectionChange(newState);
    }

    void
    OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState newState) override {
        PrivateListener::OnIceGatheringChange(newState);
        observerJni.OnIceGatheringChange(newState);
    }

    void OnIceCandidate(const webrtc::IceCandidateInterface *candidate) override {
        PrivateListener::OnIceCandidate(candidate);
        observerJni.OnIceCandidate(candidate);
    }

    void OnIceCandidatesRemoved(const std::vector<cricket::Candidate> &candidates) override {
        PrivateListener::OnIceCandidatesRemoved(candidates);
        observerJni.OnIceCandidatesRemoved(candidates);
    }

    void OnIceConnectionReceivingChange(bool receiving) override {
        PrivateListener::OnIceConnectionReceivingChange(receiving);
        observerJni.OnIceConnectionReceivingChange(receiving);
    }

    void OnAddTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
                    const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>> &streams) override {
        PrivateListener::OnAddTrack(receiver, streams);
        observerJni.OnAddTrack(receiver, streams);
    }

    void OnTrack(rtc::scoped_refptr<webrtc::RtpTransceiverInterface> transceiver) override {
        PrivateListener::OnTrack(transceiver);
        observerJni.OnTrack(transceiver);
    }

    void OnRemoveTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver) override {
        PrivateListener::OnRemoveTrack(receiver);
        observerJni.OnRemoveTrack(receiver);
    }

    void OnInterestingUsage(int usagePattern) override {
        PrivateListener::OnInterestingUsage(usagePattern);
        observerJni.OnInterestingUsage(usagePattern);
    }
};

extern "C"
JNIEXPORT jlong JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeNewPeerConnection(
        JNIEnv *env,
        jclass /* j_type */,
        jobject j_listener,
        jobject j_rtc_config,
        jlong j_native_peerConnection_factory) {
    MSC_TRACE();

    auto listener = new PrivateListenerJNI(env, webrtc::JavaParamRef<jobject>(j_listener));
    webrtc::PeerConnectionInterface::RTCConfiguration rtc_config(
            webrtc::PeerConnectionInterface::RTCConfigurationType::kAggressive);
    webrtc::jni::JavaToNativeRTCConfiguration(env, webrtc::JavaParamRef<jobject>(j_rtc_config),
                                              &rtc_config);
    PeerConnection::Options options;
    options.config = rtc_config;
    options.factory = reinterpret_cast<webrtc::PeerConnectionFactoryInterface *>(j_native_peerConnection_factory);

    auto *pc = new PeerConnection(listener, &options);
    return webrtc::jni::jlongFromPointer(new OwnedPeerConnection(pc, listener));
}

PeerConnection *ExtractNativePC(JNIEnv *env,
                                jlong j_peerConnection) {
    auto *pc = reinterpret_cast<OwnedPeerConnection *>(j_peerConnection);
    MSC_ASSERT(pc != nullptr, "native peerConnection pointer null");
    return pc->pc();
}

extern "C"
JNIEXPORT void JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeFreeOwnedPeerConnection(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_peerConnection) {
    MSC_TRACE();

    auto *pc = ExtractNativePC(env, j_peerConnection);
    delete pc;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeSetConfiguration(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_peerConnection,
        jobject j_rtc_config) {
    MSC_TRACE();

    webrtc::PeerConnectionInterface::RTCConfiguration rtc_config(
            webrtc::PeerConnectionInterface::RTCConfigurationType::kAggressive);
    webrtc::jni::JavaToNativeRTCConfiguration(env, webrtc::JavaParamRef<jobject>(j_rtc_config),
                                              &rtc_config);
    bool result = ExtractNativePC(env, j_peerConnection)->SetConfiguration(rtc_config);
    return static_cast<jboolean>(result);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeCreateOffer(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_peerConnection,
        jobject j_constraints) {
    MSC_TRACE();

    std::unique_ptr<webrtc::MediaConstraints> constraints =
            webrtc::jni::JavaToNativeMediaConstraints(env,
                                                      webrtc::JavaParamRef<jobject>(j_constraints));
    webrtc::PeerConnectionInterface::RTCOfferAnswerOptions options;
    webrtc::CopyConstraintsIntoOfferAnswerOptions(constraints.release(), &options);

    auto offer = ExtractNativePC(env, j_peerConnection)->CreateOffer(options);
    auto j_offer = webrtc::NativeToJavaString(env, offer);
    return j_offer.Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeCreateAnswer(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_peerConnection,
        jobject j_constraints) {
    MSC_TRACE();

    std::unique_ptr<webrtc::MediaConstraints> constraints =
            webrtc::jni::JavaToNativeMediaConstraints(env,
                                                      webrtc::JavaParamRef<jobject>(j_constraints));
    webrtc::PeerConnectionInterface::RTCOfferAnswerOptions options;
    webrtc::CopyConstraintsIntoOfferAnswerOptions(constraints.release(), &options);

    try {
        auto answer = ExtractNativePC(env, j_peerConnection)->CreateAnswer(options);
        auto j_answer = webrtc::NativeToJavaString(env, answer);
        return j_answer.Release();
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
        jobject /* j_object */,
        jlong j_peerConnection,
        jstring j_type,
        jstring j_desc) {
    MSC_TRACE();

    std::string std_type = webrtc::JavaToStdString(env, j_type);
    std::string std_description = webrtc::JavaToStdString(env, j_desc);

    try {
        ExtractNativePC(env, j_peerConnection)->SetLocalDescription(
                std_type, std_description);
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
        jobject /* j_object */,
        jlong j_peerConnection,
        jstring j_type,
        jstring j_desc) {
    MSC_TRACE();

    std::string std_type = webrtc::JavaToStdString(env, j_type);
    std::string std_description = webrtc::JavaToStdString(env, j_desc);

    try {
        ExtractNativePC(env, j_peerConnection)->SetRemoteDescription(
                std_type, std_description);
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
        jobject /* j_object */,
        jlong j_peerConnection) {
    MSC_TRACE();

    auto *pc = reinterpret_cast<PeerConnection *>(j_peerConnection);
    MSC_ASSERT(pc != nullptr, "native peerConnection pointer null");

    auto desc = ExtractNativePC(env, j_peerConnection)->GetLocalDescription();
    auto j_desc = webrtc::NativeToJavaString(env, desc);
    return j_desc.Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeGetRemoteDescription(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_peerConnection) {
    MSC_TRACE();

    auto desc = ExtractNativePC(env, j_peerConnection)->GetRemoteDescription();
    auto j_desc = webrtc::NativeToJavaString(env, desc);
    return j_desc.Release();
}

extern "C"
JNIEXPORT jobject JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeGetSenders(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_peerConnection) {
    MSC_TRACE();

    auto senders = ExtractNativePC(env, j_peerConnection)->GetSenders();
    return webrtc::NativeToJavaList(env, senders, &webrtc::jni::NativeToJavaRtpSender).Release();
}

extern "C"
JNIEXPORT jobject JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeGetTransceivers(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_peerConnection) {
    MSC_TRACE();

    auto trans = ExtractNativePC(env, j_peerConnection)->GetTransceivers();
    return webrtc::NativeToJavaList(env, trans, &webrtc::jni::NativeToJavaRtpTransceiver).Release();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeRemoveTrack(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_peerConnection,
        jlong native_sender) {
    MSC_TRACE();

    auto sender = reinterpret_cast<webrtc::RtpSenderInterface *>(native_sender);
    return static_cast<jboolean>(ExtractNativePC(env, j_peerConnection)->RemoveTrack(sender));
}

extern "C"
JNIEXPORT jobject JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeAddTransceiverWithTrack(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_peerConnection,
        jlong native_track) {
    MSC_TRACE();

    webrtc::RTCErrorOr<rtc::scoped_refptr<webrtc::RtpTransceiverInterface>> result =
            ExtractNativePC(env, j_peerConnection)->AddTransceiver(
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
        jobject /* j_object */,
        jlong j_peerConnection,
        jobject j_media_type) {
    MSC_TRACE();
    ExtractNativePC(env, j_peerConnection)->Close();

    webrtc::RTCErrorOr<rtc::scoped_refptr<webrtc::RtpTransceiverInterface>> result =
            ExtractNativePC(env, j_peerConnection)->AddTransceiver(
                    webrtc::jni::JavaToNativeMediaType(env, webrtc::JavaParamRef<jobject>(
                            j_media_type)));
    if (!result.ok()) {
        MSC_ERROR("Failed to add transceiver:", result.error().message());
        return nullptr;
    } else {
        return webrtc::jni::NativeToJavaRtpTransceiver(env, result.MoveValue()).Release();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeClose(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_peerConnection) {
    MSC_TRACE();

    ExtractNativePC(env, j_peerConnection)->Close();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeGetStats(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_peerConnection) {
    MSC_TRACE();

    auto stats = ExtractNativePC(env, j_peerConnection)->GetStats().dump();
    auto j_stats = webrtc::NativeToJavaString(env, stats);
    return j_stats.Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeGetStatsForRtpSender(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_peerConnection,
        jlong j_selector) {
    MSC_TRACE();

    auto selector = reinterpret_cast<webrtc::RtpSenderInterface *>(j_selector);
    auto stats = ExtractNativePC(env, j_peerConnection)->GetStats(
            rtc::scoped_refptr<webrtc::RtpSenderInterface>(selector)).dump();
    auto j_stats = webrtc::NativeToJavaString(env, stats);
    return j_stats.Release();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeGetStatsForRtpReceiver(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_peerConnection,
        jlong j_selector) {
    MSC_TRACE();

    auto selector = reinterpret_cast<webrtc::RtpReceiverInterface *>(j_selector);
    auto stats = ExtractNativePC(env, j_peerConnection)->GetStats(
            rtc::scoped_refptr<webrtc::RtpReceiverInterface>(selector)).dump();
    auto j_stats = webrtc::NativeToJavaString(env, stats);
    return j_stats.Release();
}

}

