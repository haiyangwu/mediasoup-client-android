package org.mediasoup.droid;

import org.webrtc.CalledByNative;
import org.webrtc.MediaStreamTrack;
import org.webrtc.RTCUtils;
import org.webrtc.RtpParameters;

import java.util.List;

public class SendTransport extends Transport {

  public interface Listener extends Transport.Listener {

    /** @return producer Id */
    @CalledByNative("Listener")
    String onProduce(Transport transport, String kind, String rtpParameters, String appData);
  }

  private long mNativeTransport;

  @CalledByNative
  public SendTransport(long nativeTransport) {
    mNativeTransport = nativeTransport;
  }

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

  public Producer produce(
      Producer.Listener listener,
      MediaStreamTrack track,
      List<RtpParameters.Encoding> encodings,
      String codecOptions)
      throws MediasoupException {
    return produce(listener, track, encodings, codecOptions, null);
  }

  public Producer produce(
      Producer.Listener listener,
      MediaStreamTrack track,
      List<RtpParameters.Encoding> encodings,
      String codecOptions,
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
        mNativeTransport, listener, nativeTrack, pEncodings, codecOptions, appData);
  }

  private static native long nativeGetNativeTransport(long transport);

  // may throws MediasoupException
  private static native Producer nativeProduce(
      long transport,
      Producer.Listener listener,
      long track,
      RtpParameters.Encoding[] encodings,
      String codecOptions,
      String appData);

  private static native void nativeFreeTransport(long transport);
}
