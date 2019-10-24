package org.mediasoup.droid;

import org.webrtc.CalledByNative;

public class Producer {

  public interface Listener {

    @CalledByNative
    void onTransportClose();
  }

  private long mNativeProducer;

  public Producer(long nativeProducer) {
    mNativeProducer = nativeProducer;
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

  // TODO(haiyangwu): return native track pointer for now.
  public long getTrack() {
    return getNativeTrack(mNativeProducer);
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

  private static native String getNativeId(long nativeProducer);

  private static native boolean isNativeClosed(long nativeProducer);

  private static native String getNativeKind(long nativeProducer);

  private static native long getNativeTrack(long nativeProducer);

  private static native boolean isNativePaused(long nativeProducer);

  private static native int getNativeMaxSpatialLayer(long nativeProducer);

  private static native String getNativeAppData(long nativeProducer);

  private static native String getNativeRtpParameters(long nativeProducer);

  private static native void nativeResume(long nativeProducer);

  private static native void setNativeMaxSpatialLayer(long nativeProducer, int layer);
}
