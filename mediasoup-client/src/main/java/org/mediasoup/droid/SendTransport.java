package org.mediasoup.droid;

import org.webrtc.CalledByNative;
import org.webrtc.MediaStreamTrack;
import org.webrtc.RTCUtils;
import org.webrtc.RtpParameters;

import java.util.List;

public class SendTransport extends Transport {

  /** This is an abstract class which must be implemented and used according to the API. */
  public interface Listener extends Transport.Listener {

    /**
     * Emitted when the transport needs to transmit information about a new producer to the
     * associated server side transport. This event occurs before the produce() method completes.
     *
     * @param transport SendTransport instance.
     * @param kind Producer's media kind ("audio" or "video").
     * @param rtpParameters Producer's RTP parameters.
     * @param appData Custom application data as given in the transport.produce() method.
     * @return std::future<std::string> ID of the producer created in server side mediasoup
     */
    @CalledByNative("Listener")
    String onProduce(Transport transport, String kind, String rtpParameters, String appData);

    /**
     * Emitted when the transport needs to transmit information about a new data producer to the
     * associated server side transport. This event occurs before the produceData() method
     * completes.
     *
     * @param transport SendTransport instance.
     * @param sctpStreamParameters sctpStreamParameters.
     * @param label A label which can be used to distinguish this DataChannel from others.
     * @param protocol Name of the sub-protocol used by this DataChannel.
     * @param appData Custom application data as given in the transport.produceData() method.
     * @return std::future<std::string> ID of the data producer created in server side mediasoup
     */
    @CalledByNative("Listener")
    String onProduceData(
        Transport transport,
        String sctpStreamParameters,
        String label,
        String protocol,
        String appData);
  }

  private long mNativeTransport;

  @CalledByNative
  public SendTransport(long nativeTransport) {
    mNativeTransport = nativeTransport;
  }

  /** dispose */
  public void dispose() {
    checkTransportExists();
    nativeFreeTransport(mNativeTransport);
    mNativeTransport = 0;
  }

  private void checkTransportExists() {
    if (mNativeTransport == 0) {
      throw new IllegalStateException("SendTransport has been disposed.");
    }
  }

  @Override
  public long getNativeTransport() {
    return nativeGetNativeTransport(mNativeTransport);
  }

  /**
   * Instructs the transport to send an audio or video track to the mediasoup router.
   *
   * @param listener Producer listener.
   * @param track An audio or video track.
   * @param encodings Encoding settings.
   * @param codecOptions Per codec specific options.
   * @param codec codec.
   * @return {@link Producer}
   * @throws MediasoupException
   */
  public Producer produce(
      Producer.Listener listener,
      MediaStreamTrack track,
      List<RtpParameters.Encoding> encodings,
      String codecOptions,
      String codec)
      throws MediasoupException {
    return produce(listener, track, encodings, codecOptions, codec, null);
  }

  /**
   * Instructs the transport to send an audio or video track to the mediasoup router.
   *
   * @param listener Producer listener.
   * @param track An audio or video track.
   * @param encodings Encoding settings.
   * @param codecOptions Per codec specific options.
   * @param codec codec.
   * @param appData Custom application data.
   * @return {@link Producer}
   * @throws MediasoupException
   */
  public Producer produce(
      Producer.Listener listener,
      MediaStreamTrack track,
      List<RtpParameters.Encoding> encodings,
      String codecOptions,
      String codec,
      String appData)
      throws MediasoupException {
    checkTransportExists();
    long nativeTrack = RTCUtils.getNativeMediaStreamTrack(track);
    RtpParameters.Encoding[] pEncodings = null;
    if (encodings != null && !encodings.isEmpty()) {
      pEncodings = new RtpParameters.Encoding[encodings.size()];
      encodings.toArray(pEncodings);
    }
    return nativeProduce(
        mNativeTransport, listener, nativeTrack, pEncodings, codecOptions, codec, appData);
  }

  /**
   * Instructs the transport to send data via DataChannel to the mediasoup router.
   *
   * @param listener DataProducer listener.
   * @return {@link DataProducer}
   */
  public DataProducer produceData(DataProducer.Listener listener) {
    return nativeProduceData(mNativeTransport, listener, null, null, true, 0, 0, null);
  }

  /**
   * Instructs the transport to send data via DataChannel to the mediasoup router.
   *
   * @param listener DataProducer listener.
   * @param label A label which can be used to distinguish this DataChannel from others.
   * @param protocol Name of the sub-protocol used by this DataChannel.
   * @param ordered Whether data messages must be received in order. if true the messages will be
   *     sent reliably.
   * @param maxRetransmits When ordered is false indicates the time (in milliseconds) after which a
   *     SCTP packet will stop being retransmitted.
   * @param maxPacketLifeTime When ordered is false indicates the maximum number of times a packet
   *     will be retransmitted.
   * @param appData Custom application data.
   * @return {@link DataProducer}
   */
  public DataProducer produceData(
      DataProducer.Listener listener,
      String label,
      String protocol,
      boolean ordered,
      int maxRetransmits,
      int maxPacketLifeTime,
      String appData) {
    return nativeProduceData(
        mNativeTransport,
        listener,
        label,
        protocol,
        ordered,
        maxRetransmits,
        maxPacketLifeTime,
        appData);
  }

  private static native long nativeGetNativeTransport(long transport);

  // may throws MediasoupException
  private static native Producer nativeProduce(
      long transport,
      Producer.Listener listener,
      long track,
      RtpParameters.Encoding[] encodings,
      String codecOptions,
      String codec,
      String appData);

  // may throws MediasoupException
  private static native DataProducer nativeProduceData(
      long transport,
      DataProducer.Listener listener,
      String label,
      String protocol,
      boolean ordered,
      int maxRetransmits,
      int maxPacketLifeTime,
      String appData);

  private static native void nativeFreeTransport(long transport);
}
