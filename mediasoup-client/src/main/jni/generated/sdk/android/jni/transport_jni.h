#ifndef GEN_MEDIASOUP_CLIENT_ANDROID_TRANSPORT_JNI_H
#define GEN_MEDIASOUP_CLIENT_ANDROID_TRANSPORT_JNI_H

#include <jni.h>
#include <sdk/android/src/jni/jni_generator_helper.h>

const char kClassPath_org_mediasoup_droid_Transport[] =
        "org/mediasoup/droid/Transport";
const char kClassPath_org_mediasoup_droid_Transport_Listener[] =
        "org/mediasoup/droid/Transport$Listener";

JNI_GENERATOR_EXPORT
std::atomic<jclass> g_org_mediasoup_droid_Transport_clazz(nullptr);
// Leaking this jclass as we cannot use LazyInstance from some threads.
#ifndef org_mediasoup_droid_Transport_clazz_defined
#define org_mediasoup_droid_Transport_clazz_defined

inline jclass org_mediasoup_droid_Transport_clazz(JNIEnv *env) {
    return base::android::LazyGetClass(env, kClassPath_org_mediasoup_droid_Transport,
                                       &g_org_mediasoup_droid_Transport_clazz);
}

#endif

JNI_GENERATOR_EXPORT
std::atomic<jclass> g_org_mediasoup_droid_Transport_Listener_clazz(nullptr);
// Leaking this jclass as we cannot use LazyInstance from some threads.
#ifndef org_mediasoup_droid_Transport_Listener_clazz_defined
#define org_mediasoup_droid_Transport_Listener_clazz_defined

inline jclass org_mediasoup_droid_Transport_Listener_clazz(JNIEnv *env) {
    return base::android::LazyGetClass(env, kClassPath_org_mediasoup_droid_Transport_Listener,
                                       &g_org_mediasoup_droid_Transport_Listener_clazz);
}

#endif

static std::atomic<jmethodID> g_org_mediasoup_droid_Transport_getNativeTransport(
        nullptr);

static jlong Java_Mediasoup_Transport_getNativeTransport(
        JNIEnv *env,
        const base::android::JavaRef<jobject> &obj) {
    jclass clazz = org_mediasoup_droid_Transport_clazz(env);
    CHECK_CLAZZ(env, obj.obj(),
                org_mediasoup_droid_Transport_clazz(env), 0);

    jni_generator::JniJavaCallContextChecked call_context;
    call_context.Init<
            base::android::MethodID::TYPE_INSTANCE>(
            env,
            clazz,
            "getNativeTransport",
            "()J",
            &g_org_mediasoup_droid_Transport_getNativeTransport);

    jlong ret = env->CallLongMethod(obj.obj(), call_context.base.method_id);
    return ret;
}

static std::atomic<jmethodID> g_org_mediasoup_droid_Transport_Listener_onConnect(nullptr);

static void
Java_Mediasoup_Listener_OnConnect(JNIEnv *env, const base::android::JavaRef<jobject> &obj,
                                  const base::android::JavaRef<jobject> &transport,
                                  const base::android::JavaRef<jstring> &dtlsParameters) {
    jclass clazz = org_mediasoup_droid_Transport_Listener_clazz(env);
    CHECK_CLAZZ(env, obj.obj(),
                org_mediasoup_droid_Transport_Listener_clazz(env));

    jni_generator::JniJavaCallContextChecked call_context;
    call_context.Init<
            base::android::MethodID::TYPE_INSTANCE>(
            env,
            clazz,
            "onConnect",
            "(Lorg/mediasoup/droid/Transport;Ljava/lang/String;)V",
            &g_org_mediasoup_droid_Transport_Listener_onConnect);

    env->CallVoidMethod(obj.obj(),
                        call_context.base.method_id, transport.obj(), dtlsParameters.obj());
}

static std::atomic<jmethodID> g_org_mediasoup_droid_Transport_Listener_onConnectionStateChange(
        nullptr);

static void
Java_Mediasoup_Listener_OnConnectionStateChange(JNIEnv *env,
                                                const base::android::JavaRef<jobject> &obj,
                                                const base::android::JavaRef<jobject> &transport,
                                                const base::android::JavaRef<jstring> &connectionState) {
    jclass clazz = org_mediasoup_droid_Transport_Listener_clazz(env);
    CHECK_CLAZZ(env, obj.obj(),
                org_mediasoup_droid_Transport_Listener_clazz(env));

    jni_generator::JniJavaCallContextChecked call_context;
    call_context.Init<
            base::android::MethodID::TYPE_INSTANCE>(
            env,
            clazz,
            "onConnectionStateChange",
            "(Lorg/mediasoup/droid/Transport;Ljava/lang/String;)V",
            &g_org_mediasoup_droid_Transport_Listener_onConnectionStateChange);

    env->CallVoidMethod(obj.obj(),
                        call_context.base.method_id, transport.obj(), connectionState.obj());
}

#endif //GEN_MEDIASOUP_CLIENT_ANDROID_TRANSPORT_JNI_H
