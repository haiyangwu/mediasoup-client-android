package org.mediasoup.droid;

public class MediasoupClient {

    static {
        System.loadLibrary("mediasoupclient_so");
    }

    public static void initialize() {
        nativeInitialize();
    }

    public static String version() {
        return nativeVersion();
    }

    public static void cleanup() {
        nativeCleanup();
    }

    private static native void nativeInitialize();

    private static native void nativeCleanup();

    private static native String nativeVersion();
}
