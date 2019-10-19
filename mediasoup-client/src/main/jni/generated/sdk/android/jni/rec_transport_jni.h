#ifndef GEN_MEDIASOUP_CLIENT_ANDROID_REC_TRANSPORT_JNI_H
#define GEN_MEDIASOUP_CLIENT_ANDROID_REC_TRANSPORT_JNI_H

#include <jni.h>
#include "sdk/android/src/jni/jni_generator_helper.h"

const char kClassPath_org_mediasoup_droid_RecvTransport[] =
        "org/mediasoup/droid/RecvTransport";
const char kClassPath_org_mediasoup_droid_RecvTransport_Listener[] =
        "org/mediasoup/droid/RecvTransport$Listener";

JNI_GENERATOR_EXPORT
std::atomic<jclass> g_org_mediasoup_droid_RecvTransport_clazz(nullptr);
// Leaking this jclass as we cannot use LazyInstance from some threads.
#ifndef org_mediasoup_droid_RecvTransport_clazz_defined
#define org_mediasoup_droid_RecvTransport_clazz_defined

inline jclass org_mediasoup_droid_RecvTransport_clazz(JNIEnv *env) {
    return base::android::LazyGetClass(env, kClassPath_org_mediasoup_droid_RecvTransport,
                                       &g_org_mediasoup_droid_RecvTransport_clazz);
}

#endif

JNI_GENERATOR_EXPORT
std::atomic<jclass> g_org_mediasoup_droid_RecvTransport_Listener_clazz(nullptr);
// Leaking this jclass as we cannot use LazyInstance from some threads.
#ifndef org_mediasoup_droid_RecvTransport_Listener_clazz_defined
#define org_mediasoup_droid_RecvTransport_Listener_clazz_defined

inline jclass org_mediasoup_droid_RecvTransport_Listener_clazz(JNIEnv *env) {
    return base::android::LazyGetClass(env, kClassPath_org_mediasoup_droid_RecvTransport_Listener,
                                       &g_org_mediasoup_droid_RecvTransport_Listener_clazz);
}

#endif

static std::atomic<jmethodID> g_org_mediasoup_droid_RecvTransport_Constructor(nullptr);

base::android::ScopedJavaLocalRef<jobject> Java_Mediasoup_RecvTransport_Constructor(
        JNIEnv *env, jlong nativeRecvTransport) {
    jclass clazz = org_mediasoup_droid_RecvTransport_clazz(env);
    CHECK_CLAZZ(env, clazz,
                org_mediasoup_droid_RecvTransport_clazz(env), NULL);

    jni_generator::JniJavaCallContextChecked call_context;
    call_context.Init<
            base::android::MethodID::TYPE_INSTANCE>(
            env,
            clazz,
            "<init>",
            "(J)V",
            &g_org_mediasoup_droid_RecvTransport_Constructor);

    jobject ret =
            env->NewObject(clazz,
                           call_context.base.method_id, nativeRecvTransport);
    return base::android::ScopedJavaLocalRef<jobject>(env, ret);
}

#endif //GEN_MEDIASOUP_CLIENT_ANDROID_REC_TRANSPORT_JNI_H
