package org.mediasoup.droid;

import android.content.Context;
import android.support.test.InstrumentationRegistry;

import org.junit.Before;

public abstract class BaseTest {

  @Before
  public void setUp() {
    Context context = InstrumentationRegistry.getContext();
    Logger.setLogLevel(Logger.LogLevel.LOG_DEBUG);
    Logger.setDefaultHandler();
    MediasoupClient.initialize(context.getApplicationContext());
  }
}
