#include "generated_mediasoupclient_jni/jni/Parameters_jni.h"
#include "fakeParameters.hpp"
#include "json.hpp"
#include <include/java_types.h>
#include <jni.h>

using json = nlohmann::json;

namespace mediasoupclient
{
namespace test
{
	static ScopedJavaLocalRef<jstring> JNI_Parameters_GenRouterRtpCapabilities(JNIEnv* env)
	{
		json routerRtpCapabilities = generateRouterRtpCapabilities();
		return NativeToJavaString(env, routerRtpCapabilities.dump());
	}

	static ScopedJavaLocalRef<jstring> JNI_Parameters_GenRouterRtpCapabilitiesExclude(
	  JNIEnv* env, const JavaParamRef<jstring>& j_exclude)
	{
		std::string exclude        = JavaToNativeString(env, j_exclude);
		json routerRtpCapabilities = generateRouterRtpCapabilities();
		for (auto& codec : routerRtpCapabilities["codecs"])
			codec.erase(exclude);

		return NativeToJavaString(env, routerRtpCapabilities.dump());
	}

	static ScopedJavaLocalRef<jstring> JNI_Parameters_GenRtpParametersByKind(JNIEnv* env)
	{
		json rtpParametersByKind = generateRtpParametersByKind();
		return NativeToJavaString(env, rtpParametersByKind.dump());
	}

	static ScopedJavaLocalRef<jstring> JNI_Parameters_GenLocalDtlsParameters(JNIEnv* env)
	{
		json localDtlsParameters = generateLocalDtlsParameters();
		return NativeToJavaString(env, localDtlsParameters.dump());
	}

	static ScopedJavaLocalRef<jstring> JNI_Parameters_GenTransportRemoteParameters(JNIEnv* env)
	{
		json transportRemoteParameters = generateTransportRemoteParameters();
		return NativeToJavaString(env, transportRemoteParameters.dump());
	}

	static ScopedJavaLocalRef<jstring> JNI_Parameters_GenProducerRemoteId(JNIEnv* env)
	{
		std::string producerRemoteId = generateProducerRemoteId();
		return NativeToJavaString(env, producerRemoteId);
	}

	static ScopedJavaLocalRef<jstring> JNI_Parameters_GenConsumerRemoteParameters(
	  JNIEnv* env, const JavaParamRef<jstring>& j_codecMimeType)
	{
		std::string codecMimeType     = JavaToNativeString(env, j_codecMimeType);
		json consumerRemoteParameters = generateConsumerRemoteParameters(codecMimeType);
		return NativeToJavaString(env, consumerRemoteParameters.dump());
	}

} // namespace test
} // namespace mediasoupclient
