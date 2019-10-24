package org.mediasoup.droid;

import org.webrtc.CalledByNative;

public abstract class Transport {

  public String getId() {
    return getNativeId();
  }

  public String getConnectionState() {
    return getNativeConnectionState();
  }

  public String getAppData() {
    return getNativeAppData();
  }

  public String getStats() {
    return getNativeStats();
  }

  public boolean isClosed() {
    return isNativeClosed();
  }

  public void restartIce(String iceParameters) {
    nativeRestartIce(iceParameters);
  }

  public void updateIceServers(String iceServers) {
    nativeUpdateIceServers(iceServers);
  }

  public void close() {
    nativeClose();
  }

  @CalledByNative
  public abstract long getNativeTransport();

  public interface Listener {

    @CalledByNative
    void onConnect(Transport transport, String dtlsParameters);

    @CalledByNative
    void onConnectionStateChange(Transport transport, String connectionState);
  }

  private native String getNativeId();

  private native String getNativeConnectionState();

  private native String getNativeAppData();

  private native String getNativeStats();

  private native boolean isNativeClosed();

  private native void nativeRestartIce(String iceParameters);

  private native void nativeUpdateIceServers(String iceServers);

  private native void nativeClose();
}
