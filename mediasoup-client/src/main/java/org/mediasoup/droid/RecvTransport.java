package org.mediasoup.droid;

import org.webrtc.CalledByNative;

public class RecvTransport extends Transport {

  public interface Listener extends Transport.Listener {}

  private long mNativeTransport;

  @CalledByNative
  public RecvTransport(long nativeTransport) {
    mNativeTransport = nativeTransport;
  }

  public void dispose() {
    checkTransportExists();
    nativeFreeTransport(mNativeTransport);
    mNativeTransport = 0;
  }

  @Override
  public long getNativeTransport() {
    return nativeGetNativeTransport(mNativeTransport);
  }

  private void checkTransportExists() {
    if (mNativeTransport == 0) {
      throw new IllegalStateException("RecvTransport has been disposed.");
    }
  }

  public Consumer consume(
      Consumer.Listener listener, String id, String producerId, String kind, String rtpParameters) {
    return consume(listener, id, producerId, kind, rtpParameters, null);
  }

  public Consumer consume(
      Consumer.Listener listener,
      String id,
      String producerId,
      String kind,
      String rtpParameters,
      String appData) {
    checkTransportExists();
    return nativeConsume(mNativeTransport, listener, id, producerId, kind, rtpParameters, appData);
  }

  private static native long nativeGetNativeTransport(long nativeTransport);

  private static native Consumer nativeConsume(
      long mNativeTransport,
      Consumer.Listener listener,
      String id,
      String producerId,
      String kind,
      String rtpParameters,
      String appData);

  private static native void nativeFreeTransport(long nativeTransport);
}
