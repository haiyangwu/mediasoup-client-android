package org.mediasoup.droid;

import android.support.test.runner.AndroidJUnit4;
import android.text.TextUtils;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mediasoup.droid.data.Parameters;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mediasoup.droid.Utils.exceptionException;

@RunWith(AndroidJUnit4.class)
public class DeviceTest extends BaseTest {

  private Device mDevice;

  @Before
  public void setUp() {
    super.setUp();
    mDevice = new Device();
  }

  @After
  public void tearDown() {
    mDevice.dispose();
  }

  @Test
  public void testWithoutLoad() {
    // 'device->IsLoaded()' is false if not loaded.
    assertFalse(mDevice.isLoaded());

    // 'device->GetRtpCapabilities()' throws if not loaded.
    exceptionException(mDevice::GetRtpCapabilities, "Not loaded");

    // 'device->CanProduce()' with audio/video throws if not loaded.
    exceptionException(() -> mDevice.canProduce("video"), "Not loaded");
    exceptionException(() -> mDevice.canProduce("audio"), "Not loaded");

    // 'device->CreateSendTransport()' fails if not loaded".
    // TODO:

    // 'device->CreateRecvTransport()' fails if not loaded.
    // TODO:
  }

  @Test
  public void testLoad() {
    String routerRtpCapabilities = Parameters.generateRouterRtpCapabilities();
    assertFalse(TextUtils.isEmpty(routerRtpCapabilities));

    // 'device->Load()' succeeds.
    mDevice.load(routerRtpCapabilities);
    assertTrue(mDevice.isLoaded());
    assertFalse(TextUtils.isEmpty(mDevice.GetRtpCapabilities()));
    assertTrue(mDevice.canProduce("audio"));
    assertTrue(mDevice.canProduce("video"));

    // device->CanProduce() with invalid kind throws exception.
    exceptionException(() -> mDevice.canProduce("chicken"));

    // 'device->CreateSendTransport()' succeeds.
    // TODO;

    // 'device->CreateRecvTransport()' succeeds.
    // TODO:
  }
}
