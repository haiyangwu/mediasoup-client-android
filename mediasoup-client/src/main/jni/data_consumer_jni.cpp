#define MSC_CLASS "data_consumer_jni"

#include "data_consumer_jni.h"
#include "Logger.hpp"
#include "generated_mediasoupclient_jni/jni/DataConsumer_jni.h"
#include "include/java_types.h"
#include <jni.h>
#include <sdk/android/native_api/jni/java_types.h>

namespace mediasoupclient
{
DataConsumerListenerJni::DataConsumerListenerJni(JNIEnv* env, const JavaRef<jobject>& j_listener)
  : j_listener_global_(j_listener)
{
	MSC_TRACE();
}

void DataConsumerListenerJni::OnConnecting(DataConsumer* dataConsumer)
{
	MSC_TRACE();

	JNIEnv* env = webrtc::AttachCurrentThreadIfNeeded();
	Java_DataConsumerListener_OnConnecting(env, j_listener_global_, j_data_consumer_);
}

void DataConsumerListenerJni::OnOpen(DataConsumer* dataConsumer)
{
	MSC_TRACE();

	JNIEnv* env = webrtc::AttachCurrentThreadIfNeeded();
	Java_DataConsumerListener_OnOpen(env, j_listener_global_, j_data_consumer_);
}

void DataConsumerListenerJni::OnClosing(DataConsumer* dataConsumer)
{
	MSC_TRACE();

	JNIEnv* env = webrtc::AttachCurrentThreadIfNeeded();
	Java_DataConsumerListener_OnClosing(env, j_listener_global_, j_data_consumer_);
}

void DataConsumerListenerJni::OnClose(DataConsumer* dataConsumer)
{
	MSC_TRACE();

	JNIEnv* env = webrtc::AttachCurrentThreadIfNeeded();
	Java_DataConsumerListener_OnClose(env, j_listener_global_, j_data_consumer_);
}

void DataConsumerListenerJni::OnMessage(DataConsumer* dataConsumer, const webrtc::DataBuffer& buffer)
{
	MSC_TRACE();

	JNIEnv* env                                     = webrtc::AttachCurrentThreadIfNeeded();
	webrtc::ScopedJavaLocalRef<jobject> byte_buffer = webrtc::jni::NewDirectByteBuffer(
	  env, const_cast<char*>(buffer.data.data<char>()), buffer.data.size());
	ScopedJavaLocalRef<jobject> j_buffer = Java_DataConsumer_generateBuffer(
	  env, JavaParamRef<jobject>(env, byte_buffer.obj()), buffer.binary);
	Java_DataConsumerListener_OnMessage(env, j_listener_global_, j_data_consumer_, j_buffer);
}

void DataConsumerListenerJni::OnTransportClose(DataConsumer* dataConsumer)
{
	MSC_TRACE();

	JNIEnv* env = webrtc::AttachCurrentThreadIfNeeded();
	Java_DataConsumerListener_OnTransportClose(env, j_listener_global_, j_data_consumer_);
}

static ScopedJavaLocalRef<jstring> JNI_DataConsumer_GetId(JNIEnv* env, jlong j_consumer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedDataConsumer*>(j_consumer)->consumer()->GetId();
	return NativeToJavaString(env, result);
}

static ScopedJavaLocalRef<jstring> JNI_DataConsumer_GetDataProducerId(JNIEnv* env, jlong j_consumer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedDataConsumer*>(j_consumer)->consumer()->GetDataProducerId();
	return NativeToJavaString(env, result);
}

static ScopedJavaLocalRef<jstring> JNI_DataConsumer_GetLocalId(JNIEnv* env, jlong j_consumer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedDataConsumer*>(j_consumer)->consumer()->GetLocalId();
	return NativeToJavaString(env, result);
}

static ScopedJavaLocalRef<jstring> JNI_DataConsumer_GetSctpStreamParameters(JNIEnv* env, jlong j_consumer)
{
	MSC_TRACE();

	auto result =
	  reinterpret_cast<OwnedDataConsumer*>(j_consumer)->consumer()->GetSctpStreamParameters();
	return NativeToJavaString(env, result.dump());
}

static jint JNI_DataConsumer_GetReadyState(JNIEnv* env, jlong j_consumer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedDataConsumer*>(j_consumer)->consumer()->GetReadyState();
	return result;
}

static ScopedJavaLocalRef<jstring> JNI_DataConsumer_GetLabel(JNIEnv* env, jlong j_consumer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedDataConsumer*>(j_consumer)->consumer()->GetLabel();
	return NativeToJavaString(env, result);
}

static ScopedJavaLocalRef<jstring> JNI_DataConsumer_GetProtocol(JNIEnv* env, jlong j_consumer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedDataConsumer*>(j_consumer)->consumer()->GetProtocol();
	return NativeToJavaString(env, result);
}

static ScopedJavaLocalRef<jstring> JNI_DataConsumer_GetAppData(JNIEnv* env, jlong j_consumer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedDataConsumer*>(j_consumer)->consumer()->GetAppData();

	return NativeToJavaString(env, result.dump());
}

static jboolean JNI_DataConsumer_IsClosed(JNIEnv* env, jlong j_consumer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedDataConsumer*>(j_consumer)->consumer()->IsClosed();
	return static_cast<jboolean>(result);
}

static void JNI_DataConsumer_Close(JNIEnv* env, jlong j_consumer)
{
	MSC_TRACE();

	reinterpret_cast<OwnedDataConsumer*>(j_consumer)->consumer()->Close();
}

ScopedJavaLocalRef<jobject> NativeToJavaDataConsumer(
  JNIEnv* env, DataConsumer* consumer, DataConsumerListenerJni* listener)
{
	auto* ownedDataConsumer = new OwnedDataConsumer(consumer, listener);
	auto j_consumer =
	  Java_DataConsumer_Constructor(env, webrtc::NativeToJavaPointer(ownedDataConsumer));
	listener->SetJConsumer(env, j_consumer);
	return { env, j_consumer.Release() };
}

} // namespace mediasoupclient
