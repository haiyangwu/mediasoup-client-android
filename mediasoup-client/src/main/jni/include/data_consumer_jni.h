#ifndef MEDIASOUP_CLIENT_ANDROID_DATA_CONSUMER_JNI_H
#define MEDIASOUP_CLIENT_ANDROID_DATA_CONSUMER_JNI_H

#include "common_jni.h"
#include <DataConsumer.hpp>
#include <jni.h>

namespace mediasoupclient
{
class DataConsumerListenerJni final : public DataConsumer::Listener
{
public:
	DataConsumerListenerJni(JNIEnv* env, const JavaRef<jobject>& j_listener);

	~DataConsumerListenerJni()
	{
	}

	void OnConnecting(DataConsumer* dataConsumer) override;

	void OnOpen(DataConsumer* dataConsumer) override;

	void OnClosing(DataConsumer* dataConsumer) override;

	void OnClose(DataConsumer* dataConsumer) override;

	void OnMessage(DataConsumer* dataConsumer, const webrtc::DataBuffer& buffer) override;

	void OnTransportClose(DataConsumer* dataConsumer) override;

public:
	void SetJConsumer(JNIEnv* env, const JavaRef<jobject>& j_consumer)
	{
		j_data_consumer_.Reset(j_consumer);
	}

private:
	const ScopedJavaGlobalRef<jobject> j_listener_global_;
	ScopedJavaGlobalRef<jobject> j_data_consumer_;
};

class OwnedDataConsumer
{
public:
	OwnedDataConsumer(DataConsumer* consumer, DataConsumerListenerJni* listener)
	  : consumer_(consumer), listener_(listener)
	{
	}

	~OwnedDataConsumer()
	{
		delete listener_;
		delete consumer_;
	}

	DataConsumer* consumer() const
	{
		return consumer_;
	}

private:
	DataConsumer* consumer_;
	DataConsumerListenerJni* listener_;
};

ScopedJavaLocalRef<jobject> NativeToJavaDataConsumer(
  JNIEnv* env, DataConsumer* consumer, DataConsumerListenerJni* listener);

} // namespace mediasoupclient

#endif // MEDIASOUP_CLIENT_ANDROID_DATA_CONSUMER_JNI_H
