package org.mediasoup.droid;

import androidx.annotation.Nullable;

import org.webrtc.CalledByNative;
import org.webrtc.MediaStreamTrack;
import org.webrtc.RTCUtils;

public class Producer {

  public interface Listener {

    @CalledByNative("Listener")
    void onTransportClose(Producer producer);
  }

  private long mNativeProducer;
  @Nullable private MediaStreamTrack mCachedTrack;

  @CalledByNative
  public Producer(long nativeProducer) {
    mNativeProducer = nativeProducer;
    long nativeTrack = nativeGetTrack(mNativeProducer);
    mCachedTrack = RTCUtils.createMediaStreamTrack(nativeTrack);
  }

  public String getId() {
    return nativeGetId(mNativeProducer);
  }

  public boolean isClosed() {
    return nativeIsClosed(mNativeProducer);
  }

  public String getKind() {
    return nativeGetKind(mNativeProducer);
  }

  public MediaStreamTrack getTrack() {
    return mCachedTrack;
  }

  public boolean isPaused() {
    return nativeIsPaused(mNativeProducer);
  }

  public int getMaxSpatialLayer() {
    return nativeGetMaxSpatialLayer(mNativeProducer);
  }

  public String getAppData() {
    return nativeGetAppData(mNativeProducer);
  }

  public String getRtpParameters() {
    return nativeGetRtpParameters(mNativeProducer);
  }

  public void resume() {
    nativeResume(mNativeProducer);
  }

  public void setMaxSpatialLayer(int layer) throws MediasoupException {
    nativeSetMaxSpatialLayer(mNativeProducer, layer);
  }

  public void pause() {
    nativePause(mNativeProducer);
  }

  public void replaceTrack(MediaStreamTrack track) throws MediasoupException {
    long nativeMediaStreamTrack = RTCUtils.getNativeMediaStreamTrack(track);
    nativeReplaceTrack(mNativeProducer, nativeMediaStreamTrack);
    mCachedTrack = track;
  }

  public String getStats() throws MediasoupException {
    return nativeGetStats(mNativeProducer);
  }

  public void close() {
    nativeClose(mNativeProducer);
  }

  private static native String nativeGetId(long producer);

  private static native boolean nativeIsClosed(long producer);

  private static native String nativeGetKind(long producer);

  private static native long nativeGetTrack(long producer);

  private static native boolean nativeIsPaused(long producer);

  private static native int nativeGetMaxSpatialLayer(long producer);

  private static native String nativeGetAppData(long producer);

  private static native String nativeGetRtpParameters(long producer);

  // may throws MediasoupException
  private static native String nativeGetStats(long producer);

  private static native void nativeResume(long producer);

  // may throws MediasoupException;
  private static native void nativeSetMaxSpatialLayer(long producer, int layer);

  private static native void nativePause(long producer);

  // may throws MediasoupException;
  private static native void nativeReplaceTrack(long producer, long track);

  private static native void nativeClose(long producer);
}
