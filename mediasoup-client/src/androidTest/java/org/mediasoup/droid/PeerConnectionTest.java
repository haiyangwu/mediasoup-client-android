package org.mediasoup.droid;

import android.content.Context;
import android.support.test.InstrumentationRegistry;
import android.support.test.runner.AndroidJUnit4;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;

import java.util.ArrayList;
import java.util.List;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;

@RunWith(AndroidJUnit4.class)
public class PeerConnectionTest {

  private PeerConnection.PrivateListener mListener;
  private PeerConnection.Options mPeerConnectionOptions;
  private PeerConnection mPc;

  @Before
  public void setUp() {
    Context context = InstrumentationRegistry.getTargetContext();
    Logger.setLogLevel(Logger.LogLevel.LOG_DEBUG);
    Logger.setDefaultHandler();
    MediasoupClient.initialize(context.getApplicationContext());

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
}
