package org.mediasoup.droid;

import android.support.test.runner.AndroidJUnit4;
import android.text.TextUtils;

import org.json.JSONException;
import org.json.JSONObject;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mediasoup.droid.data.Parameters;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mediasoup.droid.Utils.exceptionException;
import static org.mediasoup.droid.data.Parameters.generateTransportRemoteParameters;

@RunWith(AndroidJUnit4.class)
public class MediasoupClientTest extends BaseTest {

  private String mId;
  private String mIceParameters;
  private String mIceCandidates;
  private String mDtlsParameters;

  @Override
  @Before
  public void setUp() {
    super.setUp();
    try {
      JSONObject transportRemoteParameters = new JSONObject(generateTransportRemoteParameters());
      mId = transportRemoteParameters.getString("id");
      mIceParameters = transportRemoteParameters.getString("iceParameters");
      mIceCandidates = transportRemoteParameters.getString("iceCandidates");
      mDtlsParameters = transportRemoteParameters.getString("dtlsParameters");
    } catch (JSONException e) {
      e.printStackTrace();
    }
  }

  @Test
  public void mediasoupclient() {
    Device device;
    String routerRtpCapabilities;

    SendTransport sendTransport;
    RecvTransport recvTransport;

    // create a Device succeeds.
    {
      device = new Device();
      assertFalse(device.isLoaded());
    }

    // "device->GetRtpCapabilities throws if not loaded.
    {
      exceptionException(device::GetRtpCapabilities);
    }

    // device->CanProduce() throws if not loaded.
    {
      exceptionException(() -> device.canProduce("audio"));
      exceptionException(() -> device.canProduce("video"));
    }

    // 'device->CreateSendTransport()' throws if not loaded.
    {
      final FakeTransportListener.FakeRecvTransportListener listener =
          new FakeTransportListener.FakeRecvTransportListener();
      exceptionException(
          () ->
              device.createRecvTransport(
                  listener, mId, mIceParameters, mIceCandidates, mDtlsParameters));
    }

    // device->load() with invalid routerRtpCapabilities throws.
    {
      final String cap = Parameters.generateRouterRtpCapabilitiesExclude("mimeType");
      exceptionException(() -> device.load(cap));
    }

    // device->load() succeeds.
    {
      routerRtpCapabilities = Parameters.generateRouterRtpCapabilities();
      device.load(routerRtpCapabilities);
      assertTrue(device.isLoaded());
    }

    // device->load() rejects if already loaded.
    {
      final String cap = routerRtpCapabilities;
      exceptionException(() -> device.load(cap));
    }

    // 'device->GetRtpCapabilities()' succeeds".
    {
      assertFalse(TextUtils.isEmpty(device.GetRtpCapabilities()));
    }

    // device->CanProduce() with 'audio'/'video' kind returns true.
    {
      assertTrue(device.canProduce("audio"));
      assertTrue(device.canProduce("video"));
    }

    // device->CanProduce() with invalid kind throws exception.
    {
      exceptionException(() -> device.canProduce("chicken"));
    }
    // "device->createSendTransport() for sending media succeeds.
    {
      String appData = "{\"baz\":\"BAZ\"}";

      final FakeTransportListener.FakeSendTransportListener listener =
          new FakeTransportListener.FakeSendTransportListener();
      sendTransport =
          device.createSendTransport(
              listener, mId, mIceParameters, mIceCandidates, mDtlsParameters, null, appData);

      assertEquals(mId, sendTransport.getId());
      assertFalse(sendTransport.isClosed());
      assertEquals(appData, sendTransport.getAppData());
      assertEquals("new", sendTransport.getConnectionState());
    }
    // device->createRecvTransport() for receiving media succeeds.
    {
      final FakeTransportListener.FakeRecvTransportListener listener =
          new FakeTransportListener.FakeRecvTransportListener();
      recvTransport =
          device.createRecvTransport(
              listener, mId, mIceParameters, mIceCandidates, mDtlsParameters);

      assertEquals(mId, recvTransport.getId());
      assertFalse(recvTransport.isClosed());
      assertEquals("{}", recvTransport.getAppData());
      assertEquals("new", recvTransport.getConnectionState());
    }

    // "transport.produce() succeeds.
    {
      // TODO:
    }

    // transport.produce() without track throws.
    {
      // TODO:
    }

    // transport.consume() succeeds.
    {
      // TODO:
    }

    // transport.consume() with unsupported consumerRtpParameters throws.
    {
      // TODO:
    }

    // 'sendTransport.GetStats()' succeeds.
    {
      // TODO:
    }

    // sendTransport.RestartIce()' succeeds.
    {
      // TODO:
    }

    // 'sendTransport.UpdateIceServers()' succeeds.
    {
      // TODO:
    }

    // 'producer->Pause()' succeeds.
    {
      // TODO:
    }

    // 'producer->Resume()' succeeds.
    {
      // TODO:
    }

    // 'producer->ReplaceTrack()' succeeds.
    {
      // TODO:
    }

    // 'producer->ReplaceTrack()' fails if null track is provided.
    {
      // TODO:
    }

    // 'producer->SetMaxSpatialLayer()' succeeds.
    {
      // TODO:
    }

    // 'producer->SetMaxSpatialLayer()' in an audio Producer throws.
    {
      // TODO:
    }

    // 'producer->GetStats()' succeeds.
    {
      // TODO:
    }

    // 'consumer->Resume()' succeeds.
    {
      // TODO:
    }

    // 'consumer->Pause()' succeeds.
    {
      // TODO:
    }

    // 'consumer->GetStats()' succeeds.
    {
      // TODO:
    }

    // 'producer->Close()' succeeds.
    {
      // TODO:
    }

    // producer->getStats() throws if closed.
    {
      // TODO:
    }

    // consumer->Close()' succeeds.
    {
      // TODO:
    }

    // consumer->getStats() throws if closed.
    {
      // TODO:
    }

    // transport->Close() fires 'OnTransportClose' in live Producers/Consumers.
    {
      // TODO:
    }

    // transport.produce() throws if closed.
    {
      // TODO:
    }

    // transport.consume() throws if closed.
    {
      // TODO:
    }

    // transport.getStats() throws if closed.
    {
      // TODO:
    }

    // transport.restartIce() throws if closed".
    {
      // TODO:
    }

    // transport.restartIce() throws if closed".
    {
      // TODO:
    }

    // dispose.
    sendTransport.dispose();
    recvTransport.dispose();
    device.dispose();
  }

  @Test
  public void version() {
    assertEquals(MediasoupClient.version(), BuildConfig.VERSION_NAME);
  }
}
