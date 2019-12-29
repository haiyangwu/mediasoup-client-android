package org.mediasoup.droid;

import android.content.Context;
import androidx.test.platform.app.InstrumentationRegistry;

import org.junit.Before;

public abstract class BaseTest {

  protected Context mContext;

  @Before
  public void setUp() {
    mContext = InstrumentationRegistry.getInstrumentation().getTargetContext();
    Logger.setLogLevel(Logger.LogLevel.LOG_TRACE);
    Logger.setDefaultHandler();
    MediasoupClient.initialize(mContext.getApplicationContext());
  }
}
