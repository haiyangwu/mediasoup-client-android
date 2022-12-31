package org.mediasoup.droid;

import androidx.annotation.Nullable;

import org.webrtc.CalledByNative;
import org.webrtc.MediaStreamTrack;
import org.webrtc.RTCUtils;

public class Consumer {

  public interface Listener {

    /**
     * Executed when the transport this consumer belongs to is closed for whatever reason. The
     * consumer itself is also closed.
     *
     * @param consumer The consumer instance executing this method.
     */
    @CalledByNative("Listener")
    void onTransportClose(Consumer consumer);
  }

  private final long mNativeConsumer;

  @Nullable private final MediaStreamTrack mCachedTrack;

  @CalledByNative
  public Consumer(long nativeConsumer) {
    mNativeConsumer = nativeConsumer;
    long nativeTrack = nativeGetTrack(mNativeConsumer);
    mCachedTrack = RTCUtils.createMediaStreamTrack(nativeTrack);
  }

  /**
   * @return Consumer identifier.
   */
  public String getId() {
    return nativeGetNativeId(mNativeConsumer);
  }

  /**
   * @return Consumer local identifier.
   */
  public String getLocalId() {
    return nativeGetLocalId(mNativeConsumer);
  }

  /**
   * @return The associated producer identifier.
   */
  public String getProducerId() {
    return nativeGetProducerId(mNativeConsumer);
  }

  /**
   * @return Whether the consumer is closed.
   */
  public boolean isClosed() {
    return nativeIsClosed(mNativeConsumer);
  }

  /**
   * @return Whether the consumer is paused.
   */
  public boolean isPaused() {
    return nativeIsPaused(mNativeConsumer);
  }

  /**
   * @return The media kind (“audio” or “video”).
   */
  public String getKind() {
    return nativeGetKind(mNativeConsumer);
  }

  /**
   * @return The remote audio or video track.
   */
  public MediaStreamTrack getTrack() {
    return mCachedTrack;
  }

  /**
   * @return Consumer RTP parameters.
   */
  public String getRtpParameters() {
    return nativeGetRtpParameters(mNativeConsumer);
  }

  /**
   * @return Custom data Object provided by the application in the consumer factory method. The app
   *     can modify its content at any time.
   */
  public String getAppData() {
    return nativeGetAppData(mNativeConsumer);
  }

  /**
   * Resumes the consumer Internally the library executes track->set_enabled(true) in the remote
   * track.
   */
  public void resume() {
    nativeResume(mNativeConsumer);
  }

  /**
   * Pauses the consumer. Internally the library executes track->set_enabled(false) in the remote
   * track.
   */
  public void pause() {
    nativePause(mNativeConsumer);
  }

  /**
   * Gets the local RTP receiver statistics by calling getStats() in the underlying RTCRtpReceiver
   * instance.
   *
   * @throws MediasoupException
   */
  public String getStats() throws MediasoupException {
    return nativeGetStats(mNativeConsumer);
  }

  /**
   * Closes the consumer. This method should be called when the server side consumer has been closed
   * (and vice-versa).
   */
  public void close() {
    nativeClose(mNativeConsumer);
  }

  private static native String nativeGetNativeId(long consumer);

  private static native String nativeGetLocalId(long consumer);

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
