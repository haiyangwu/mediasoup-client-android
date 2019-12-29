#define MSC_CLASS "consumer_jni"

#include "consumer_jni.h"
#include "Logger.hpp"
#include "sdk/android/jni/consumer_jni.h"
#include <jni.h>

namespace mediasoupclient
{
ConsumerListenerJni::ConsumerListenerJni(JNIEnv* env, const JavaRef<jobject>& j_listener)
  : j_listener_global_(env, j_listener)
{
	MSC_TRACE();
}

void ConsumerListenerJni::OnTransportClose(Consumer* /* native_consumer */)
{
	MSC_TRACE();

	JNIEnv* env = webrtc::AttachCurrentThreadIfNeeded();
	Java_Mediasoup_Consumer_Listener_onTransportClose(
	  env, j_listener_global_, JavaParamRef<jobject>(j_consumer_));
}

extern "C" JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Consumer_getNativeId(JNIEnv* env, jclass /* j_type */, jlong j_consumer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedConsumer*>(j_consumer)->consumer()->GetId();
	return NativeToJavaString(env, result).Release();
}

extern "C" JNIEXPORT jstring JNICALL Java_org_mediasoup_droid_Consumer_getNativeProducerId(
  JNIEnv* env, jclass /* j_type */, jlong j_consumer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedConsumer*>(j_consumer)->consumer()->GetProducerId();
	return NativeToJavaString(env, result).Release();
}

extern "C" JNIEXPORT jboolean JNICALL
Java_org_mediasoup_droid_Consumer_isNativeClosed(JNIEnv* env, jclass /* j_type */, jlong j_consumer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedConsumer*>(j_consumer)->consumer()->IsClosed();
	return static_cast<jboolean>(result);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_org_mediasoup_droid_Consumer_isNativePaused(JNIEnv* env, jclass /* j_type */, jlong j_consumer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedConsumer*>(j_consumer)->consumer()->IsPaused();
	return static_cast<jboolean>(result);
}

extern "C" JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Consumer_getNativeKind(JNIEnv* env, jclass /* j_type */, jlong j_consumer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedConsumer*>(j_consumer)->consumer()->GetKind();
	return NativeToJavaString(env, result).Release();
}

extern "C" JNIEXPORT jlong JNICALL
Java_org_mediasoup_droid_Consumer_getNativeTrack(JNIEnv* env, jclass /* j_type */, jlong j_consumer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedConsumer*>(j_consumer)->consumer()->GetTrack();
	return NativeToJavaPointer(result);
}

extern "C" JNIEXPORT jstring JNICALL Java_org_mediasoup_droid_Consumer_getNativeRtpParameters(
  JNIEnv* env, jclass /* j_type */, jlong j_consumer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedConsumer*>(j_consumer)->consumer()->GetRtpParameters();

	return NativeToJavaString(env, result.dump()).Release();
}

extern "C" JNIEXPORT jstring JNICALL Java_org_mediasoup_droid_Consumer_getNativeAppData(
  JNIEnv* env, jclass /* j_type */, jlong j_consumer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedConsumer*>(j_consumer)->consumer()->GetAppData();

	return NativeToJavaString(env, result.dump()).Release();
}

extern "C" JNIEXPORT void JNICALL
Java_org_mediasoup_droid_Consumer_nativeResume(JNIEnv* env, jclass /* j_type */, jlong j_consumer)
{
	MSC_TRACE();

	reinterpret_cast<OwnedConsumer*>(j_consumer)->consumer()->Resume();
}

extern "C" JNIEXPORT void JNICALL
Java_org_mediasoup_droid_Consumer_nativePause(JNIEnv* env, jclass /* j_type */, jlong j_consumer)
{
	MSC_TRACE();

	reinterpret_cast<OwnedConsumer*>(j_consumer)->consumer()->Pause();
}

extern "C" JNIEXPORT jstring JNICALL
Java_org_mediasoup_droid_Consumer_getNativeStats(JNIEnv* env, jclass /* j_type */, jlong j_consumer)
{
	MSC_TRACE();

	try
	{
		auto result = reinterpret_cast<OwnedConsumer*>(j_consumer)->consumer()->GetStats();
		return NativeToJavaString(env, result.dump()).Release();
	}
	catch (const std::exception& e)
	{
		MSC_ERROR("%s", e.what());
		jclass clazz = env->FindClass("java/lang/RuntimeException");
		env->ThrowNew(clazz, e.what());
		env->DeleteLocalRef(clazz);
		return nullptr;
	}
}

extern "C" JNIEXPORT void JNICALL
Java_org_mediasoup_droid_Consumer_nativeClose(JNIEnv* env, jclass /* j_type */, jlong j_consumer)
{
	MSC_TRACE();

	reinterpret_cast<OwnedConsumer*>(j_consumer)->consumer()->Close();
}

} // namespace mediasoupclient
