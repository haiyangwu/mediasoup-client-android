package org.mediasoup.droid;

import android.support.test.runner.AndroidJUnit4;
import android.text.TextUtils;

import org.json.JSONException;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mediasoup.droid.data.Parameters;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mediasoup.droid.Utils.exceptionException;

@RunWith(AndroidJUnit4.class)
public class MediasoupClientTest extends BaseTest {

  @Test
  public void mediasoupclient() throws JSONException {
    Device device;
    String routerRtpCapabilities;

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
      // TODO:
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
      // TODO:
    }

    // device->createRecvTransport() for receiving media succeeds.
    {
      // TODO:
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
  }

  @Test
  public void version() {
    assertEquals(MediasoupClient.version(), BuildConfig.VERSION_NAME);
  }
}
