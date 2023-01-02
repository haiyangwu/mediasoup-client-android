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

  /**
   * Instructs the transport to receive an audio or video track to the mediasoup router.
   *
   * @param listener Consumer listener.
   * @param id The identifier of the server side consumer.
   * @param producerId The identifier of the server side producer being consumed.
   * @param kind Media kind (“audio” or “video”).
   * @param rtpParameters Receive RTP parameters.
   * @return {@link Consumer}
   * @throws MediasoupException
   */
  public Consumer consume(
      Consumer.Listener listener, String id, String producerId, String kind, String rtpParameters)
      throws MediasoupException {
    return consume(listener, id, producerId, kind, rtpParameters, null);
  }

  /**
   * Instructs the transport to receive an audio or video track to the mediasoup router.
   *
   * @param listener Consumer listener.
   * @param id The identifier of the server side consumer.
   * @param producerId The identifier of the server side producer being consumed.
   * @param kind Media kind (“audio” or “video”).
   * @param rtpParameters Receive RTP parameters.
   * @param appData Custom application data.
   * @return {@link Consumer}
   * @throws MediasoupException
   */
  public Consumer consume(
      Consumer.Listener listener,
      String id,
      String producerId,
      String kind,
      String rtpParameters,
      String appData)
      throws MediasoupException {
    checkTransportExists();
    return nativeConsume(mNativeTransport, listener, id, producerId, kind, rtpParameters, appData);
  }

  /**
   * Instructs the transport to receive data via DataChannel from the mediasoup router.
   *
   * @param listener Consumer listener.
   * @param id The identifier of the server side consumer.
   * @param producerId The identifier of the server side producer being consumed.
   * @param streamId streamId.
   * @param label A label which can be used to distinguish this DataChannel from others.
   * @return {@link DataConsumer}
   */
  public DataConsumer consumeData(
      DataConsumer.Listener listener, String id, String producerId, long streamId, String label) {
    return consumeData(listener, id, producerId, streamId, label, null, null);
  }

  /**
   * Instructs the transport to receive data via DataChannel from the mediasoup router.
   *
   * @param listener Consumer listener.
   * @param id The identifier of the server side consumer.
   * @param producerId The identifier of the server side producer being consumed.
   * @param streamId streamId.
   * @param label A label which can be used to distinguish this DataChannel from others.
   * @param protocol Name of the sub-protocol used by this DataChannel.
   * @param appData Custom application data.
   * @return {@link DataConsumer}
   */
  public DataConsumer consumeData(
      DataConsumer.Listener listener,
      String id,
      String producerId,
      long streamId,
      String label,
      String protocol,
      String appData) {
    checkTransportExists();
    return nativeConsumeData(
        mNativeTransport, listener, id, producerId, streamId, label, protocol, appData);
  }

  private static native long nativeGetNativeTransport(long transport);

  // may throws MediasoupException
  private static native Consumer nativeConsume(
      long transport,
      Consumer.Listener listener,
      String id,
      String producerId,
      String kind,
      String rtpParameters,
      String appData);

  // may throws MediasoupException
  private static native DataConsumer nativeConsumeData(
      long transport,
      DataConsumer.Listener listener,
      String id,
      String producerId,
      long streamId,
      String label,
      String protocol,
      String appData);

  private static native void nativeFreeTransport(long transport);
}
