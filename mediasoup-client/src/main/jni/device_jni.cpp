#define MSC_CLASS "device_jni"

#include "generated_mediasoupclient_jni/jni/Device_jni.h"
#include "Device.hpp"
#include "Logger.hpp"
#include "PeerConnection.hpp"
#include "common_jni.h"
#include "transport_jni.h"
#include <include/java_types.h>
#include <jni.h>
#include <sdk/android/native_api/jni/scoped_java_ref.h>
#include <sdk/android/src/jni/pc/peer_connection.h>

namespace mediasoupclient
{
static jlong JNI_Device_NewDevice(JNIEnv* env)
{
	MSC_TRACE();

	auto* device = new Device();
	return webrtc::NativeToJavaPointer(device);
}

static void JNI_Device_FreeDevice(JNIEnv* env, jlong j_device)
{
	MSC_TRACE();

	delete reinterpret_cast<Device*>(j_device);
}

static void JNI_Device_Load(
  JNIEnv* env, jlong j_device, const JavaParamRef<jstring>& j_routerRtpCapabilities)
{
	MSC_TRACE();

	try
	{
		auto capabilities = JavaToNativeString(env, j_routerRtpCapabilities);
		reinterpret_cast<Device*>(j_device)->Load(json::parse(capabilities));
	}
	catch (const std::exception& e)
	{
		MSC_ERROR("%s", e.what());
		THROW_MEDIASOUP_CLIENT_EXCEPTION(env, e);
	}
}

static jboolean JNI_Device_IsLoaded(JNIEnv* env, jlong j_device)
{
	MSC_TRACE();

	auto result = reinterpret_cast<Device*>(j_device)->IsLoaded();
	return static_cast<jboolean>(result);
}

static ScopedJavaLocalRef<jstring> JNI_Device_GetRtpCapabilities(JNIEnv* env, jlong j_device)
{
	MSC_TRACE();

	try
	{
		auto rtpCap = reinterpret_cast<Device*>(j_device)->GetRtpCapabilities().dump();
		return NativeToJavaString(env, rtpCap);
	}
	catch (const std::exception& e)
	{
		MSC_ERROR("%s", e.what());
		THROW_MEDIASOUP_CLIENT_EXCEPTION(env, e);
		return nullptr;
	}
}

static jboolean JNI_Device_CanProduce(JNIEnv* env, jlong j_device, const JavaParamRef<jstring>& j_kind)
{
	MSC_TRACE();

	try
	{
		std::string kind = JavaToNativeString(env, j_kind);
		auto result      = reinterpret_cast<Device*>(j_device)->CanProduce(kind);
		return static_cast<jboolean>(result);
	}
	catch (const std::exception& e)
	{
		MSC_ERROR("%s", e.what());
		THROW_MEDIASOUP_CLIENT_EXCEPTION(env, e);
		return JNI_FALSE;
	}
}

static ScopedJavaLocalRef<jobject> JNI_Device_CreateSendTransport(
  JNIEnv* env,
  jlong j_device,
  const JavaParamRef<jobject>& j_listener,
  const JavaParamRef<jstring>& j_id,
  const JavaParamRef<jstring>& j_iceParameters,
  const JavaParamRef<jstring>& j_iceCandidates,
  const JavaParamRef<jstring>& j_dtlsParameters,
  const JavaParamRef<jobject>& j_config,
  jlong j_peerConnection_factory,
  const JavaParamRef<jstring>& j_appData)
{
	MSC_TRACE();

	try
	{
		auto listener       = new SendTransportListenerJni(env, j_listener);
		auto iceParameters  = JavaToNativeString(env, j_iceParameters);
		auto iceCandidates  = JavaToNativeString(env, j_iceCandidates);
		auto dtlsParameters = JavaToNativeString(env, j_dtlsParameters);

		PeerConnection::Options options;
		JavaToNativeOptions(env, j_config, j_peerConnection_factory, options);

		json appData = nlohmann::json::object();
		if (!j_appData.is_null())
		{
			appData = json::parse(JavaToNativeString(env, j_appData));
		}

		auto transport = reinterpret_cast<Device*>(j_device)->CreateSendTransport(
		  listener,
		  JavaToNativeString(env, j_id),
		  json::parse(iceParameters),
		  json::parse(iceCandidates),
		  json::parse(dtlsParameters),
		  &options,
		  appData);
		return NativeToJavaSendTransport(env, transport, listener);
	}
	catch (const std::exception& e)
	{
		MSC_ERROR("%s", e.what());
		THROW_MEDIASOUP_CLIENT_EXCEPTION(env, e);
	}
	return nullptr;
}

static ScopedJavaLocalRef<jobject> JNI_Device_CreateRecvTransport(
  JNIEnv* env,
  jlong j_device,
  const JavaParamRef<jobject>& j_listener,
  const JavaParamRef<jstring>& j_id,
  const JavaParamRef<jstring>& j_iceParameters,
  const JavaParamRef<jstring>& j_iceCandidates,
  const JavaParamRef<jstring>& j_dtlsParameters,
  const JavaParamRef<jobject>& j_config,
  jlong j_peerConnection_factory,
  const JavaParamRef<jstring>& j_appData)
{
	MSC_TRACE();

	try
	{
		auto listener       = new RecvTransportListenerJni(env, j_listener);
		auto iceParameters  = JavaToNativeString(env, j_iceParameters);
		auto iceCandidates  = JavaToNativeString(env, j_iceCandidates);
		auto dtlsParameters = JavaToNativeString(env, j_dtlsParameters);

		PeerConnection::Options options;
		JavaToNativeOptions(env, j_config, j_peerConnection_factory, options);

		json appData = nlohmann::json::object();
		if (j_appData != nullptr)
		{
			appData = json::parse(JavaToNativeString(env, j_appData));
		}

		auto transport = reinterpret_cast<Device*>(j_device)->CreateRecvTransport(
		  listener,
		  JavaToNativeString(env, j_id),
		  json::parse(iceParameters),
		  json::parse(iceCandidates),
		  json::parse(dtlsParameters),
		  &options,
		  appData);

		return NativeToJavaRecvTransport(env, transport, listener);
	}
	catch (const std::exception& e)
	{
		MSC_ERROR("%s", e.what());
		THROW_MEDIASOUP_CLIENT_EXCEPTION(env, e);
	}
	return nullptr;
}

} // namespace mediasoupclient
