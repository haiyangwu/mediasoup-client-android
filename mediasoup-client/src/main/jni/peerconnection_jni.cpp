#define MSC_CLASS "peerConnection_jni"

#include "peerconnection_jni.h"
#include "Logger.hpp"
#include "PeerConnection.hpp"
#include "generated_mediasoupclient_jni/jni/PeerConnection_jni.h"
#include <api/rtc_error.h>
#include <include/java_types.h>
#include <jni.h>
#include <sdk/android/src/jni/pc/media_stream_track.h>
#include <sdk/android/src/jni/pc/peer_connection.h>
#include <sdk/android/src/jni/pc/peer_connection_factory.h>
#include <sdk/android/src/jni/pc/rtp_sender.h>
#include <sdk/android/src/jni/pc/session_description.h>

namespace mediasoupclient
{
static jlong JNI_PeerConnection_NewPeerConnection(
  JNIEnv* env,
  const JavaParamRef<jobject>& nativeListener,
  const JavaParamRef<jobject>& configuration,
  jlong j_peerConnection_factory)
{
	MSC_TRACE();

	auto listener = new PrivateListenerJni(env, webrtc::JavaParamRef<jobject>(nativeListener));
	PeerConnection::Options options;
	JavaToNativeOptions(env, configuration, j_peerConnection_factory, options);

	auto* pc = new PeerConnection(listener, &options);
	return webrtc::NativeToJavaPointer(new OwnedPeerConnection(pc, listener));
}

PeerConnection* ExtractNativePC(JNIEnv* env, const JavaRef<jobject>& j_pc)
{
	auto* pc = reinterpret_cast<OwnedPeerConnection*>(
	  Java_PeerConnection_getNativeOwnedPeerConnection(env, j_pc));
	MSC_ASSERT(pc != nullptr, "native peerConnection pointer null");
	return pc->pc();
}

static void JNI_PeerConnection_FreeOwnedPeerConnection(JNIEnv* env, jlong ownedPeerConnection)
{
	MSC_TRACE();

	delete reinterpret_cast<OwnedPeerConnection*>(ownedPeerConnection);
}

static jlong JNI_PeerConnection_GetNativePeerConnection(JNIEnv* env, const JavaParamRef<jobject>& j_pc)
{
	MSC_TRACE();

	auto result = ExtractNativePC(env, j_pc);
	return webrtc::NativeToJavaPointer(result);
}

static jboolean JNI_PeerConnection_SetConfiguration(
  JNIEnv* env, const JavaParamRef<jobject>& j_pc, const JavaParamRef<jobject>& j_rtc_config)
{
	MSC_TRACE();

	webrtc::PeerConnectionInterface::RTCConfiguration rtc_config(
	  webrtc::PeerConnectionInterface::RTCConfigurationType::kAggressive);
	webrtc::jni::JavaToNativeRTCConfiguration(
	  env, webrtc::JavaParamRef<jobject>(j_rtc_config), &rtc_config);
	bool result = ExtractNativePC(env, j_pc)->SetConfiguration(rtc_config);
	return static_cast<jboolean>(result);
}

static ScopedJavaLocalRef<jstring> JNI_PeerConnection_CreateOffer(
  JNIEnv* env, const JavaParamRef<jobject>& j_pc, const JavaParamRef<jobject>& j_constraints)
{
	MSC_TRACE();

	std::unique_ptr<webrtc::MediaConstraints> constraints =
	  webrtc::jni::JavaToNativeMediaConstraints(env, webrtc::JavaParamRef<jobject>(j_constraints));
	webrtc::PeerConnectionInterface::RTCOfferAnswerOptions options;
	webrtc::CopyConstraintsIntoOfferAnswerOptions(constraints.release(), &options);

	auto offer = ExtractNativePC(env, j_pc)->CreateOffer(options);
	return NativeToJavaString(env, offer);
}

static ScopedJavaLocalRef<jstring> JNI_PeerConnection_CreateAnswer(
  JNIEnv* env, const JavaParamRef<jobject>& j_pc, const JavaParamRef<jobject>& j_constraints)
{
	MSC_TRACE();

	std::unique_ptr<webrtc::MediaConstraints> constraints =
	  webrtc::jni::JavaToNativeMediaConstraints(env, webrtc::JavaParamRef<jobject>(j_constraints));
	webrtc::PeerConnectionInterface::RTCOfferAnswerOptions options;
	webrtc::CopyConstraintsIntoOfferAnswerOptions(constraints.release(), &options);

	try
	{
		auto answer = ExtractNativePC(env, j_pc)->CreateAnswer(options);
		return NativeToJavaString(env, answer);
	}
	catch (const std::exception& e)
	{
		MSC_ERROR("%s", e.what());
		THROW_MEDIASOUP_CLIENT_EXCEPTION(env, e);
		return nullptr;
	}
}

static void JNI_PeerConnection_SetLocalDescription(
  JNIEnv* env,
  const JavaParamRef<jobject>& j_pc,
  const JavaParamRef<jstring>& j_type,
  const JavaParamRef<jstring>& j_desc)
{
	MSC_TRACE();

	auto std_type        = JavaToNativeString(env, j_type);
	auto std_description = JavaToNativeString(env, j_desc);

	try
	{
		ExtractNativePC(env, j_pc)->SetLocalDescription(std_type, std_description);
	}
	catch (const std::exception& e)
	{
		MSC_ERROR("%s", e.what());
		THROW_MEDIASOUP_CLIENT_EXCEPTION(env, e);
	}
}

static void JNI_PeerConnection_SetRemoteDescription(
  JNIEnv* env,
  const JavaParamRef<jobject>& j_pc,
  const JavaParamRef<jstring>& j_type,
  const JavaParamRef<jstring>& j_desc)
{
	MSC_TRACE();

	auto std_type        = JavaToNativeString(env, j_type);
	auto std_description = JavaToNativeString(env, j_desc);

	try
	{
		ExtractNativePC(env, j_pc)->SetRemoteDescription(std_type, std_description);
	}
	catch (const std::exception& e)
	{
		MSC_ERROR("%s", e.what());
		THROW_MEDIASOUP_CLIENT_EXCEPTION(env, e);
	}
}

static ScopedJavaLocalRef<jstring> JNI_PeerConnection_GetLocalDescription(
  JNIEnv* env, const JavaParamRef<jobject>& j_pc)
{
	MSC_TRACE();

	auto desc = ExtractNativePC(env, j_pc)->GetLocalDescription();
	return NativeToJavaString(env, desc);
}

static ScopedJavaLocalRef<jstring> JNI_PeerConnection_GetRemoteDescription(
  JNIEnv* env, const JavaParamRef<jobject>& j_pc)
{
	MSC_TRACE();

	auto desc = ExtractNativePC(env, j_pc)->GetRemoteDescription();
	return NativeToJavaString(env, desc);
}

static ScopedJavaLocalRef<jobject> JNI_PeerConnection_GetSenders(
  JNIEnv* env, const JavaParamRef<jobject>& j_pc)
{
	MSC_TRACE();

	auto senders  = ExtractNativePC(env, j_pc)->GetSenders();
	auto j_result = webrtc::NativeToJavaList(env, senders, &webrtc::jni::NativeToJavaRtpSender);
	return ScopedJavaLocalRef<jobject>(env, j_result.obj());
}

static ScopedJavaLocalRef<jobject> JNI_PeerConnection_GetTransceivers(
  JNIEnv* env, const JavaParamRef<jobject>& j_pc)

{
	MSC_TRACE();

	auto trans    = ExtractNativePC(env, j_pc)->GetTransceivers();
	auto j_result = webrtc::NativeToJavaList(env, trans, &webrtc::jni::NativeToJavaRtpTransceiver);
	return ScopedJavaLocalRef<jobject>(env, j_result.obj());
}

static jboolean JNI_PeerConnection_RemoveTrack(
  JNIEnv* env, const JavaParamRef<jobject>& j_pc, jlong native_sender)
{
	MSC_TRACE();

	auto sender = reinterpret_cast<webrtc::RtpSenderInterface*>(native_sender);
	auto result = ExtractNativePC(env, j_pc)->RemoveTrack(sender);
	return static_cast<jboolean>(result);
}

static ScopedJavaLocalRef<jobject> JNI_PeerConnection_AddTransceiverWithTrack(
  JNIEnv* env, const JavaParamRef<jobject>& j_pc, jlong native_track)
{
	MSC_TRACE();

	webrtc::RTCErrorOr<rtc::scoped_refptr<webrtc::RtpTransceiverInterface>> result =
	  ExtractNativePC(env, j_pc)->AddTransceiver(
	    reinterpret_cast<webrtc::MediaStreamTrackInterface*>(native_track));
	if (!result.ok())
	{
		MSC_ERROR("Failed to add transceiver: %s", result.error().message());
		return nullptr;
	}
	else
	{
		auto j_result = webrtc::jni::NativeToJavaRtpTransceiver(env, result.MoveValue());
		return ScopedJavaLocalRef<jobject>(env, j_result.obj());
	}
}

static ScopedJavaLocalRef<jobject> JNI_PeerConnection_AddTransceiverOfType(
  JNIEnv* env, const JavaParamRef<jobject>& j_pc, const JavaParamRef<jobject>& j_media_type)
{
	MSC_TRACE();

	auto media_type =
	  webrtc::jni::JavaToNativeMediaType(env, webrtc::JavaParamRef<jobject>(j_media_type));
	webrtc::RTCErrorOr<rtc::scoped_refptr<webrtc::RtpTransceiverInterface>> result =
	  ExtractNativePC(env, j_pc)->AddTransceiver(media_type);
	if (!result.ok())
	{
		MSC_ERROR("Failed to add transceiver: %s", result.error().message());
		return nullptr;
	}
	else
	{
		auto j_result = webrtc::jni::NativeToJavaRtpTransceiver(env, result.MoveValue());
		return ScopedJavaLocalRef<jobject>(env, j_result.obj());
	}
}

static void JNI_PeerConnection_Close(JNIEnv* env, const JavaParamRef<jobject>& j_pc)
{
	MSC_TRACE();

	ExtractNativePC(env, j_pc)->Close();
}

static ScopedJavaLocalRef<jstring> JNI_PeerConnection_GetStats(
  JNIEnv* env, const JavaParamRef<jobject>& j_pc)
{
	MSC_TRACE();

	auto stats = ExtractNativePC(env, j_pc)->GetStats().dump();
	return NativeToJavaString(env, stats);
}

static ScopedJavaLocalRef<jstring> JNI_PeerConnection_GetStatsForRtpSender(
  JNIEnv* env, const JavaParamRef<jobject>& j_pc, jlong j_selector)
{
	MSC_TRACE();

	auto selector = reinterpret_cast<webrtc::RtpSenderInterface*>(j_selector);
	auto stats =
	  ExtractNativePC(env, j_pc)->GetStats(rtc::scoped_refptr<webrtc::RtpSenderInterface>(selector)).dump();
	return NativeToJavaString(env, stats);
}

static ScopedJavaLocalRef<jstring> JNI_PeerConnection_GetStatsForRtpReceiver(
  JNIEnv* env, const JavaParamRef<jobject>& j_pc, jlong j_selector)
{
	MSC_TRACE();

	auto selector = reinterpret_cast<webrtc::RtpReceiverInterface*>(j_selector);
	auto stats    = ExtractNativePC(env, j_pc)
	               ->GetStats(rtc::scoped_refptr<webrtc::RtpReceiverInterface>(selector))
	               .dump();
	return NativeToJavaString(env, stats);
}

} // namespace mediasoupclient
