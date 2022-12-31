package org.mediasoup.droid;

import android.content.Context;

import org.webrtc.PeerConnectionFactory;
import org.webrtc.PeerConnectionFactory.InitializationOptions;

public class MediasoupClient {

  static {
    System.loadLibrary("mediasoupclient_so");
  }

  /**
   * libmediasoupclient initialization. Initializes libwebrtc.
   *
   * @param appContext app context
   */
  public static void initialize(Context appContext) {
    initialize(appContext, null);
  }

  /**
   * libmediasoupclient initialization. Initializes libwebrtc.
   *
   * @param appContext app context
   * @param fieldTrials fieldTrials desc
   */
  public static void initialize(Context appContext, String fieldTrials) {
    InitializationOptions options =
        InitializationOptions.builder(appContext)
            .setFieldTrials(fieldTrials)
            .setEnableInternalTracer(true)
            .setNativeLibraryName("mediasoupclient_so")
            .createInitializationOptions();
    PeerConnectionFactory.initialize(options);
  }

  /**
   * @return The libmediasoupclient version.
   */
  public static String version() {
    return nativeVersion();
  }

  private static native String nativeVersion();
}
