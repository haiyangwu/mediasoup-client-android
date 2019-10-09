package org.mediasoup.droid;

import android.support.test.runner.AndroidJUnit4;

import org.junit.Test;
import org.junit.runner.RunWith;

import static org.junit.Assert.assertEquals;

@RunWith(AndroidJUnit4.class)
public class MediasoupClientTest {

  @Test
  public void version() {
    assertEquals(MediasoupClient.version(), BuildConfig.VERSION_NAME);
  }
}
