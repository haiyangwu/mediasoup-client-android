#ifndef MEDIASOUP_CLIENT_PEERCONNECTION_JNI_H
#define MEDIASOUP_CLIENT_PEERCONNECTION_JNI_H

#include "PeerConnection.hpp"
#include "common_jni.h"
#include <jni.h>
#include <sdk/android/src/jni/pc/peer_connection.h>

namespace mediasoupclient
{
class PrivateListenerJni final : public PeerConnection::PrivateListener
{
private:
	webrtc::jni::PeerConnectionObserverJni observerJni;

public:
	PrivateListenerJni(JNIEnv* env, const webrtc::JavaRef<jobject>& j_observer)
	  : observerJni(env, j_observer)
	{
	}

	void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState newState) override
	{
		PrivateListener::OnSignalingChange(newState);
		observerJni.OnSignalingChange(newState);
	}

	void OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override
	{
		PrivateListener::OnAddStream(stream);
		observerJni.OnAddStream(stream);
	}

	void OnRemoveStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override
	{
		PrivateListener::OnRemoveStream(stream);
		observerJni.OnRemoveStream(stream);
	}

	void OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> dataChannel) override
	{
		PrivateListener::OnDataChannel(dataChannel);
		observerJni.OnDataChannel(dataChannel);
	}

	void OnRenegotiationNeeded() override
	{
		PrivateListener::OnRenegotiationNeeded();
		observerJni.OnRenegotiationNeeded();
	}

	void OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState newState) override
	{
		PrivateListener::OnIceConnectionChange(newState);
		observerJni.OnIceConnectionChange(newState);
	}

	void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState newState) override
	{
		PrivateListener::OnIceGatheringChange(newState);
		observerJni.OnIceGatheringChange(newState);
	}

	void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override
	{
		PrivateListener::OnIceCandidate(candidate);
		observerJni.OnIceCandidate(candidate);
	}

	void OnIceCandidatesRemoved(const std::vector<cricket::Candidate>& candidates) override
	{
		PrivateListener::OnIceCandidatesRemoved(candidates);
		observerJni.OnIceCandidatesRemoved(candidates);
	}

	void OnIceConnectionReceivingChange(bool receiving) override
	{
		PrivateListener::OnIceConnectionReceivingChange(receiving);
		observerJni.OnIceConnectionReceivingChange(receiving);
	}

	void OnAddTrack(
	  rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
	  const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>& streams) override
	{
		PrivateListener::OnAddTrack(receiver, streams);
		observerJni.OnAddTrack(receiver, streams);
	}

	void OnTrack(rtc::scoped_refptr<webrtc::RtpTransceiverInterface> transceiver) override
	{
		PrivateListener::OnTrack(transceiver);
		observerJni.OnTrack(transceiver);
	}

	void OnRemoveTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver) override
	{
		PrivateListener::OnRemoveTrack(receiver);
		observerJni.OnRemoveTrack(receiver);
	}

	void OnInterestingUsage(int usagePattern) override
	{
		PrivateListener::OnInterestingUsage(usagePattern);
		observerJni.OnInterestingUsage(usagePattern);
	}
};

// Just like OwnedPeerConnection in "sdk/android/src/jni/pc/peer_connection.h"
//
// PeerConnection doesn't take ownership of the observer. In Java API, we don't
// want the client to have to manually dispose the observer. To solve this, this
// wrapper class is used for object ownership.
//
// Also stores reference to the deprecated PeerConnection constraints for now.
class OwnedPeerConnection
{
public:
	OwnedPeerConnection(PeerConnection* peer_connection, PrivateListenerJni* observer)
	  : peer_connection_(peer_connection), observer_(observer)
	{
	}

	~OwnedPeerConnection()
	{
		delete observer_;
		delete peer_connection_;
	}

	PeerConnection* pc() const
	{
		return peer_connection_;
	}

private:
	PeerConnection* peer_connection_;
	PrivateListenerJni* observer_;
};

} // namespace mediasoupclient
#endif // MEDIASOUP_CLIENT_PEERCONNECTION_JNI_H
