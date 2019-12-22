#ifndef MEDIASOUP_CLIENT_ANDROID_CONSUMER_JNI_H
#define MEDIASOUP_CLIENT_ANDROID_CONSUMER_JNI_H

#include "common_jni.h"
#include <Consumer.hpp>
#include <jni.h>

namespace mediasoupclient
{
class OwnedConsumer
{
public:
	OwnedConsumer(Consumer* consumer, Consumer::Listener* listener)
	  : consumer_(consumer), listener_(listener)
	{
	}

	~OwnedConsumer() = default;

	Consumer* consumer() const
	{
		return consumer_.get();
	}

private:
	std::unique_ptr<Consumer> consumer_;
	std::unique_ptr<Consumer::Listener> listener_;
};

class ConsumerListenerJNI : public Consumer::Listener
{
public:
	ConsumerListenerJNI(JNIEnv* env, const JavaRef<jobject>& j_listener_);

	~ConsumerListenerJNI()
	{
		if (j_consumer_ != nullptr)
		{
			webrtc::AttachCurrentThreadIfNeeded()->DeleteGlobalRef(j_consumer_);
		}
	}

	void OnTransportClose(Consumer* consumer) override;

public:
	void SetConsumer(JNIEnv* env, const JavaRef<jobject>& j_consumer)
	{
		j_consumer_ = env->NewGlobalRef(j_consumer.obj());
	}

private:
	const ScopedJavaGlobalRef<jobject> j_listener_;
	jobject j_consumer_;
};

} // namespace mediasoupclient

#endif // MEDIASOUP_CLIENT_ANDROID_CONSUMER_JNI_H
