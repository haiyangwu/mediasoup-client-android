package org.mediasoup.droid;

import android.support.annotation.Nullable;

import org.webrtc.CalledByNative;
import org.webrtc.MediaStreamTrack;
import org.webrtc.RTCUtils;

public class Producer {

  public interface Listener {

    @CalledByNative
    void onTransportClose(Producer producer);
  }

  private long mNativeProducer;
  @Nullable
  private MediaStreamTrack mCachedTrack;

  @CalledByNative
  public Producer(long nativeProducer) {
    mNativeProducer = nativeProducer;
    long nativeTrack =  getNativeTrack(mNativeProducer);
    mCachedTrack = RTCUtils.createMediaStreamTrack(nativeTrack);
  }

  public String getId() {
    return getNativeId(mNativeProducer);
  }

  public boolean isClosed() {
    return isNativeClosed(mNativeProducer);
  }

  public String getKind() {
    return getNativeKind(mNativeProducer);
  }

  public MediaStreamTrack getTrack() {
    return mCachedTrack;
  }

  public boolean isPaused() {
    return isNativePaused(mNativeProducer);
  }

  public int getMaxSpatialLayer() {
    return getNativeMaxSpatialLayer(mNativeProducer);
  }

  public String getAppData() {
    return getNativeAppData(mNativeProducer);
  }

  public String getRtpParameters() {
    return getNativeRtpParameters(mNativeProducer);
  }

  public void resume() {
    nativeResume(mNativeProducer);
  }

  public void setMaxSpatialLayer(int layer) {
    setNativeMaxSpatialLayer(mNativeProducer, layer);
  }

  public void pause() {
    nativePause(mNativeProducer);
  }

  public void replaceTrack(MediaStreamTrack track) {
    long nativeMediaStreamTrack = RTCUtils.getNativeMediaStreamTrack(track);
    nativeReplaceTrack(mNativeProducer, nativeMediaStreamTrack);
    mCachedTrack = track;
  }

  public String getStats() {
    return getNativeStats(mNativeProducer);
  }

  public void close() {
    nativeClose(mNativeProducer);
  }

  private static native String getNativeId(long nativeProducer);

  private static native boolean isNativeClosed(long nativeProducer);

  private static native String getNativeKind(long nativeProducer);

  private static native long getNativeTrack(long nativeProducer);

  private static native boolean isNativePaused(long nativeProducer);

  private static native int getNativeMaxSpatialLayer(long nativeProducer);

  private static native String getNativeAppData(long nativeProducer);

  private static native String getNativeRtpParameters(long nativeProducer);

  private static native String getNativeStats(long nativeProducer);

  private static native void nativeResume(long nativeProducer);

  private static native void setNativeMaxSpatialLayer(long nativeProducer, int layer);

  private static native void nativePause(long nativeProducer);

  private static native void nativeReplaceTrack(long nativeProducer, long track);

  private static native void nativeClose(long nativeProducer);
}
