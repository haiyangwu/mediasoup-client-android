package org.mediasoup.droid;

import android.content.Context;

import org.webrtc.MediaConstraints;

public class Handler {

    private static final String TAG = "Handler";

//    public static String getNativeRtpCapabilities(Context context) {
//        Logger.t(TAG).d("getNativeRtpCapabilities()");
//
//        PeerConnection.PrivateListener privateListener = new PeerConnection.PrivateListener();
//        PeerConnection peerConnection = new PeerConnection(context, privateListener, null);
//
//        // Create SDP constraints.
//        MediaConstraints sdpMediaConstraints = new MediaConstraints();
//        sdpMediaConstraints.mandatory.add(new MediaConstraints.KeyValuePair("OfferToReceiveAudio", "true"));
//        sdpMediaConstraints.mandatory.add(new MediaConstraints.KeyValuePair("OfferToReceiveVideo", "true"));
//
//        String offer = peerConnection.createOffer(sdpMediaConstraints);
//
//        Logger.t(TAG).d("offer result %s", offer);
//        return "";
//    }
}
