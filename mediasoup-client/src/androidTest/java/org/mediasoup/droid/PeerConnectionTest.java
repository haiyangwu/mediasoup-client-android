package org.mediasoup.droid;

import android.content.Context;
import androidx.test.platform.app.InstrumentationRegistry;
import androidx.test.ext.junit.runners.AndroidJUnit4;
import android.text.TextUtils;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.webrtc.MediaConstraints;
import org.webrtc.SessionDescription;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.mediasoup.droid.Utils.exceptionException;

@RunWith(AndroidJUnit4.class)
public class PeerConnectionTest extends BaseTest{

  private PeerConnection.PrivateListener mListener;
  private PeerConnection.Options mPeerConnectionOptions;
  private PeerConnection mPc;

  @Before
  public void setUp() {
    super.setUp();
    mListener = new PeerConnection.PrivateListener();
    mPeerConnectionOptions = new PeerConnection.Options();
    mPc = new PeerConnection(mListener, mPeerConnectionOptions);
  }

  @After
  public void tearDown() {
    mPc.dispose();
  }

  @Test
  public void getConfiguration() {
    assertNotNull("'pc.GetConfiguration()' succeeds", mPc.getConfiguration());
  }

  @Test
  public void setConfiguration() {
    PeerConnection.PrivateListener listener = new PeerConnection.PrivateListener();

    List<org.webrtc.PeerConnection.IceServer> iceServerUris = new ArrayList<>();
    iceServerUris.add(new org.webrtc.PeerConnection.IceServer("Wrong URI"));
    org.webrtc.PeerConnection.RTCConfiguration rtcConfiguration =
        new org.webrtc.PeerConnection.RTCConfiguration(iceServerUris);

    PeerConnection pc = new PeerConnection(listener, mPeerConnectionOptions);
    assertFalse(
        "'pc.SetConfiguration()' fails if wrong options are provided",
        pc.setConfiguration(rtcConfiguration));
    pc.dispose();
  }

  @Test
  public void getStats() {
    String stats = mPc.getStats();
    assertFalse("'pc.GetStats()' succeeds", TextUtils.isEmpty(stats));
  }

  @Test
  public void createAnswer() {
    // 'pc.CreateAnswer()' fails if no remote offer has been provided.
    exceptionException(
        () -> mPc.createAnswer(new MediaConstraints()),
        "PeerConnection cannot create an answer "
            + "in a state other than have-remote-offer or have-local-pranswer.");
  }

  @Test
  public void setLocalDescription() {
    // 'pc.SetRemoteDescription()' fails if incorrect SDP is provided.
    SessionDescription sessionDescription =
        new SessionDescription(SessionDescription.Type.OFFER, "");
    exceptionException(
        () -> mPc.setLocalDescription(sessionDescription),
        "webrtc::CreateSessionDescription failed [:Expect line: v=]");
  }

  @Test
  public void setRemoteDescription() throws Exception {
    // 'pc.SetRemoteDescription()' succeeds if correct SDP is provided.
    Context ctx = InstrumentationRegistry.getInstrumentation().getTargetContext();
    InputStream is = ctx.getResources().getAssets().open("webrtc.sdp");
    String sdp = Utils.readTextStream(is);
    is.close();

    SessionDescription sessionDescription =
        new SessionDescription(SessionDescription.Type.OFFER, sdp);
    mPc.setRemoteDescription(sessionDescription);
  }

  @Test
  public void createOffer() throws MediasoupException {
    // 'pc.CreateOffer()' succeeds.
    String offer = mPc.createOffer(new MediaConstraints());
    assertFalse("'pc.CreateOffer()' succeeds", TextUtils.isEmpty(offer));

    // 'pc.SetRemoteDescription()' succeeds.
    SessionDescription sessionDescription =
        new SessionDescription(SessionDescription.Type.OFFER, offer);
    mPc.setRemoteDescription(sessionDescription);

    // 'pc.CreateAnswer()' succeeds if remote offer is provided.
    String answer = mPc.createAnswer(new MediaConstraints());
    assertFalse(
        "'pc.CreateAnswer()' succeeds if remote offer is provided", TextUtils.isEmpty(answer));
  }
}
