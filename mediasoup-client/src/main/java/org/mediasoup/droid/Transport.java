package org.mediasoup.droid;

import org.webrtc.CalledByNative;

/**
 * A Transport instance in <b>libmediasoupclient</b> represents the local side of a <a
 * href="https://mediasoup.org/documentation/v3/mediasoup/api/#WebRtcTransport">WebRtcTransport</a>
 * in mediasoup server. A WebRTC transport connects a mediasoupclient {@link Device} with a
 * mediasoup <a href="https://mediasoup.org/documentation/v3/mediasoup/api/#Router">Router</a> at
 * media level and enables the sending of media (by means of {@link Producer} instances) or the
 * receiving of media (by means of {@link Consumer} instances).
 *
 * <p>Internally, the transport holds a WebRTC <a
 * href="https://w3c.github.io/webrtc-pc/#dom-rtcpeerconnection">PTCPeerConnection</a> instance.
 */
public abstract class Transport {

  /**
   * Transport identifier. It matches the <b>id</b> of the server side transport.
   *
   * @return const std::string&
   */
  public String getId() {
    return nativeGetId();
  }

  /**
   * The current connection state of the local peerconnection.
   *
   * @return const std::string& <a
   *     href="https://w3c.github.io/webrtc-pc/#rtcpeerconnectionstate-enum">RTCPeerConnectionState</a>
   */
  public String getConnectionState() {
    return nativeGetConnectionState();
  }

  /**
   * Custom data Object provided by the application in the transport constructor. The app can modify
   * its content at any time.
   *
   * @return const nlohmann::json&
   */
  public String getAppData() {
    return nativeGetAppData();
  }

  /**
   * Gets the local transport statistics by calling <b>getStats()</b>in the underlying
   * <b>RTCPeerConnection</b> instance.
   *
   * @return nlohmann::json& <a
   *     href="https://w3c.github.io/webrtc-pc/#dom-rtcstatsreport">RTCStatsReport</a>
   * @throws MediasoupException
   */
  public String getStats() throws MediasoupException {
    return nativeGetStats();
  }

  /**
   * Whether the transport is closed.
   *
   * @return bool
   */
  public boolean isClosed() {
    return nativeIsClosed();
  }

  /**
   * Instructs the underlying peerconnection to restart ICE by providing it with new remote ICE
   * parameters.
   *
   * @param iceParameters New ICE parameters of the server side transport.
   * @throws MediasoupException
   */
  public void restartIce(String iceParameters) throws MediasoupException {
    nativeRestartIce(iceParameters);
  }

  /**
   * Provides the underlying peerconnection with a new list of TURN servers.
   *
   * @param iceServers List of TURN servers to provide the local peerconnection with.
   * @throws MediasoupException
   */
  public void updateIceServers(String iceServers) throws MediasoupException {
    nativeUpdateIceServers(iceServers);
  }

  /** Closes the transport, including all its producers and consumers. */
  public void close() {
    nativeClose();
  }

  @CalledByNative
  public abstract long getNativeTransport();

  public interface Listener {

    /**
     * Called when the transport is about to establish the ICE+DTLS connection and needs to exchange
     * information with the associated server side transport.
     *
     * @param transport Transport instance.
     * @param dtlsParameters Local DTLS parameters.
     */
    @CalledByNative("Listener")
    void onConnect(Transport transport, String dtlsParameters);

    /**
     * Emitted when the local transport connection state changes.
     *
     * @param transport Transport instance.
     * @param connectionState Transport connection state.
     */
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
