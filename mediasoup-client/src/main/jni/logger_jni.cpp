#include "Logger.hpp"
#include <android/log.h>
#include <jni.h>

#define TAG "mediasoupclient-jni"
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

namespace mediasoupclient
{
class DefaultLogHandler : public Logger::LogHandlerInterface
{
public:
	void OnLog(Logger::LogLevel level, char* payload, size_t len) override
	{
		switch (level)
		{
			case Logger::LogLevel::LOG_ERROR:
				ALOGE("%s", payload);
				break;
			case Logger::LogLevel::LOG_WARN:
				ALOGW("%s", payload);
				break;
			case Logger::LogLevel::LOG_DEBUG:
				ALOGD("%s", payload);
				break;
			case Logger::LogLevel::LOG_TRACE:
				ALOGI("%s", payload);
				break;
			default:
				break;
		}
	}
};

extern "C" JNIEXPORT void JNICALL
Java_org_mediasoup_droid_Logger_nativeSetLogLevel(JNIEnv* /* env */, jclass /* type */, jint j_level)
{
	Logger::SetLogLevel(static_cast<Logger::LogLevel>(j_level));
}

extern "C" JNIEXPORT void JNICALL
Java_org_mediasoup_droid_Logger_nativeSetDefaultHandler(JNIEnv* /* env */, jclass /* type */)
{
	Logger::SetHandler(new DefaultLogHandler());
}

} // namespace mediasoupclient
