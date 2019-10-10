package org.mediasoup.droid;

import org.webrtc.DataChannel;
import org.webrtc.IceCandidate;
import org.webrtc.MediaConstraints;
import org.webrtc.MediaStream;
import org.webrtc.MediaStreamTrack;
import org.webrtc.PeerConnection.RTCConfiguration;
import org.webrtc.PeerConnectionFactory;
import org.webrtc.RtpReceiver;
import org.webrtc.RtpSender;
import org.webrtc.RtpTransceiver;
import org.webrtc.SessionDescription;

import java.util.ArrayList;
import java.util.List;

public class PeerConnection {

  public static class Options {
    public org.webrtc.PeerConnection.RTCConfiguration mRTCConfig;
    public PeerConnectionFactory mFactory;
  }

  public static class PrivateListener implements org.webrtc.PeerConnection.Observer {

    @Override
    public void onSignalingChange(org.webrtc.PeerConnection.SignalingState signalingState) {}

    @Override
    public void onIceConnectionChange(
        org.webrtc.PeerConnection.IceConnectionState iceConnectionState) {}

    @Override
    public void onIceConnectionReceivingChange(boolean b) {}

    @Override
    public void onIceGatheringChange(
        org.webrtc.PeerConnection.IceGatheringState iceGatheringState) {}

    @Override
    public void onIceCandidate(IceCandidate iceCandidate) {}

    @Override
    public void onIceCandidatesRemoved(IceCandidate[] iceCandidates) {}

    @Override
    public void onAddStream(MediaStream mediaStream) {
      Logger.d("TONY", "xxxx: " + mediaStream.getId());
    }

    @Override
    public void onRemoveStream(MediaStream mediaStream) {}

    @Override
    public void onDataChannel(DataChannel dataChannel) {}

    @Override
    public void onRenegotiationNeeded() {}

    @Override
    public void onAddTrack(RtpReceiver rtpReceiver, MediaStream[] mediaStreams) {}

    @Override
    public void onConnectionChange(org.webrtc.PeerConnection.PeerConnectionState newState) {}

    @Override
    public void onTrack(RtpTransceiver transceiver) {}
  }

  private final Options mOptions;
  private final PrivateListener mListener;
  private long mNativeListener;
  private long mNativePeerConnection;

  public PeerConnection(PrivateListener listener, Options options) {
    if (listener == null) {
      throw new IllegalArgumentException("given listener is null");
    }

    mOptions = new Options();
    mListener = listener;

    if (options != null && options.mRTCConfig != null) {
      mOptions.mRTCConfig = options.mRTCConfig;
    } else {
      mOptions.mRTCConfig = new RTCConfiguration(new ArrayList<>());
    }

    long nativePeerConnectionFactory = 0;
    if (options != null && options.mFactory != null) {
      mOptions.mFactory = options.mFactory;
      nativePeerConnectionFactory = options.mFactory.getNativePeerConnectionFactory();
    }

    mNativeListener = nativeNewListener(mListener);
    mNativePeerConnection =
        nativeNewPeerConnection(mNativeListener, mOptions.mRTCConfig, nativePeerConnectionFactory);
  }

  public void dispose() {
    nativeFreePeerConnection(mNativePeerConnection);
    mNativePeerConnection = 0;
    nativeFreeListener(mNativeListener);
    mNativeListener = 0;
  }

  public RTCConfiguration getConfiguration() {
    return mOptions.mRTCConfig;
  }

  public boolean setConfiguration(RTCConfiguration config) {
    return nativeSetConfiguration(mNativePeerConnection, config);
  }

  public String createOffer(MediaConstraints constraints) {
    return nativeCreateOffer(mNativePeerConnection, constraints);
  }

  public String createAnswer(MediaConstraints constraints) {
    return nativeCreateAnswer(mNativePeerConnection, constraints);
  }

  public void setLocalDescription(SessionDescription sessionDescription) {
    if (sessionDescription == null) {
      throw new IllegalArgumentException("given sessionDescription is null");
    }

    nativeSetLocalDescription(
        mNativePeerConnection,
        sessionDescription.type.canonicalForm(),
        sessionDescription.description);
  }

  public void setRemoteDescription(SessionDescription sessionDescription) {
    if (sessionDescription == null) {
      throw new IllegalArgumentException("given sessionDescription is null");
    }

    nativeSetRemoteDescription(
        mNativePeerConnection,
        sessionDescription.type.canonicalForm(),
        sessionDescription.description);
  }

  public String getLocalDescription() {
    // TODO:
    return null;
  }

  public String getRemoteDescription() {
    // TODO:
    return null;
  }

  public List<RtpTransceiver> getTransceivers() {
    // TODO:
    return null;
  }

  public RtpTransceiver addTransceiver(MediaStreamTrack.MediaType mediaType) {
    // TODO:
    return null;
  }

  public RtpTransceiver addTransceiver(MediaStreamTrack track) {
    // TODO:
    return null;
  }

  public void close() {
    // TODO:
  }

  public List<RtpSender> getSenders() {
    // TODO:
    return null;
  }

  public boolean removeTrack(RtpSender sender) {
    // TODO:
    return false;
  }

  public String getStats() {
    return nativeGetStats(mNativePeerConnection);
  }

  public String getStats(RtpSender selector) {
    // TODO:
    return null;
  }

  public String getStats(RtpReceiver selector) {
    // TODO:
    return null;
  }

  private static native long nativeNewListener(PrivateListener listener);

  private static native long nativeFreeListener(long nativeListener);

  private static native long nativeNewPeerConnection(
      long nativeListener, RTCConfiguration rtcConfig, long nativePeerConnectionFactory);

  private static native void nativeFreePeerConnection(long nativePeerConnection);

  private native boolean nativeSetConfiguration(
      long nativePeerConnection, RTCConfiguration rtcConfig);

  private native String nativeCreateOffer(long nativePeerConnection, MediaConstraints constraints);

  private native String nativeCreateAnswer(long nativePeerConnection, MediaConstraints constraints);

  private native void nativeSetLocalDescription(
      long nativePeerConnection, String type, String description);

  private native void nativeSetRemoteDescription(
      long nativePeerConnection, String type, String description);

  private native String nativeGetStats(long nativePeerConnection);
}
