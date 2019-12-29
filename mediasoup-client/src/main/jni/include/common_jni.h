#ifndef MEDIASOUP_CLIENT_ANDROID_COMMON_JNI_H
#define MEDIASOUP_CLIENT_ANDROID_COMMON_JNI_H

#include <json.hpp>
#include <sdk/android/native_api/jni/java_types.h>

#define MEDIASOUP_CLIENT_EXCEPTION "org/mediasoup/droid/MediasoupException"

#define THROW_JAVA_EXCEPTION(env, e, name)                                                         \
	jclass clazz = env->FindClass(name);                                                             \
	env->ThrowNew(clazz, e.what());                                                                  \
	env->DeleteLocalRef(clazz);

#define THROW_MEDIASOUP_CLIENT_EXCEPTION(env, e)                                                   \
	THROW_JAVA_EXCEPTION(env, e, MEDIASOUP_CLIENT_EXCEPTION)

namespace mediasoupclient
{
using webrtc::JavaParamRef;
using webrtc::JavaRef;

using webrtc::JavaToNativeString;
using webrtc::NativeToJavaList;
using webrtc::NativeToJavaPointer;
using webrtc::NativeToJavaString;
using webrtc::ScopedJavaGlobalRef;
using webrtc::ScopedJavaLocalRef;

using nlohmann::json;
} // namespace mediasoupclient

#endif // MEDIASOUP_CLIENT_ANDROID_COMMON_JNI_H
