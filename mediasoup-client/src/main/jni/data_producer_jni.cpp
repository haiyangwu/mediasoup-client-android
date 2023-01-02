#define MSC_CLASS "data_producer_jni"

#include "data_producer_jni.h"
#include "Logger.hpp"
#include "generated_mediasoupclient_jni/jni/DataProducer_jni.h"
#include <include/java_types.h>
#include <jni.h>

namespace mediasoupclient
{
DataProducerListenerJni::DataProducerListenerJni(JNIEnv* env, const JavaRef<jobject>& j_listener_)
  : j_listener_(j_listener_)
{
	MSC_TRACE();
}

void DataProducerListenerJni::OnOpen(DataProducer* dataProducer)
{
	MSC_TRACE();

	JNIEnv* env = webrtc::AttachCurrentThreadIfNeeded();
	Java_DataProducerListener_onOpen(env, j_listener_, j_data_producer_);
}

void DataProducerListenerJni::OnClose(DataProducer* dataProducer)
{
	MSC_TRACE();

	JNIEnv* env = webrtc::AttachCurrentThreadIfNeeded();
	Java_DataProducerListener_onClose(env, j_listener_, j_data_producer_);
}

void DataProducerListenerJni::OnBufferedAmountChange(DataProducer* dataProducer, uint64_t sentDataSize)
{
	MSC_TRACE();

	JNIEnv* env = webrtc::AttachCurrentThreadIfNeeded();
	Java_DataProducerListener_onBufferedAmountChange(env, j_listener_, j_data_producer_, sentDataSize);
}

void DataProducerListenerJni::OnTransportClose(DataProducer* dataProducer)
{
	MSC_TRACE();

	JNIEnv* env = webrtc::AttachCurrentThreadIfNeeded();
	Java_DataProducerListener_onTransportClose(env, j_listener_, j_data_producer_);
}

ScopedJavaLocalRef<jstring> JNI_DataProducer_GetId(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedDataProducer*>(j_producer)->dataProducer()->GetId();
	return NativeToJavaString(env, result);
}

ScopedJavaLocalRef<jstring> JNI_DataProducer_GetLocalId(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedDataProducer*>(j_producer)->dataProducer()->GetLocalId();
	return NativeToJavaString(env, result);
}

ScopedJavaLocalRef<jstring> JNI_DataProducer_GetSctpStreamParameters(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();

	auto result =
	  reinterpret_cast<OwnedDataProducer*>(j_producer)->dataProducer()->GetSctpStreamParameters();
	return NativeToJavaString(env, result.dump());
}

jint JNI_DataProducer_GetReadyState(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();
	auto result = reinterpret_cast<OwnedDataProducer*>(j_producer)->dataProducer()->GetReadyState();
	return result;
}

ScopedJavaLocalRef<jstring> JNI_DataProducer_GetLabel(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedDataProducer*>(j_producer)->dataProducer()->GetLabel();
	return NativeToJavaString(env, result);
}

ScopedJavaLocalRef<jstring> JNI_DataProducer_GetProtocol(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedDataProducer*>(j_producer)->dataProducer()->GetProtocol();
	return NativeToJavaString(env, result);
}

jlong JNI_DataProducer_GetBufferedAmount(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedDataProducer*>(j_producer)->dataProducer()->GetBufferedAmount();
	return result;
}

ScopedJavaLocalRef<jstring> JNI_DataProducer_GetAppData(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedDataProducer*>(j_producer)->dataProducer()->GetAppData();
	return NativeToJavaString(env, result.dump());
}

jboolean JNI_DataProducer_IsClosed(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();

	auto result = reinterpret_cast<OwnedDataProducer*>(j_producer)->dataProducer()->IsClosed();
	return result;
}

void JNI_DataProducer_Close(JNIEnv* env, jlong j_producer)
{
	MSC_TRACE();

	reinterpret_cast<OwnedDataProducer*>(j_producer)->dataProducer()->Close();
}

void JNI_DataProducer_Send(
  JNIEnv* env, jlong j_producer, const JavaParamRef<jbyteArray>& data, jboolean binary)
{
	MSC_TRACE();

	std::vector<int8_t> buffer =
	  webrtc::JavaToNativeByteArray(env, webrtc::JavaParamRef<jbyteArray>(env, data.obj()));
	reinterpret_cast<OwnedDataProducer*>(j_producer)
	  ->dataProducer()
	  ->Send(webrtc::DataBuffer(rtc::CopyOnWriteBuffer(buffer.data(), buffer.size()), binary));
}

ScopedJavaLocalRef<jobject> NativeToJavaDataProducer(
  JNIEnv* env, DataProducer* producer, DataProducerListenerJni* listener)
{
	auto* owned_producer = new OwnedDataProducer(producer, listener);
	auto j_producer = Java_DataProducer_Constructor(env, webrtc::NativeToJavaPointer(owned_producer));
	listener->SetJProducer(env, j_producer);
	return { env, j_producer.Release() };
}
} // namespace mediasoupclient
