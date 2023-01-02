#define MSC_CLASS "transport_jni"

#include "transport_jni.h"
#include "Logger.hpp"
#include "consumer_jni.h"
#include "data_consumer_jni.h"
#include "data_producer_jni.h"
#include "generated_mediasoupclient_jni/jni/RecvTransport_jni.h"
#include "generated_mediasoupclient_jni/jni/SendTransport_jni.h"
#include "generated_mediasoupclient_jni/jni/Transport_jni.h"
#include "producer_jni.h"
#include <include/java_types.h>
#include <sdk/android/native_api/jni/java_types.h>
#include <sdk/android/src/jni/pc/rtp_parameters.h>

namespace mediasoupclient
{
std::future<void> SendTransportListenerJni::OnConnect(Transport* /*transport*/, const json& dtlsParameters)
{
	JNIEnv* env = webrtc::AttachCurrentThreadIfNeeded();
	Java_Listener_onConnect(
	  env, j_listener_, j_transport_, NativeToJavaString(env, dtlsParameters.dump()));
	std::promise<void> promise;
	promise.set_value();
	return promise.get_future();
}

void SendTransportListenerJni::OnConnectionStateChange(
  Transport* /*transport*/, const std::string& connectionState)
{
	JNIEnv* env = webrtc::AttachCurrentThreadIfNeeded();
	Java_Listener_onConnectionStateChange(
	  env, j_listener_, j_transport_, NativeToJavaString(env, connectionState));
}

std::future<std::string> SendTransportListenerJni::OnProduce(
  SendTransport* /*transport*/, const std::string& kind, json rtpParameters, const json& appData)
{
	JNIEnv* env = webrtc::AttachCurrentThreadIfNeeded();
	auto result = Java_Listener_onProduce(
	  env,
	  j_listener_,
	  j_transport_,
	  NativeToJavaString(env, kind),
	  NativeToJavaString(env, rtpParameters.dump()),
	  NativeToJavaString(env, appData.dump()));
	std::promise<std::string> promise;
	promise.set_value(JavaToNativeString(env, result));
	return promise.get_future();
}

std::future<std::string> SendTransportListenerJni::OnProduceData(
  SendTransport* transport,
  const json& sctpStreamParameters,
  const std::string& label,
  const std::string& protocol,
  const json& appData)
{
	JNIEnv* env = webrtc::AttachCurrentThreadIfNeeded();
	auto result = Java_Listener_onProduceData(
	  env,
	  j_listener_,
	  j_transport_,
	  NativeToJavaString(env, sctpStreamParameters.dump()),
	  NativeToJavaString(env, label),
	  NativeToJavaString(env, protocol),
	  NativeToJavaString(env, appData.dump()));
	std::promise<std::string> promise;
	promise.set_value(JavaToNativeString(env, result));
	return promise.get_future();
}

std::future<void> RecvTransportListenerJni::OnConnect(Transport* /*transport*/, const json& dtlsParameters)
{
	JNIEnv* env = webrtc::AttachCurrentThreadIfNeeded();
	Java_Listener_onConnect(
	  env, j_listener_, j_transport_, NativeToJavaString(env, dtlsParameters.dump()));

	std::promise<void> promise;
	promise.set_value();
	return promise.get_future();
}

void RecvTransportListenerJni::OnConnectionStateChange(
  Transport* /*transport*/, const std::string& connectionState)
{
	JNIEnv* env = webrtc::AttachCurrentThreadIfNeeded();
	Java_Listener_onConnectionStateChange(
	  env, j_listener_, j_transport_, NativeToJavaString(env, connectionState));
}

Transport* ExtractNativeTransport(JNIEnv* env, const JavaRef<jobject>& j_transport)
{
	auto* pc = reinterpret_cast<Transport*>(Java_Transport_getNativeTransport(env, j_transport));
	MSC_ASSERT(pc != nullptr, "native transport pointer null");
	return pc;
}

static ScopedJavaLocalRef<jstring> JNI_Transport_GetId(
  JNIEnv* env, const JavaParamRef<jobject>& j_transport)
{
	MSC_TRACE();

	auto id = ExtractNativeTransport(env, j_transport)->GetId();
	return NativeToJavaString(env, id);
}

static ScopedJavaLocalRef<jstring> JNI_Transport_GetConnectionState(
  JNIEnv* env, const JavaParamRef<jobject>& j_transport)
{
	MSC_TRACE();

	auto state = ExtractNativeTransport(env, j_transport)->GetConnectionState();
	return NativeToJavaString(env, state);
}

static ScopedJavaLocalRef<jstring> JNI_Transport_GetAppData(
  JNIEnv* env, const JavaParamRef<jobject>& j_transport)
{
	MSC_TRACE();

	auto appData = ExtractNativeTransport(env, j_transport)->GetAppData().dump();
	return NativeToJavaString(env, appData);
}

static ScopedJavaLocalRef<jstring> JNI_Transport_GetStats(
  JNIEnv* env, const JavaParamRef<jobject>& j_transport)
{
	MSC_TRACE();

	try
	{
		auto stats = ExtractNativeTransport(env, j_transport)->GetStats().dump();
		return NativeToJavaString(env, stats);
	}
	catch (const std::exception& e)
	{
		MSC_ERROR("%s", e.what());
		THROW_MEDIASOUP_CLIENT_EXCEPTION(env, e);
		return nullptr;
	}
}

static jboolean JNI_Transport_IsClosed(JNIEnv* env, const JavaParamRef<jobject>& j_transport)
{
	MSC_TRACE();

	auto closed = ExtractNativeTransport(env, j_transport)->IsClosed();
	return static_cast<jboolean>(closed);
}

static void JNI_Transport_RestartIce(
  JNIEnv* env, const JavaParamRef<jobject>& j_transport, const JavaParamRef<jstring>& j_iceParameters)
{
	MSC_TRACE();

	try
	{
		auto iceParameters = json::object();
		if (!j_iceParameters.is_null())
		{
			iceParameters = json::parse(JavaToNativeString(env, j_iceParameters));
		}
		ExtractNativeTransport(env, j_transport)->RestartIce(iceParameters);
	}
	catch (const std::exception& e)
	{
		MSC_ERROR("%s", e.what());
		THROW_MEDIASOUP_CLIENT_EXCEPTION(env, e);
	}
}

static void JNI_Transport_UpdateIceServers(
  JNIEnv* env, const JavaParamRef<jobject>& j_transport, const JavaParamRef<jstring>& j_iceServers)
{
	MSC_TRACE();

	try
	{
		auto iceServers = json::array();
		if (!j_iceServers.is_null())
		{
			iceServers = json::parse(JavaToNativeString(env, j_iceServers));
		}
		ExtractNativeTransport(env, j_transport)->UpdateIceServers(iceServers);
	}
	catch (const std::exception& e)
	{
		MSC_ERROR("%s", e.what());
		THROW_MEDIASOUP_CLIENT_EXCEPTION(env, e);
	}
}

static void JNI_Transport_Close(JNIEnv* env, const JavaParamRef<jobject>& j_transport)
{
	MSC_TRACE();

	ExtractNativeTransport(env, j_transport)->Close();
}

static jlong JNI_SendTransport_GetNativeTransport(JNIEnv* env, jlong j_transport)
{
	MSC_TRACE();

	return webrtc::NativeToJavaPointer(reinterpret_cast<OwnedSendTransport*>(j_transport)->transport());
}

static ScopedJavaLocalRef<jobject> JNI_SendTransport_Produce(
  JNIEnv* env,
  jlong j_transport,
  const JavaParamRef<jobject>& j_listener,
  jlong j_track,
  const JavaParamRef<jobjectArray>& j_encodings,
  const JavaParamRef<jstring>& j_codecOptions,
  const JavaParamRef<jstring>& j_codec,
  const JavaParamRef<jstring>& j_appData)
{
	MSC_TRACE();

	try
	{
		auto listener = new ProducerListenerJni(env, j_listener);
		auto track    = reinterpret_cast<webrtc::MediaStreamTrackInterface*>(j_track);
		std::vector<webrtc::RtpEncodingParameters> encodings;
		if (!j_encodings.is_null())
		{
			encodings = webrtc::JavaToNativeVector<webrtc::RtpEncodingParameters>(
			  env,
			  webrtc::JavaParamRef<jobjectArray>(j_encodings.obj()),
			  &webrtc::jni::JavaToNativeRtpEncodingParameters);
		}
		json* codecOptions = nullptr;
		json codecOptionsJson;
		if (!j_codecOptions.is_null())
		{
			codecOptionsJson = json::parse(JavaToNativeString(env, j_codecOptions));
			codecOptions     = &codecOptionsJson;
		}
		json* codec = nullptr;
		json codecJson;
		if (!j_codec.is_null())
		{
			codecJson = json::parse(JavaToNativeString(env, j_codec));
			codec     = &codecJson;
		}

		json appData = json::object();
		if (!j_appData.is_null())
		{
			appData = json::parse(JavaToNativeString(env, j_appData));
		}
		auto transport = (reinterpret_cast<OwnedSendTransport*>(j_transport))->transport();
		auto producer  = transport->Produce(listener, track, &encodings, codecOptions, codec, appData);
		return NativeToJavaProducer(env, producer, listener);
	}
	catch (const std::exception& e)
	{
		MSC_ERROR("%s", e.what());
		THROW_MEDIASOUP_CLIENT_EXCEPTION(env, e);
		return nullptr;
	}
}

ScopedJavaLocalRef<jobject> JNI_SendTransport_ProduceData(
  JNIEnv* env,
  jlong j_transport,
  const JavaParamRef<jobject>& j_listener,
  const JavaParamRef<jstring>& j_label,
  const JavaParamRef<jstring>& j_protocol,
  jboolean j_ordered,
  jint j_maxRetransmits,
  jint j_maxPacketLifeTime,
  const JavaParamRef<jstring>& j_appData)
{
	MSC_TRACE();

	try
	{
		auto listener = new DataProducerListenerJni(env, j_listener);
		std::string label;
		if (!j_label.is_null())
		{
			label = JavaToNativeString(env, j_label);
		}
		std::string protocol;
		if (!j_protocol.is_null())
		{
			protocol = JavaToNativeString(env, j_protocol);
		}
		json appData = json::object();
		if (!j_appData.is_null())
		{
			appData = json::parse(JavaToNativeString(env, j_appData));
		}
		auto transport = (reinterpret_cast<OwnedSendTransport*>(j_transport))->transport();
		auto producer  = transport->ProduceData(
      listener, label, protocol, j_ordered, j_maxRetransmits, j_maxPacketLifeTime, appData);
		return NativeToJavaDataProducer(env, producer, listener);
	}
	catch (const std::exception& e)
	{
		MSC_ERROR("%s", e.what());
		THROW_MEDIASOUP_CLIENT_EXCEPTION(env, e);
		return nullptr;
	}
}

static void JNI_SendTransport_FreeTransport(JNIEnv* env, jlong j_transport)
{
	MSC_TRACE();

	delete reinterpret_cast<OwnedSendTransport*>(j_transport);
}

static jlong JNI_RecvTransport_GetNativeTransport(JNIEnv* env, jlong j_transport)
{
	MSC_TRACE();

	return webrtc::NativeToJavaPointer(reinterpret_cast<OwnedRecvTransport*>(j_transport)->transport());
}

static ScopedJavaLocalRef<jobject> JNI_RecvTransport_Consume(
  JNIEnv* env,
  jlong j_transport,
  const JavaParamRef<jobject>& j_listener,
  const JavaParamRef<jstring>& j_id,
  const JavaParamRef<jstring>& j_producerId,
  const JavaParamRef<jstring>& j_kind,
  const JavaParamRef<jstring>& j_rtpParameters,
  const JavaParamRef<jstring>& j_appData)
{
	MSC_TRACE();

	try
	{
		auto listener      = new ConsumerListenerJni(env, j_listener);
		auto id            = JavaToNativeString(env, j_id);
		auto producerId    = JavaToNativeString(env, j_producerId);
		auto kind          = JavaToNativeString(env, j_kind);
		auto rtpParameters = json::object();
		if (!j_rtpParameters.is_null())
		{
			rtpParameters = json::parse(JavaToNativeString(env, j_rtpParameters));
		}
		auto appData = json::object();
		if (!j_appData.is_null())
		{
			appData = json::parse(JavaToNativeString(env, j_appData));
		}

		auto transport = (reinterpret_cast<OwnedRecvTransport*>(j_transport))->transport();
		auto consumer  = transport->Consume(listener, id, producerId, kind, &rtpParameters, appData);
		return NativeToJavaConsumer(env, consumer, listener);
	}
	catch (const std::exception& e)
	{
		MSC_ERROR("%s", e.what());
		THROW_MEDIASOUP_CLIENT_EXCEPTION(env, e);
		return nullptr;
	}
}

static ScopedJavaLocalRef<jobject> JNI_RecvTransport_ConsumeData(
  JNIEnv* env,
  jlong j_transport,
  const JavaParamRef<jobject>& j_listener,
  const JavaParamRef<jstring>& j_id,
  const JavaParamRef<jstring>& j_producerId,
  jlong j_streamId,
  const JavaParamRef<jstring>& j_label,
  const JavaParamRef<jstring>& j_protocol,
  const JavaParamRef<jstring>& j_appData)
{
	MSC_TRACE();

	try
	{
		auto listener   = new DataConsumerListenerJni(env, j_listener);
		auto id         = JavaToNativeString(env, j_id);
		auto producerId = JavaToNativeString(env, j_producerId);

		std::string label;
		if (!j_label.is_null())
		{
			label = JavaToNativeString(env, j_label);
		}
		std::string protocol;
		if (!j_protocol.is_null())
		{
			protocol = JavaToNativeString(env, j_protocol);
		}
		auto appData = json::object();
		if (!j_appData.is_null())
		{
			appData = json::parse(JavaToNativeString(env, j_appData));
		}

		auto transport = (reinterpret_cast<OwnedRecvTransport*>(j_transport))->transport();
		auto consumer =
		  transport->ConsumeData(listener, id, producerId, j_streamId, label, protocol, appData);
		return NativeToJavaDataConsumer(env, consumer, listener);
	}
	catch (const std::exception& e)
	{
		MSC_ERROR("%s", e.what());
		THROW_MEDIASOUP_CLIENT_EXCEPTION(env, e);
		return nullptr;
	}
}

static void JNI_RecvTransport_FreeTransport(JNIEnv* env, jlong j_transport)
{
	MSC_TRACE();

	delete reinterpret_cast<OwnedRecvTransport*>(j_transport);
}

ScopedJavaLocalRef<jobject> NativeToJavaSendTransport(
  JNIEnv* env, SendTransport* transport, SendTransportListenerJni* listener)
{
	auto ownedSendTransport = new OwnedSendTransport(transport, listener);
	auto j_transport =
	  Java_SendTransport_Constructor(env, webrtc::NativeToJavaPointer(ownedSendTransport));
	listener->SetJTransport(env, j_transport);
	return ScopedJavaLocalRef<jobject>(env, j_transport.Release());
}

ScopedJavaLocalRef<jobject> NativeToJavaRecvTransport(
  JNIEnv* env, RecvTransport* transport, RecvTransportListenerJni* listener)
{
	auto ownedRecvTransport = new OwnedRecvTransport(transport, listener);
	auto j_transport =
	  Java_RecvTransport_Constructor(env, webrtc::NativeToJavaPointer(ownedRecvTransport));
	listener->SetJTransport(env, j_transport);
	return ScopedJavaLocalRef<jobject>(env, j_transport.Release());
}

} // namespace mediasoupclient
