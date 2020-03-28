#ifndef MEDIASOUP_CLIENT_ANDROID_COMMON_JNI_H
#define MEDIASOUP_CLIENT_ANDROID_COMMON_JNI_H

#include "include/scoped_java_ref.h"
#include <json.hpp>

#define MEDIASOUP_CLIENT_EXCEPTION "org/mediasoup/droid/MediasoupException"

#define THROW_JAVA_EXCEPTION(env, e, name)                                                         \
	jclass clazz = env->FindClass(name);                                                             \
	env->ThrowNew(clazz, e.what());                                                                  \
	env->DeleteLocalRef(clazz);

#define THROW_MEDIASOUP_CLIENT_EXCEPTION(env, e)                                                   \
	THROW_JAVA_EXCEPTION(env, e, MEDIASOUP_CLIENT_EXCEPTION)

namespace mediasoupclient
{
using nlohmann::json;
} // namespace mediasoupclient

#endif // MEDIASOUP_CLIENT_ANDROID_COMMON_JNI_H
