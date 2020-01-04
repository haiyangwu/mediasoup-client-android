#ifndef MEDIASOUP_CLIENT_ANDROID_CONSUMER_JNI_H
#define MEDIASOUP_CLIENT_ANDROID_CONSUMER_JNI_H

#include "common_jni.h"
#include <Consumer.hpp>
#include <jni.h>

namespace mediasoupclient
{
class ConsumerListenerJni final : public Consumer::Listener
{
public:
	ConsumerListenerJni(JNIEnv* env, const JavaRef<jobject>& j_listener);

	~ConsumerListenerJni()
	{
		if (j_consumer_ != nullptr)
		{
			webrtc::AttachCurrentThreadIfNeeded()->DeleteGlobalRef(j_consumer_);
		}
	}

	void OnTransportClose(Consumer* native_consumer) override;

public:
	void SetJConsumer(JNIEnv* env, const JavaRef<jobject>& j_consumer)
	{
		j_consumer_ = env->NewGlobalRef(j_consumer.obj());
	}

private:
	const ScopedJavaGlobalRef<jobject> j_listener_global_;
	jobject j_consumer_;
};

class OwnedConsumer
{
public:
	OwnedConsumer(Consumer* consumer, ConsumerListenerJni* listener)
	  : consumer_(consumer), listener_(listener)
	{
	}

	~OwnedConsumer()
	{
		delete listener_;
		delete consumer_;
	}

	Consumer* consumer() const
	{
		return consumer_;
	}

private:
	Consumer* consumer_;
	ConsumerListenerJni* listener_;
};

} // namespace mediasoupclient

#endif // MEDIASOUP_CLIENT_ANDROID_CONSUMER_JNI_H
