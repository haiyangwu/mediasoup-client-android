package org.mediasoup.droid;

import androidx.test.ext.junit.runners.AndroidJUnit4;
import android.text.TextUtils;

import org.json.JSONException;
import org.json.JSONObject;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mediasoup.droid.data.Parameters;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mediasoup.droid.Utils.exceptionException;
import static org.mediasoup.droid.data.Parameters.nativeGenTransportRemoteParameters;

@RunWith(AndroidJUnit4.class)
public class DeviceTest extends BaseTest {

  private String mId;
  private String mIceParameters;
  private String mIceCandidates;
  private String mDtlsParameters;
  private Device mDevice;

  @Before
  public void setUp() {
    super.setUp();
    try {
      JSONObject transportRemoteParameters = new JSONObject(nativeGenTransportRemoteParameters());
      mId = transportRemoteParameters.getString("id");
      mIceParameters = transportRemoteParameters.getString("iceParameters");
      mIceCandidates = transportRemoteParameters.getString("iceCandidates");
      mDtlsParameters = transportRemoteParameters.getString("dtlsParameters");
    } catch (JSONException e) {
      e.printStackTrace();
    }
    mDevice = new Device();
  }

  @After
  public void tearDown() {
    mDevice.dispose();
  }

  @Test
  public void testWithoutLoad() throws JSONException {
    // 'device->IsLoaded()' is false if not loaded.
    {
      assertFalse(mDevice.isLoaded());
    }

    // 'device->getRtpCapabilities()' throws if not loaded.
    {
      exceptionException(mDevice::getRtpCapabilities, "Not loaded");
    }

    // 'device->CanProduce()' with audio/video throws if not loaded.
    {
      exceptionException(() -> mDevice.canProduce("video"), "Not loaded");
      exceptionException(() -> mDevice.canProduce("audio"), "Not loaded");
    }

    // 'device->CreateSendTransport()' fails if not loaded".
    {
      final FakeTransportListener.FakeSendTransportListener listener =
          new FakeTransportListener.FakeSendTransportListener();
      exceptionException(
          () ->
              mDevice.createSendTransport(
                  listener, mId, mIceParameters, mIceCandidates, mDtlsParameters));
    }

    // 'device->CreateRecvTransport()' fails if not loaded.
    {
      final FakeTransportListener.FakeRecvTransportListener listener =
          new FakeTransportListener.FakeRecvTransportListener();
      exceptionException(
          () ->
              mDevice.createRecvTransport(
                  listener, mId, mIceParameters, mIceCandidates, mDtlsParameters));
    }
  }

  @Test
  public void testLoad() throws MediasoupException {
    String routerRtpCapabilities = Parameters.nativeGenRouterRtpCapabilities();
    assertFalse(TextUtils.isEmpty(routerRtpCapabilities));

    // 'device->Load()' succeeds.
    mDevice.load(routerRtpCapabilities);
    assertTrue(mDevice.isLoaded());
    assertFalse(TextUtils.isEmpty(mDevice.getRtpCapabilities()));
    assertTrue(mDevice.canProduce("audio"));
    assertTrue(mDevice.canProduce("video"));

    // device->CanProduce() with invalid kind throws exception.
    exceptionException(() -> mDevice.canProduce("chicken"));

    // 'device->CreateSendTransport()' succeeds.
    {
      final FakeTransportListener.FakeSendTransportListener listener =
          new FakeTransportListener.FakeSendTransportListener();
      SendTransport transport =
          mDevice.createSendTransport(
              listener, mId, mIceParameters, mIceCandidates, mDtlsParameters);
      transport.dispose();
    }

    // 'device->CreateRecvTransport()' succeeds.
    {
      final FakeTransportListener.FakeRecvTransportListener listener =
          new FakeTransportListener.FakeRecvTransportListener();
      RecvTransport transport =
          mDevice.createRecvTransport(
              listener, mId, mIceParameters, mIceCandidates, mDtlsParameters);
      transport.dispose();
    }
  }
}
