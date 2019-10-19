#ifndef MEDIASOUP_CLIENT_ANDROID_COMMON_JNI_H
#define MEDIASOUP_CLIENT_ANDROID_COMMON_JNI_H

#include "json.hpp"
#include "sdk/android/native_api/jni/java_types.h"

using webrtc::JavaParamRef;
using webrtc::JavaRef;

using webrtc::NativeToJavaString;
using webrtc::JavaToNativeString;
using webrtc::NativeToJavaPointer;
using webrtc::NativeToJavaList;
using webrtc::ScopedJavaLocalRef;
using webrtc::ScopedJavaGlobalRef;

using nlohmann::json;

#endif //MEDIASOUP_CLIENT_ANDROID_COMMON_JNI_H
