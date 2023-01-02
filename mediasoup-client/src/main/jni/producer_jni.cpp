#define MSC_CLASS "producer_jni"

#include "producer_jni.h"
#include "Logger.hpp"
#include "generated_mediasoupclient_jni/jni/Producer_jni.h"
#include <include/java_types.h>
#include <jni.h>

namespace mediasoupclient
{
ProducerListenerJni::ProducerListenerJni(JNIEnv* env, const JavaRef<jobject>& j_listener_)
  : j_listener_(j_listener_)
{
	MSC_TRACE();
}

void ProducerListenerJni::OnTransportClose(Producer* producer)
{
	MSC_TRACE();

	JNIEnv* env = webrtc::AttachCurrentThreadIfNeeded();
	Java_Listener_onTransportClose(env, j_listener_, j_producer_);
}

static ScopedJavaLocalRef<jstring> JNI_Producer_GetId(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedProducer*>(j_producer)->producer()->GetId();
	return NativeToJavaString(env, result);
}

static jboolean JNI_Producer_IsClosed(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedProducer*>(j_producer)->producer()->IsClosed();
	return static_cast<jboolean>(result);
}

static ScopedJavaLocalRef<jstring> JNI_Producer_GetKind(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedProducer*>(j_producer)->producer()->GetKind();
	return NativeToJavaString(env, result);
}

static jlong JNI_Producer_GetTrack(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedProducer*>(j_producer)->producer()->GetTrack();
	return NativeToJavaPointer(result);
}

static jboolean JNI_Producer_IsPaused(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedProducer*>(j_producer)->producer()->IsPaused();
	return static_cast<jboolean>(result);
}

static jint JNI_Producer_GetMaxSpatialLayer(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedProducer*>(j_producer)->producer()->GetMaxSpatialLayer();
	return result;
}

static ScopedJavaLocalRef<jstring> JNI_Producer_GetAppData(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedProducer*>(j_producer)->producer()->GetAppData();
	return NativeToJavaString(env, result.dump());
}

static ScopedJavaLocalRef<jstring> JNI_Producer_GetRtpParameters(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedProducer*>(j_producer)->producer()->GetRtpParameters();
	return NativeToJavaString(env, result.dump());
}

static ScopedJavaLocalRef<jstring> JNI_Producer_GetStats(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();

	try
	{
		auto result = reinterpret_cast<OwnedProducer*>(j_producer)->producer()->GetStats();
		return NativeToJavaString(env, result.dump());
	}
	catch (const std::exception& e)
	{
		MSC_ERROR("%s", e.what());
		THROW_MEDIASOUP_CLIENT_EXCEPTION(env, e);
		return nullptr;
	}
}

static void JNI_Producer_Resume(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();

	reinterpret_cast<OwnedProducer*>(j_producer)->producer()->Resume();
}

static void JNI_Producer_SetMaxSpatialLayer(JNIEnv* env, jlong j_producer, jint j_layer)
{
	MSC_TRACE();

	try
	{
		reinterpret_cast<OwnedProducer*>(j_producer)
		  ->producer()
		  ->SetMaxSpatialLayer(static_cast<uint8_t>(j_layer));
	}
	catch (const std::exception& e)
	{
		MSC_ERROR("%s", e.what());
		THROW_MEDIASOUP_CLIENT_EXCEPTION(env, e);
	}
}

static void JNI_Producer_Pause(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();

	reinterpret_cast<OwnedProducer*>(j_producer)->producer()->Pause();
}

static void JNI_Producer_ReplaceTrack(JNIEnv* env, jlong j_producer, jlong j_track)
{
	MSC_TRACE();

	try
	{
		auto track = reinterpret_cast<webrtc::MediaStreamTrackInterface*>(j_track);
		reinterpret_cast<OwnedProducer*>(j_producer)->producer()->ReplaceTrack(track);
	}
	catch (const std::exception& e)
	{
		MSC_ERROR("%s", e.what());
		THROW_MEDIASOUP_CLIENT_EXCEPTION(env, e);
	}
}

static void JNI_Producer_Close(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();

	reinterpret_cast<OwnedProducer*>(j_producer)->producer()->Close();
}

ScopedJavaLocalRef<jobject> NativeToJavaProducer(
  JNIEnv* env, Producer* producer, ProducerListenerJni* listener)
{
	auto* owned_producer = new OwnedProducer(producer, listener);
	auto j_producer = Java_Producer_Constructor(env, webrtc::NativeToJavaPointer(owned_producer));
	listener->SetJProducer(env, j_producer);
	return { env, j_producer.Release() };
}

} // namespace mediasoupclient
