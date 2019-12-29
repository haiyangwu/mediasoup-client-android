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

  public String getStats() throws MediasoupException {
    return getNativeStats();
  }

  public boolean isClosed() {
    return isNativeClosed();
  }

  public void restartIce(String iceParameters) throws MediasoupException {
    nativeRestartIce(iceParameters);
  }

  public void updateIceServers(String iceServers) throws MediasoupException {
    nativeUpdateIceServers(iceServers);
  }

  public void close() {
    nativeClose();
  }

  @CalledByNative
  public abstract long getNativeTransport();

  public interface Listener {

    @CalledByNative("Listener")
    void onConnect(Transport transport, String dtlsParameters);

    @CalledByNative("Listener")
    void onConnectionStateChange(Transport transport, String connectionState);
  }

  private native String getNativeId();

  private native String getNativeConnectionState();

  private native String getNativeAppData();

  private native String getNativeStats() throws MediasoupException;

  private native boolean isNativeClosed();

  private native void nativeRestartIce(String iceParameters) throws MediasoupException;

  private native void nativeUpdateIceServers(String iceServers) throws MediasoupException;

  private native void nativeClose();
}
