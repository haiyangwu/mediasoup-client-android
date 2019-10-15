package org.mediasoup.droid;

import org.mediasoup.droid.hack.Utils;
import org.webrtc.MediaStreamTrack;
import org.webrtc.RtpParameters;

import java.util.List;

public class Transport {

  protected long mNativeTransport;

  protected Transport(long mNativeTransport) {
    this.mNativeTransport = mNativeTransport;
  }

  public String getId() {
    return getNativeId(mNativeTransport);
  }

  public String getConnectionState() {
    return getNativeConnectionState(mNativeTransport);
  }

  public String getAppData() {
    return getNativeAppData(mNativeTransport);
  }

  public String getStats() {
    return getNativeStats(mNativeTransport);
  }

  public boolean isClosed() {
    return isNativeClosed(mNativeTransport);
  }

  public void restartIce(String iceParameters) {
    nativeRestartIce(mNativeTransport, iceParameters);
  }

  public void updateIceServers(String iceServers) {
    nativeUpdateIceServers(mNativeTransport, iceServers);
  }

  public void close() {
    nativeClose(mNativeTransport);
  }

  public interface Listener {

    void onConnect(Transport transport, String dtlsParameters);

    void OnConnectionStateChange(Transport transport, String connectionState);
  }

  public static class SendTransport extends Transport {

    public interface Listener extends Transport.Listener {}

    public SendTransport(long mNativeTransport) {
      super(mNativeTransport);
    }

    Producer produce(
        Producer.Listener listener,
        MediaStreamTrack track,
        List<RtpParameters> parameters,
        String codecOptions) {
      return produce(listener, track, parameters, codecOptions, null);
    }

    Producer produce(
        Producer.Listener listener,
        MediaStreamTrack track,
        List<RtpParameters> parameters,
        String codecOptions,
        String appData) {
      return nativeProduce(
          mNativeTransport,
          listener,
          Utils.getNativeMediaStreamTrack(track),
          parameters,
          codecOptions,
          appData);
    }
  }

  public static class RecvTransport extends Transport {

    public interface Listener extends Transport.Listener {}

    public RecvTransport(long mNativeTransport) {
      super(mNativeTransport);
    }
  }

  private native String getNativeId(long nativeTransport);

  private native String getNativeConnectionState(long nativeTransport);

  private native String getNativeAppData(long nativeTransport);

  private native String getNativeStats(long nativeTransport);

  private native boolean isNativeClosed(long nativeTransport);

  private native void nativeRestartIce(long nativeTransport, String iceParameters);

  private native void nativeUpdateIceServers(long nativeTransport, String iceServers);

  private native void nativeClose(long nativeTransport);

  private static native Producer nativeProduce(
      long mNativeTransport,
      Producer.Listener listener,
      long track,
      List<RtpParameters> parameters,
      String codecOptions,
      String appData);
}
