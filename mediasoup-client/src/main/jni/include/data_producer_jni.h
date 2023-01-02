#ifndef MEDIASOUP_CLIENT_ANDROID_DATA_PRODUCER_JNI_H
#define MEDIASOUP_CLIENT_ANDROID_DATA_PRODUCER_JNI_H

#include "DataProducer.hpp"
#include "common_jni.h"
#include <jni.h>

namespace mediasoupclient
{
class DataProducerListenerJni final : public DataProducer::Listener
{
public:
	DataProducerListenerJni(JNIEnv* env, const JavaRef<jobject>& j_listener_);

	~DataProducerListenerJni()
	{
	}

	void OnOpen(DataProducer* dataProducer);

	void OnClose(DataProducer* dataProducer);

	void OnBufferedAmountChange(DataProducer* dataProducer, uint64_t sentDataSize);

	void OnTransportClose(DataProducer* dataProducer);

public:
	void SetJProducer(JNIEnv* env, const JavaRef<jobject>& j_data_producer)
	{
		j_data_producer_.Reset(j_data_producer);
	}

private:
	const ScopedJavaGlobalRef<jobject> j_listener_;
	ScopedJavaGlobalRef<jobject> j_data_producer_;
};

class OwnedDataProducer
{
public:
	OwnedDataProducer(DataProducer* producer, DataProducerListenerJni* listener)
	  : data_producer_(producer), listener_(listener)
	{
	}

	~OwnedDataProducer()
	{
		delete listener_;
		delete data_producer_;
	}

	DataProducer* dataProducer() const
	{
		return data_producer_;
	}

private:
	DataProducer* data_producer_;
	DataProducerListenerJni* listener_;
};

ScopedJavaLocalRef<jobject> NativeToJavaDataProducer(
  JNIEnv* env, DataProducer* producer, DataProducerListenerJni* listener);

} // namespace mediasoupclient

#endif // MEDIASOUP_CLIENT_ANDROID_DATA_PRODUCER_JNI_H
