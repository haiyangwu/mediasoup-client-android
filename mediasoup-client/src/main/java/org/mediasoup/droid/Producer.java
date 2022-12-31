package org.mediasoup.droid;

import androidx.annotation.Nullable;

import org.webrtc.CalledByNative;
import org.webrtc.MediaStreamTrack;
import org.webrtc.RTCUtils;

public class Producer {

  public interface Listener {

    /**
     * Executed when the transport this producer belongs to is closed for whatever reason. The
     * producer itself is also closed.
     *
     * @param producer The producer instance executing this method.
     */
    @CalledByNative("Listener")
    void onTransportClose(Producer producer);
  }

  private final long mNativeProducer;
  @Nullable private MediaStreamTrack mCachedTrack;

  @CalledByNative
  public Producer(long nativeProducer) {
    mNativeProducer = nativeProducer;
    long nativeTrack = nativeGetTrack(mNativeProducer);
    mCachedTrack = RTCUtils.createMediaStreamTrack(nativeTrack);
  }

  /**
   * @return Producer identifier.
   */
  public String getId() {
    return nativeGetId(mNativeProducer);
  }

  /**
   * @return Whether the producer is closed.
   */
  public boolean isClosed() {
    return nativeIsClosed(mNativeProducer);
  }

  /**
   * @return The media kind (“audio” or “video”).
   */
  public String getKind() {
    return nativeGetKind(mNativeProducer);
  }

  /**
   * @return The audio or video track being transmitted.
   */
  public MediaStreamTrack getTrack() {
    return mCachedTrack;
  }

  /**
   * @return Whether the producer is paused.
   */
  public boolean isPaused() {
    return nativeIsPaused(mNativeProducer);
  }

  /**
   * In case of simulcast, this value determines the highest stream (from 0 to N-1) being
   * transmitted. See the SetMaxSpatialLayer() method for more about this.
   */
  public int getMaxSpatialLayer() {
    return nativeGetMaxSpatialLayer(mNativeProducer);
  }

  /**
   * Custom data Object provided by the application in the producer factory method. The app can
   * modify its content at any time.
   */
  public String getAppData() {
    return nativeGetAppData(mNativeProducer);
  }

  /**
   * @return Producer RTP parameters. These parameters are internally built by the library and
   *     conform to the syntax and requirements of mediasoup, thus they can be transmitted to the
   *     server to invoke transport.produce() with them.
   */
  public String getRtpParameters() {
    return nativeGetRtpParameters(mNativeProducer);
  }

  /** Resumes the producer (RTP is sent again to the server). */
  public void resume() {
    nativeResume(mNativeProducer);
  }

  /**
   * In case of simulcast, this method limits the highest RTP stream being transmitted to the
   * server.
   *
   * @param layer The index of the entry in encodings representing the highest RTP stream that will
   *     be transmitted.
   * @throws MediasoupException
   */
  public void setMaxSpatialLayer(int layer) throws MediasoupException {
    nativeSetMaxSpatialLayer(mNativeProducer, layer);
  }

  /** Pauses the producer (no RTP is sent to the server). */
  public void pause() {
    nativePause(mNativeProducer);
  }

  /**
   * Replaces the audio or video track being transmitted. No negotiation with the server is needed.
   *
   * @param track An audio or video track.
   * @throws MediasoupException
   */
  public void replaceTrack(MediaStreamTrack track) throws MediasoupException {
    long nativeMediaStreamTrack = RTCUtils.getNativeMediaStreamTrack(track);
    nativeReplaceTrack(mNativeProducer, nativeMediaStreamTrack);
    mCachedTrack = track;
  }

  /**
   * Gets the local RTP sender statistics by calling getStats() in the underlying RTCRtpSender
   * instance.
   */
  public String getStats() throws MediasoupException {
    return nativeGetStats(mNativeProducer);
  }

  /** Closes the producer. No more media is transmitted. */
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
