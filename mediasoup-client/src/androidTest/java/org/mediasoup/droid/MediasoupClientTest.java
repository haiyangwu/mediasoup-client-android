package org.mediasoup.droid;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class MediasoupClientTest {

    @Test
    public void version() {
        assertEquals(MediasoupClient.version(), BuildConfig.VERSION_NAME);
    }
}