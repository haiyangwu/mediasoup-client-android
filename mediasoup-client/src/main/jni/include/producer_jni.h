#ifndef MEDIASOUP_CLIENT_ANDROID_PRODUCER_JNI_H
#define MEDIASOUP_CLIENT_ANDROID_PRODUCER_JNI_H

#include "Producer.hpp"
#include "common_jni.h"
#include <jni.h>

namespace mediasoupclient
{
class ProducerListenerJni final : public Producer::Listener
{
public:
	ProducerListenerJni(JNIEnv* env, const JavaRef<jobject>& j_listener_);

	~ProducerListenerJni()
	{
		if (j_producer_ != nullptr)
		{
			webrtc::AttachCurrentThreadIfNeeded()->DeleteGlobalRef(j_producer_);
		}
	}

	void OnTransportClose(Producer* producer) override;

public:
	void SetJProducer(JNIEnv* env, const JavaRef<jobject>& j_producer)
	{
		j_producer_ = env->NewGlobalRef(j_producer.obj());
	}

private:
	const ScopedJavaGlobalRef<jobject> j_listener_;
	jobject j_producer_;
};

class OwnedProducer
{
public:
	OwnedProducer(Producer* producer, ProducerListenerJni* listener)
	  : producer_(producer), listener_(listener)
	{
	}

	~OwnedProducer()
	{
		delete producer_;
		delete listener_;
	}

	Producer* producer() const
	{
		return producer_;
	}

private:
	Producer* producer_;
	ProducerListenerJni* listener_;
};

} // namespace mediasoupclient

#endif // MEDIASOUP_CLIENT_ANDROID_PRODUCER_JNI_H
