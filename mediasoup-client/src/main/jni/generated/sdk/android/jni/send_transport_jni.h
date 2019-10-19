#ifndef GEN_MEDIASOUP_CLIENT_ANDROID_SEND_TRANSPORT_JNI_H
#define GEN_MEDIASOUP_CLIENT_ANDROID_SEND_TRANSPORT_JNI_H

#include <jni.h>
#include "sdk/android/src/jni/jni_generator_helper.h"

const char kClassPath_org_mediasoup_droid_SendTransport[] =
        "org/mediasoup/droid/SendTransport";
const char kClassPath_org_mediasoup_droid_SendTransport_Listener[] =
        "org/mediasoup/droid/SendTransport$Listener";

JNI_GENERATOR_EXPORT
std::atomic<jclass> g_org_mediasoup_droid_SendTransport_clazz(nullptr);
// Leaking this jclass as we cannot use LazyInstance from some threads.
#ifndef org_mediasoup_droid_SendTransport_clazz_defined
#define org_mediasoup_droid_SendTransport_clazz_defined

inline jclass org_mediasoup_droid_SendTransport_clazz(JNIEnv *env) {
    return base::android::LazyGetClass(env, kClassPath_org_mediasoup_droid_SendTransport,
                                       &g_org_mediasoup_droid_SendTransport_clazz);
}

#endif

JNI_GENERATOR_EXPORT
std::atomic<jclass> g_org_mediasoup_droid_SendTransport_Listener_clazz(nullptr);
// Leaking this jclass as we cannot use LazyInstance from some threads.
#ifndef org_mediasoup_droid_SendTransport_Listener_clazz_defined
#define org_mediasoup_droid_SendTransport_Listener_clazz_defined

inline jclass org_mediasoup_droid_SendTransport_Listener_clazz(JNIEnv *env) {
    return base::android::LazyGetClass(env, kClassPath_org_mediasoup_droid_SendTransport_Listener,
                                       &g_org_mediasoup_droid_SendTransport_Listener_clazz);
}

#endif

static std::atomic<jmethodID> g_org_mediasoup_droid_SendTransport_Constructor(nullptr);

base::android::ScopedJavaLocalRef<jobject> Java_Mediasoup_SendTransport_Constructor(
        JNIEnv *env, jlong nativeSendTransport) {
    jclass clazz = org_mediasoup_droid_SendTransport_clazz(env);
    CHECK_CLAZZ(env, clazz,
                org_mediasoup_droid_SendTransport_clazz(env), NULL);

    jni_generator::JniJavaCallContextChecked call_context;
    call_context.Init<
            base::android::MethodID::TYPE_INSTANCE>(
            env,
            clazz,
            "<init>",
            "(J)V",
            &g_org_mediasoup_droid_SendTransport_Constructor);

    jobject ret =
            env->NewObject(clazz,
                           call_context.base.method_id, nativeSendTransport);
    return base::android::ScopedJavaLocalRef<jobject>(env, ret);
}

static std::atomic<jmethodID> g_org_mediasoup_droid_SendTransport_Listener_onProduce(nullptr);

static std::string
Java_Mediasoup_Listener_OnProduce(JNIEnv *env, const base::android::JavaRef<jobject> &obj,
                                  const base::android::JavaRef<jobject> &transport,
                                  const base::android::JavaRef<jstring> &kind,
                                  const base::android::JavaRef<jstring> &rtpParameters,
                                  const base::android::JavaRef<jstring> &appData) {
    jclass clazz = org_mediasoup_droid_SendTransport_Listener_clazz(env);
    CHECK_CLAZZ(env, obj.obj(),
                org_mediasoup_droid_SendTransport_Listener_clazz(env));

    jni_generator::JniJavaCallContextChecked call_context;
    call_context.Init<
            base::android::MethodID::TYPE_INSTANCE>(
            env,
            clazz,
            "onConnectionStateChange",
            "(Lorg/mediasoup/droid/Transport;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;",
            &g_org_mediasoup_droid_SendTransport_Listener_onProduce);

    env->CallVoidMethod(obj.obj(),
                        call_context.base.method_id, transport.obj(), kind.obj(),
                        rtpParameters.obj(), appData.obj());
}

#endif //GEN_MEDIASOUP_CLIENT_ANDROID_SEND_TRANSPORT_JNI_H
