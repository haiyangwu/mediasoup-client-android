// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "include/scoped_java_ref.h"

namespace mediasoupclient
{
using webrtc::AttachCurrentThreadIfNeeded;

#if RTC_DCHECK_IS_ON
// This constructor is inlined when DCHECKs are disabled; don't add anything
// else here.
JavaRef<jobject>::JavaRef(JNIEnv* env, jobject obj) : obj_(obj)
{
	if (obj)
	{
		RTC_DCHECK(
		  env && (env->GetObjectRefType(obj) == JNILocalRefType ||
		          env->GetObjectRefType(obj) == JNIGlobalRefType));
	}
}
#endif

JNIEnv* JavaRef<jobject>::SetNewLocalRef(JNIEnv* env, jobject obj)
{
	if (!env)
	{
		env = AttachCurrentThreadIfNeeded();
	}
	else
	{
		RTC_DCHECK_EQ(env, AttachCurrentThreadIfNeeded()); // Is |env| on correct thread.
	}
	if (obj)
		obj = env->NewLocalRef(obj);
	if (obj_)
		env->DeleteLocalRef(obj_);
	obj_ = obj;
	return env;
}

void JavaRef<jobject>::SetNewGlobalRef(JNIEnv* env, jobject obj)
{
	if (!env)
	{
		env = AttachCurrentThreadIfNeeded();
	}
	else
	{
		RTC_DCHECK_EQ(env, AttachCurrentThreadIfNeeded()); // Is |env| on correct thread.
	}
	if (obj)
		obj = env->NewGlobalRef(obj);
	if (obj_)
		env->DeleteGlobalRef(obj_);
	obj_ = obj;
}

void JavaRef<jobject>::ResetLocalRef(JNIEnv* env)
{
	if (obj_)
	{
		RTC_DCHECK_EQ(env, AttachCurrentThreadIfNeeded()); // Is |env| on correct thread.
		env->DeleteLocalRef(obj_);
		obj_ = nullptr;
	}
}

void JavaRef<jobject>::ResetGlobalRef()
{
	if (obj_)
	{
		AttachCurrentThreadIfNeeded()->DeleteGlobalRef(obj_);
		obj_ = nullptr;
	}
}

jobject JavaRef<jobject>::ReleaseInternal()
{
	jobject obj = obj_;
	obj_        = nullptr;
	return obj;
}

} // namespace mediasoupclient
