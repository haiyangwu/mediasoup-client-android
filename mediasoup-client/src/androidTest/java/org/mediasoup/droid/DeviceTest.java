package org.mediasoup.droid;

import android.content.Context;
import android.support.test.InstrumentationRegistry;
import android.support.test.runner.AndroidJUnit4;
import android.text.TextUtils;

import org.junit.After;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;
import org.junit.runner.RunWith;
import org.mediasoup.droid.data.Parameters;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

@RunWith(AndroidJUnit4.class)
public class DeviceTest {

    private Device mDevice;

    @Rule
    public ExpectedException thrown = ExpectedException.none();

    @Before
    public void setUp() {
        Context context = InstrumentationRegistry.getTargetContext();

        Logger.setLogLevel(Logger.LogLevel.LOG_DEBUG);
        Logger.setDefaultHandler();

        MediasoupClient.initialize(context.getApplicationContext());
        mDevice = new Device();
    }

    @After
    public void tearDown() {
        mDevice.dispose();
    }

    @Test(expected = RuntimeException.class)
    public void testWithoutLoad() {
        // 'device->IsLoaded()' is false if not loaded.
        assertFalse(mDevice.isLoaded());

        // 'device->GetRtpCapabilities()' throws if not loaded.
        mDevice.GetRtpCapabilities();
        thrown.expect(RuntimeException.class);
        thrown.expectMessage("Not loaded");

        // 'device->CanProduce()' with audio/video throws if not loaded/
        mDevice.canProduce("video");
        thrown.expect(RuntimeException.class);
        thrown.expectMessage("Not loaded");
        mDevice.canProduce("audio");
        thrown.expect(RuntimeException.class);
        thrown.expectMessage("Not loaded");

        // 'device->CreateSendTransport()' fails if not loaded"
        // TODO:

        // 'device->CreateRecvTransport()' fails if not loaded
        // TODO:
    }

    @Test(expected = RuntimeException.class)
    public void testLoad() {
        String routerRtpCapabilities = Parameters.generateRouterRtpCapabilities();
        assertFalse(TextUtils.isEmpty(routerRtpCapabilities));

        // 'device->Load()' succeeds
        mDevice.load(routerRtpCapabilities);
        assertTrue(mDevice.isLoaded());
        assertFalse(TextUtils.isEmpty(mDevice.GetRtpCapabilities()));
        assertTrue(mDevice.canProduce("audio"));
        assertTrue(mDevice.canProduce("video"));

        // device->CanProduce() with invalid kind throws exception
        assertTrue(mDevice.canProduce("chicken"));
        thrown.expect(RuntimeException.class);

        // 'device->CreateSendTransport()' succeeds
        // TODO;

        // 'device->CreateRecvTransport()' succeeds
        // TODO:
    }
}
