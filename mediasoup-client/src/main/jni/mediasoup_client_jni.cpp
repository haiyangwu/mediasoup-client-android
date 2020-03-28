#include "generated_mediasoupclient_jni/jni/MediasoupClient_jni.h"
#include "mediasoupclient.hpp"
#include <include/java_types.h>
#include <sdk/android/native_api/jni/java_types.h>

namespace mediasoupclient
{
static ScopedJavaLocalRef<jstring> JNI_MediasoupClient_Version(JNIEnv* env)
{
	std::string version = mediasoupclient::Version();
	return NativeToJavaString(env, version);
}
} // namespace mediasoupclient
