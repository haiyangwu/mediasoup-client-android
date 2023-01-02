package org.mediasoup.droid;

import org.webrtc.CalledByNative;
import org.webrtc.DataChannel;

import java.nio.ByteBuffer;

public class DataConsumer {

  public interface Listener {

    @CalledByNative("DataConsumerListener")
    void OnConnecting(DataConsumer dataConsumer);

    @CalledByNative("DataConsumerListener")
    void OnOpen(DataConsumer dataConsumer);

    @CalledByNative("DataConsumerListener")
    void OnClosing(DataConsumer dataConsumer);

    @CalledByNative("DataConsumerListener")
    void OnClose(DataConsumer dataConsumer);

    @CalledByNative("DataConsumerListener")
    void OnMessage(DataConsumer dataConsumer, DataChannel.Buffer buffer);

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

  public String getId() {
    return nativeGetId(mNativeConsumer);
  }

  public String getLocalId() {
    return nativeGetLocalId(mNativeConsumer);
  }

  public String getDataProducerId() {
    return nativeGetDataProducerId(mNativeConsumer);
  }

  public String getSctpStreamParameters() {
    return nativeGetSctpStreamParameters(mNativeConsumer);
  }

  public DataChannel.State getReadyState() {
    return DataChannel.State.values()[nativeGetReadyState(mNativeConsumer)];
  }

  public String getLabel() {
    return nativeGetLabel(mNativeConsumer);
  }

  public String getProtocol() {
    return nativeGetProtocol(mNativeConsumer);
  }

  public String getAppData() {
    return nativeGetAppData(mNativeConsumer);
  }

  public boolean isClosed() {
    return nativeIsClosed(mNativeConsumer);
  }

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
