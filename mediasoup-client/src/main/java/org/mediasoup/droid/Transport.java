package org.mediasoup.droid;

import org.webrtc.CalledByNative;

public abstract class Transport {

  public String getId() {
    return nativeGetId();
  }

  public String getConnectionState() {
    return nativeGetConnectionState();
  }

  public String getAppData() {
    return nativeGetAppData();
  }

  public String getStats() throws MediasoupException {
    return nativeGetStats();
  }

  public boolean isClosed() {
    return nativeIsClosed();
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

  private native String nativeGetId();

  private native String nativeGetConnectionState();

  private native String nativeGetAppData();

  // may throws MediasoupException;
  private native String nativeGetStats();

  private native boolean nativeIsClosed();

  // may throws MediasoupException;
  private native void nativeRestartIce(String iceParameters);

  // may throws MediasoupException;
  private native void nativeUpdateIceServers(String iceServers);

  private native void nativeClose();
}
