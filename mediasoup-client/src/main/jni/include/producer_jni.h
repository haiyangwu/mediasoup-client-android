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
	}

	void OnTransportClose(Producer* producer) override;

public:
	void SetJProducer(JNIEnv* env, const JavaRef<jobject>& j_producer)
	{
		j_producer_.Reset(j_producer);
	}

private:
	const ScopedJavaGlobalRef<jobject> j_listener_;
	ScopedJavaGlobalRef<jobject> j_producer_;
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
		delete listener_;
		delete producer_;
	}

	Producer* producer() const
	{
		return producer_;
	}

private:
	Producer* producer_;
	ProducerListenerJni* listener_;
};

ScopedJavaLocalRef<jobject> NativeToJavaProducer(
  JNIEnv* env, Producer* producer, ProducerListenerJni* listener);

} // namespace mediasoupclient

#endif // MEDIASOUP_CLIENT_ANDROID_PRODUCER_JNI_H
