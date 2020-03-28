package org.mediasoup.droid;

import androidx.annotation.Nullable;

import org.webrtc.CalledByNative;
import org.webrtc.MediaStreamTrack;
import org.webrtc.RTCUtils;

public class Consumer {

  public interface Listener {

    @CalledByNative("Listener")
    void onTransportClose(Consumer consumer);
  }

  private long mNativeConsumer;

  @Nullable private MediaStreamTrack mCachedTrack;

  @CalledByNative
  public Consumer(long nativeConsumer) {
    mNativeConsumer = nativeConsumer;
    long nativeTrack = nativeGetTrack(mNativeConsumer);
    mCachedTrack = RTCUtils.createMediaStreamTrack(nativeTrack);
  }

  public String getId() {
    return nativeGetNativeId(mNativeConsumer);
  }

  public String getProducerId() {
    return nativeGetProducerId(mNativeConsumer);
  }

  public boolean isClosed() {
    return nativeIsClosed(mNativeConsumer);
  }

  public boolean isPaused() {
    return nativeIsPaused(mNativeConsumer);
  }

  public String getKind() {
    return nativeGetKind(mNativeConsumer);
  }

  public MediaStreamTrack getTrack() {
    return mCachedTrack;
  }

  public String getRtpParameters() {
    return nativeGetRtpParameters(mNativeConsumer);
  }

  public String getAppData() {
    return nativeGetAppData(mNativeConsumer);
  }

  public void resume() {
    nativeResume(mNativeConsumer);
  }

  public void pause() {
    nativePause(mNativeConsumer);
  }

  public String getStats() throws MediasoupException {
    return nativeGetStats(mNativeConsumer);
  }

  public void close() {
    nativeClose(mNativeConsumer);
  }

  private static native String nativeGetNativeId(long consumer);

  private static native String nativeGetProducerId(long consumer);

  private static native boolean nativeIsClosed(long consumer);

  private static native boolean nativeIsPaused(long consumer);

  private static native String nativeGetKind(long consumer);

  private static native long nativeGetTrack(long consumer);

  private static native String nativeGetRtpParameters(long consumer);

  private static native String nativeGetAppData(long consumer);

  private static native void nativeResume(long consumer);

  private static native void nativePause(long consumer);

  // may throws MediasoupException;
  private static native String nativeGetStats(long consumer);

  private static native void nativeClose(long consumer);
}
