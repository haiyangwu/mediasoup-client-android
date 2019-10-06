package org.mediasoup.droid.sdp;

public class RemoteSdp {

    static native long nativeCreate(long sessionId, boolean mockForTest, String rtpCaps);
}
