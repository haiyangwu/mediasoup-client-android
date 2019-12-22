#ifndef MEDIASOUP_CLIENT_ANDROID_PRODUCER_JNI_H
#define MEDIASOUP_CLIENT_ANDROID_PRODUCER_JNI_H

#include "Producer.hpp"
#include "common_jni.h"
#include <jni.h>

namespace mediasoupclient
{
class OwnedProducer
{
public:
	OwnedProducer(Producer* producer, Producer::Listener* listener)
	  : producer_(producer), listener_(listener)
	{
	}

	~OwnedProducer() = default;

	Producer* producer() const
	{
		return producer_.get();
	}

private:
	std::unique_ptr<Producer> producer_;
	std::unique_ptr<Producer::Listener> listener_;
};

class ProducerListenerJNI : public Producer::Listener
{
public:
	ProducerListenerJNI(JNIEnv* env, const JavaRef<jobject>& j_listener_);

	~ProducerListenerJNI()
	{
		if (j_producer_ != nullptr)
		{
			webrtc::AttachCurrentThreadIfNeeded()->DeleteGlobalRef(j_producer_);
		}
	}

	void OnTransportClose(Producer* producer) override;

public:
	void SetProducer(JNIEnv* env, const JavaRef<jobject>& j_producer)
	{
		j_producer_ = env->NewGlobalRef(j_producer.obj());
	}

private:
	const ScopedJavaGlobalRef<jobject> j_listener_;
	jobject j_producer_;
};

} // namespace mediasoupclient

#endif // MEDIASOUP_CLIENT_ANDROID_PRODUCER_JNI_H
