package org.mediasoup.droid;

import org.mediasoup.droid.hack.Utils;
import org.webrtc.CalledByNative;
import org.webrtc.MediaStreamTrack;

public class SendTransport extends Transport {

  public interface Listener extends Transport.Listener {

    /** @return producer Id */
    String onProduce(String kind, String rtpParameters, String appData);
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

  @CalledByNative
  long getNativeOwnedSendTransport() {
    return mNativeTransport;
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

  public Producer produce(Producer.Listener listener, MediaStreamTrack track, String codecOptions) {
    return produce(listener, track, codecOptions, null);
  }

  public Producer produce(
      Producer.Listener listener, MediaStreamTrack track, String codecOptions, String appData) {
    checkTransportExists();
    long nativeTrack = Utils.getNativeMediaStreamTrack(track);
    return nativeProduce(mNativeTransport, listener, nativeTrack, codecOptions, appData);
  }

  private static native long nativeGetNativeTransport(long nativeTransport);

  private static native Producer nativeProduce(
      long mNativeTransport,
      Producer.Listener listener,
      long track,
      String codecOptions,
      String appData);

  private static native void nativeFreeTransport(long nativeTransport);
}
