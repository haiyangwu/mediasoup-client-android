//
// Created by WHY on 2020/3/28.
//

#ifndef MEDIASOUP_CLIENT_JAVA_TYPES_H_
#define MEDIASOUP_CLIENT_JAVA_TYPES_H_

#include "scoped_java_ref.h"
#include "sdk/android/native_api/jni/java_types.h"
#include <PeerConnection.hpp>
#include <sdk/android/src/jni/pc/peer_connection.h>

namespace mediasoupclient
{
ScopedJavaLocalRef<jstring> NativeToJavaString(JNIEnv* jni, const absl::optional<std::string>& str);

std::string JavaToNativeString(JNIEnv* jni, const JavaRef<jstring>& j_string);

void JavaToNativeOptions(
  JNIEnv* env, const JavaRef<jobject>& configuration, jlong factory, PeerConnection::Options& options);
} // namespace mediasoupclient

#endif // MEDIASOUP_CLIENT_JAVA_TYPES_H_
