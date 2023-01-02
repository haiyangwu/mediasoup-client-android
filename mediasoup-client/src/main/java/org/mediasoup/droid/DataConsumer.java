package org.mediasoup.droid;

import org.webrtc.CalledByNative;
import org.webrtc.DataChannel;

import java.nio.ByteBuffer;

public class DataConsumer {

  public interface Listener {

    /**
     * Executed when the underlying DataChannel is connecting.
     *
     * @param dataConsumer The data consumer instance executing this method.
     */
    @CalledByNative("DataConsumerListener")
    void OnConnecting(DataConsumer dataConsumer);

    /**
     * Executed when the underlying DataChannel is open.
     *
     * @param dataConsumer The data consumer instance executing this method.
     */
    @CalledByNative("DataConsumerListener")
    void OnOpen(DataConsumer dataConsumer);

    /**
     * Executed when the underlying DataChannel is closing.
     *
     * @param dataConsumer The data consumer instance executing this method.
     */
    @CalledByNative("DataConsumerListener")
    void OnClosing(DataConsumer dataConsumer);

    /**
     * Executed when the underlying DataChannel is closed for unknown reasons.
     *
     * @param dataConsumer The data consumer instance executing this method.
     */
    @CalledByNative("DataConsumerListener")
    void OnClose(DataConsumer dataConsumer);

    /**
     * Executed when a DataChannel message is received.
     *
     * @param dataConsumer The data consumer instance executing this method.
     * @param buffer Data message received.
     */
    @CalledByNative("DataConsumerListener")
    void OnMessage(DataConsumer dataConsumer, DataChannel.Buffer buffer);

    /**
     * Executed when the transport this consumer belongs to is closed for whatever reason. The
     * consumer itself is also closed.
     *
     * @param dataConsumer The data consumer instance executing this method.
     */
    @CalledByNative("DataConsumerListener")
    void OnTransportClose(DataConsumer dataConsumer);
  }

  @CalledByNative
  public static DataChannel.Buffer generateBuffer(ByteBuffer data, boolean binary) {
    return new DataChannel.Buffer(data, binary);
  }

  private final long mNativeConsumer;

  @CalledByNative
  public DataConsumer(long nativeConsumer) {
    mNativeConsumer = nativeConsumer;
  }

  /**
   * @return Consumer identifier.
   */
  public String getId() {
    return nativeGetId(mNativeConsumer);
  }

  /**
   * @return Consumer local identifier.
   */
  public String getLocalId() {
    return nativeGetLocalId(mNativeConsumer);
  }

  /**
   * @return The associated data producer identifier.
   */
  public String getDataProducerId() {
    return nativeGetDataProducerId(mNativeConsumer);
  }

  /**
   * @return The SCTP stream parameters.
   */
  public String getSctpStreamParameters() {
    return nativeGetSctpStreamParameters(mNativeConsumer);
  }

  /**
   * @return The DataChannel ready state.
   */
  public DataChannel.State getReadyState() {
    return DataChannel.State.values()[nativeGetReadyState(mNativeConsumer)];
  }

  /**
   * @return The DataChannel label.
   */
  public String getLabel() {
    return nativeGetLabel(mNativeConsumer);
  }

  /**
   * @return The DataChannel sub-protocol.
   */
  public String getProtocol() {
    return nativeGetProtocol(mNativeConsumer);
  }

  /**
   * @return Custom data Object provided by the application in the consumer factory method. The app
   *     can modify its content at any time.
   */
  public String getAppData() {
    return nativeGetAppData(mNativeConsumer);
  }

  /**
   * @return Whether the consumer is closed.
   */
  public boolean isClosed() {
    return nativeIsClosed(mNativeConsumer);
  }

  /** Closes the dataConsumer. */
  public void close() {
    nativeClose(mNativeConsumer);
  }

  private static native String nativeGetId(long consumer);

  private static native String nativeGetDataProducerId(long consumer);

  private static native String nativeGetLocalId(long consumer);

  private static native String nativeGetSctpStreamParameters(long consumer);

  private static native int nativeGetReadyState(long consumer);

  private static native String nativeGetLabel(long consumer);

  private static native String nativeGetProtocol(long consumer);

  private static native String nativeGetAppData(long consumer);

  private static native boolean nativeIsClosed(long consumer);

  private static native void nativeClose(long consumer);
}
