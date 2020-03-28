#include "Logger.hpp"
#include <jni.h>
#include <sdk/android/native_api/jni/java_types.h>

#include "generated_mediasoupclient_jni/jni/Logger_jni.h"
#define TAG "mediasoupclient-jni"

namespace mediasoupclient
{
class LogHandlerInterfaceJNI : public Logger::LogHandlerInterface
{
public:
	LogHandlerInterfaceJNI(JNIEnv* env, const JavaParamRef<jobject>& j_handler_interface)
	  : j_handler_interface_(env, j_handler_interface),
	    jni_tag_(env, webrtc::NativeToJavaString(env, TAG).obj())
	{
		assert(!j_handler_interface_.is_null());
	}

public:
	void OnLog(Logger::LogLevel level, char* payload, size_t len) override
	{
		std::string message(payload, len);
		auto env       = webrtc::AttachCurrentThreadIfNeeded();
		auto j_level   = Java_LogLevel_getLogLevel(env, static_cast<int>(level));
		auto j_message = webrtc::NativeToJavaString(env, message);
		Java_LogHandlerInterface_OnLog(
		  env, j_handler_interface_, j_level, jni_tag_, JavaParamRef<jstring>(env, j_message.obj()));
	}

private:
	ScopedJavaGlobalRef<jobject> j_handler_interface_;
	ScopedJavaGlobalRef<jstring> jni_tag_;
};

static void JNI_Logger_SetLogLevel(JNIEnv* env, jint j_level)
{
	Logger::SetLogLevel(static_cast<Logger::LogLevel>(j_level));
}

static jlong JNI_Logger_SetHandler(JNIEnv* env, const JavaParamRef<jobject>& j_handler)
{
	auto* handler = new LogHandlerInterfaceJNI(env, j_handler);
	Logger::SetHandler(reinterpret_cast<Logger::LogHandlerInterface*>(handler));
	return webrtc::NativeToJavaPointer(handler);
}

static void JNI_Logger_FreeLogHandler(JNIEnv* env, jlong j_handler)
{
	auto* handler = reinterpret_cast<LogHandlerInterfaceJNI*>(j_handler);
	delete handler;
}

} // namespace mediasoupclient
