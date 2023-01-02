package org.mediasoup.droid;

import org.webrtc.CalledByNative;
import org.webrtc.DataChannel;

public class DataProducer {

  public interface Listener {

    @CalledByNative("DataProducerListener")
    void onOpen(DataProducer dataProducer);

    @CalledByNative("DataProducerListener")
    void onClose(DataProducer dataProducer);

    @CalledByNative("DataProducerListener")
    void onBufferedAmountChange(DataProducer dataProducer, long sentDataSize);

    @CalledByNative("DataProducerListener")
    void onTransportClose(DataProducer dataProducer);
  }

  private final long mNativeProducer;

  @CalledByNative
  public DataProducer(long nativeProducer) {
    mNativeProducer = nativeProducer;
  }

  public String getId() {
    return nativeGetId(mNativeProducer);
  }

  public String getLocalId() {
    return nativeGetLocalId(mNativeProducer);
  }

  public String getSctpStreamParameters() {
    return nativeGetSctpStreamParameters(mNativeProducer);
  }

  public DataChannel.State getReadyState() {
    return DataChannel.State.values()[nativeGetReadyState(mNativeProducer)];
  }

  public String getLabel() {
    return nativeGetLabel(mNativeProducer);
  }

  public String getProtocol() {
    return nativeGetProtocol(mNativeProducer);
  }

  public long getBufferedAmount() {
    return nativeGetBufferedAmount(mNativeProducer);
  }

  public String getAppData() {
    return nativeGetAppData(mNativeProducer);
  }

  public boolean isClosed() {
    return nativeIsClosed(mNativeProducer);
  }

  public void close() {
    nativeClose(mNativeProducer);
  }

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
