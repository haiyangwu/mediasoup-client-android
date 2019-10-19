package org.mediasoup.droid;

import org.mediasoup.droid.hack.Utils;
import org.webrtc.CalledByNative;
import org.webrtc.MediaStreamTrack;
import org.webrtc.RtpParameters;

import java.util.List;

public class SendTransport extends Transport {

  public interface Listener extends Transport.Listener {

    /** @return producer Id */
    String onProduce(Transport transport, String kind, String rtpParameters, String appData);
  }

  private long mNativeTransport;

  @CalledByNative
  public SendTransport(long nativeTransport) {
    mNativeTransport = nativeTransport;
  }

  public void dispose() {
    nativeFreeTransport(mNativeTransport);
    mNativeTransport = 0;
  }

  @CalledByNative
  long getNativeOwnedSendTransport() {
    return mNativeTransport;
  }

  @Override
  public long getNativeTransport() {
    return nativeGetNativeTransport(mNativeTransport);
  }

  public Producer produce(
      Producer.Listener listener,
      MediaStreamTrack track,
      List<RtpParameters> parameters,
      String codecOptions) {
    return produce(listener, track, parameters, codecOptions, null);
  }

  public Producer produce(
      Producer.Listener listener,
      MediaStreamTrack track,
      List<RtpParameters> parameters,
      String codecOptions,
      String appData) {
    long nativeTrack = Utils.getNativeMediaStreamTrack(track);
    return nativeProduce(
        mNativeTransport, listener, nativeTrack, parameters, codecOptions, appData);
  }

  private static native long nativeGetNativeTransport(long nativeTransport);

  private static native Producer nativeProduce(
      long mNativeTransport,
      Producer.Listener listener,
      long track,
      List<RtpParameters> parameters,
      String codecOptions,
      String appData);

  private static native void nativeFreeTransport(long nativeTransport);
}
