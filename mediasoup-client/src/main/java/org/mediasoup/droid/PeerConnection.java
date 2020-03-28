package org.mediasoup.droid;

import org.webrtc.CalledByNative;
import org.webrtc.DataChannel;
import org.webrtc.IceCandidate;
import org.webrtc.MediaConstraints;
import org.webrtc.MediaStream;
import org.webrtc.MediaStreamTrack;
import org.webrtc.PeerConnection.RTCConfiguration;
import org.webrtc.PeerConnectionFactory;
import org.webrtc.RTCUtils;
import org.webrtc.RtpReceiver;
import org.webrtc.RtpSender;
import org.webrtc.RtpTransceiver;
import org.webrtc.SessionDescription;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class PeerConnection {

  public static class Options {

    public RTCConfiguration mRTCConfig;

    public PeerConnectionFactory mFactory;

    public void setRTCConfig(RTCConfiguration RTCConfig) {
      mRTCConfig = RTCConfig;
    }

    public void setFactory(PeerConnectionFactory factory) {
      mFactory = factory;
    }
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
    public void onAddStream(MediaStream mediaStream) {}

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
  private long mNativePeerConnection;
  private List<RtpSender> mSenders = new ArrayList<>();
  private List<RtpTransceiver> mTransceivers = new ArrayList<>();

  public PeerConnection(PrivateListener listener, Options options) {
    if (listener == null) {
      throw new IllegalArgumentException("given listener is null");
    }

    mOptions = new Options();
    if (options != null && options.mRTCConfig != null) {
      mOptions.mRTCConfig = options.mRTCConfig;
    } else {
      mOptions.mRTCConfig = new RTCConfiguration(new ArrayList<>());
    }
    if (options != null && options.mFactory != null) {
      mOptions.mFactory = options.mFactory;
    }

    mNativePeerConnection =
        nativeNewPeerConnection(
            listener,
            mOptions.mRTCConfig,
            mOptions.mFactory != null ? mOptions.mFactory.getNativePeerConnectionFactory() : 0);
  }

  public void dispose() {
    close();
    for (RtpSender sender : mSenders) {
      sender.dispose();
    }
    mSenders.clear();
    for (RtpTransceiver transceiver : mTransceivers) {
      transceiver.dispose();
    }
    mTransceivers.clear();
    nativeFreeOwnedPeerConnection(mNativePeerConnection);
    mNativePeerConnection = 0;
  }

  public RTCConfiguration getConfiguration() {
    return mOptions.mRTCConfig;
  }

  public boolean setConfiguration(RTCConfiguration config) {
    return nativeSetConfiguration(config);
  }

  public String createOffer(MediaConstraints constraints) {
    return nativeCreateOffer(constraints);
  }

  public String createAnswer(MediaConstraints constraints) throws MediasoupException {
    return nativeCreateAnswer(constraints);
  }

  public void setLocalDescription(SessionDescription sessionDescription) throws MediasoupException {
    if (sessionDescription == null) {
      throw new IllegalArgumentException("given sessionDescription is null");
    }

    nativeSetLocalDescription(
        sessionDescription.type.canonicalForm(), sessionDescription.description);
  }

  public void setRemoteDescription(SessionDescription sessionDescription)
      throws MediasoupException {
    if (sessionDescription == null) {
      throw new IllegalArgumentException("given sessionDescription is null");
    }

    nativeSetRemoteDescription(
        sessionDescription.type.canonicalForm(), sessionDescription.description);
  }

  public String getLocalDescription() {
    return nativeGetLocalDescription();
  }

  public String getRemoteDescription() {
    return nativeGetRemoteDescription();
  }

  public List<RtpTransceiver> getTransceivers() {
    for (RtpTransceiver transceiver : mTransceivers) {
      transceiver.dispose();
    }
    mTransceivers = nativeGetTransceivers();
    return Collections.unmodifiableList(mTransceivers);
  }

  public RtpTransceiver addTransceiver(MediaStreamTrack.MediaType mediaType) {
    if (mediaType == null) {
      throw new NullPointerException("No MediaType specified for addTransceiver.");
    }
    RtpTransceiver newTransceiver = nativeAddTransceiverOfType(mediaType);
    if (newTransceiver == null) {
      throw new IllegalStateException("C++ addTransceiver failed.");
    }
    mTransceivers.add(newTransceiver);
    return newTransceiver;
  }

  public RtpTransceiver addTransceiver(MediaStreamTrack track) {
    if (track == null) {
      throw new NullPointerException("No MediaStreamTrack specified for addTransceiver.");
    }
    RtpTransceiver newTransceiver =
        nativeAddTransceiverWithTrack(RTCUtils.getNativeMediaStreamTrack(track));
    if (newTransceiver == null) {
      throw new IllegalStateException("C++ addTransceiver failed.");
    }
    mTransceivers.add(newTransceiver);
    return newTransceiver;
  }

  public void close() {
    nativeClose();
  }

  public List<RtpSender> getSenders() {
    for (RtpSender sender : mSenders) {
      sender.dispose();
    }
    mSenders = nativeGetSenders();
    return Collections.unmodifiableList(mSenders);
  }

  public boolean removeTrack(RtpSender sender) {
    if (sender == null) {
      throw new NullPointerException("No RtpSender specified for removeTrack.");
    }
    return nativeRemoveTrack(RTCUtils.getNativeRtpSender(sender));
  }

  public String getStats() {
    return nativeGetStats();
  }

  public String getStats(RtpSender selector) {
    return nativeGetStatsForRtpSender(RTCUtils.getNativeRtpSender(selector));
  }

  public String getStats(RtpReceiver selector) {
    return nativeGetStatsForRtpReceiver(RTCUtils.getNativeRtpReceiver(selector));
  }

  /** Returns a pointer to the native webrtc::PeerConnectionInterface. */
  public long getNativePeerConnection() {
    return nativeGetNativePeerConnection();
  }

  @CalledByNative
  long getNativeOwnedPeerConnection() {
    return mNativePeerConnection;
  }

  private static native long nativeNewPeerConnection(
      PrivateListener nativeListener, RTCConfiguration configuration, long peerConnectionFactory);

  private static native void nativeFreeOwnedPeerConnection(long ownedPeerConnection);

  private native long nativeGetNativePeerConnection();

  private native boolean nativeSetConfiguration(RTCConfiguration rtcConfig);

  private native String nativeCreateOffer(MediaConstraints constraints);

  // may throws MediasoupException;
  private native String nativeCreateAnswer(MediaConstraints constraints);

  // may throws MediasoupException;
  private native void nativeSetLocalDescription(String type, String description);

  // may throws MediasoupException;
  private native void nativeSetRemoteDescription(String type, String description);

  private native String nativeGetLocalDescription();

  private native String nativeGetRemoteDescription();

  private native List<RtpSender> nativeGetSenders();

  private native List<RtpTransceiver> nativeGetTransceivers();

  private native boolean nativeRemoveTrack(long sender);

  private native RtpTransceiver nativeAddTransceiverWithTrack(long track);

  private native RtpTransceiver nativeAddTransceiverOfType(MediaStreamTrack.MediaType mediaType);

  private native void nativeClose();

  private native String nativeGetStats();

  private native String nativeGetStatsForRtpSender(long selector);

  private native String nativeGetStatsForRtpReceiver(long selector);
}
