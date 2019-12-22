#ifndef MEDIASOUP_CLIENT_ANDROID_COMMON_JNI_H
#define MEDIASOUP_CLIENT_ANDROID_COMMON_JNI_H

#include "json.hpp"
#include <sdk/android/native_api/jni/java_types.h>

using webrtc::JavaParamRef;
using webrtc::JavaRef;

using webrtc::JavaToNativeString;
using webrtc::NativeToJavaList;
using webrtc::NativeToJavaPointer;
using webrtc::NativeToJavaString;
using webrtc::ScopedJavaGlobalRef;
using webrtc::ScopedJavaLocalRef;

using nlohmann::json;

#endif // MEDIASOUP_CLIENT_ANDROID_COMMON_JNI_H
