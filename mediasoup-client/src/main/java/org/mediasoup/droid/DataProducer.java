package org.mediasoup.droid;

import org.webrtc.CalledByNative;
import org.webrtc.DataChannel;

public class DataProducer {

  public interface Listener {

    /**
     * Executed when the underlying DataChannel is open.
     *
     * @param dataProducer The producer instance executing this method.
     */
    @CalledByNative("DataProducerListener")
    void onOpen(DataProducer dataProducer);

    /**
     * Executed when the underlying DataChannel is closed for unknown reasons.
     *
     * @param dataProducer The producer instance executing this method.
     */
    @CalledByNative("DataProducerListener")
    void onClose(DataProducer dataProducer);

    /**
     * Executed when the DataChannel buffered amount of bytes changes.
     *
     * @param dataProducer The producer instance executing this method.
     * @param sentDataSize The amount of data sent.
     */
    @CalledByNative("DataProducerListener")
    void onBufferedAmountChange(DataProducer dataProducer, long sentDataSize);

    /**
     * Executed when the transport this producer belongs to is closed for whatever reason. The
     * producer itself is also closed.
     *
     * @param dataProducer The producer instance executing this method.
     */
    @CalledByNative("DataProducerListener")
    void onTransportClose(DataProducer dataProducer);
  }

  private final long mNativeProducer;

  @CalledByNative
  public DataProducer(long nativeProducer) {
    mNativeProducer = nativeProducer;
  }

  /**
   * @return Producer identifier.
   */
  public String getId() {
    return nativeGetId(mNativeProducer);
  }

  /**
   * @return Producer local identifier.
   */
  public String getLocalId() {
    return nativeGetLocalId(mNativeProducer);
  }

  /**
   * @return The SCTP stream parameters.
   */
  public String getSctpStreamParameters() {
    return nativeGetSctpStreamParameters(mNativeProducer);
  }

  /**
   * @return The DataChannel ready state.
   */
  public DataChannel.State getReadyState() {
    return DataChannel.State.values()[nativeGetReadyState(mNativeProducer)];
  }

  /**
   * @return The DataChannel label.
   */
  public String getLabel() {
    return nativeGetLabel(mNativeProducer);
  }

  /**
   * @return The DataChannel sub-protocol.
   */
  public String getProtocol() {
    return nativeGetProtocol(mNativeProducer);
  }

  /**
   * @return The number of bytes of application data (UTF-8 text and binary data) that have been
   *     queued using send().
   */
  public long getBufferedAmount() {
    return nativeGetBufferedAmount(mNativeProducer);
  }

  /**
   * @return Custom data Object provided by the application in the data producer factory method. The
   *     app can modify its content at any time.
   */
  public String getAppData() {
    return nativeGetAppData(mNativeProducer);
  }

  /**
   * @return Whether the data producer is closed.
   */
  public boolean isClosed() {
    return nativeIsClosed(mNativeProducer);
  }

  /** Closes the data producer. No more data is transmitted. */
  public void close() {
    nativeClose(mNativeProducer);
  }

  /**
   * Sends the given data over the corresponding DataChannel. If the data can't be sent at the SCTP
   * level (due to congestion control), it's buffered at the data channel level, up to a maximum of
   * 16MB. If Send is called while this buffer is full, the data channel will be closed abruptly.
   *
   * @param buffer Data message to be sent.
   */
  public void send(DataChannel.Buffer buffer) {
    byte[] data = new byte[buffer.data.remaining()];
    buffer.data.get(data);
    nativeSend(mNativeProducer, data, buffer.binary);
  }

  private static native String nativeGetId(long producer);

  private static native String nativeGetLocalId(long producer);

  private static native String nativeGetSctpStreamParameters(long producer);

  private static native int nativeGetReadyState(long producer);

  private static native String nativeGetLabel(long producer);

  private static native String nativeGetProtocol(long producer);

  private static native long nativeGetBufferedAmount(long producer);

  private static native String nativeGetAppData(long producer);

  private static native boolean nativeIsClosed(long producer);

  private static native void nativeClose(long producer);

  private static native void nativeSend(long producer, byte[] data, boolean binary);
}
