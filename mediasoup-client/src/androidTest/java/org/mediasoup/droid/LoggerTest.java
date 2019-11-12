package org.mediasoup.droid;

import androidx.test.ext.junit.runners.AndroidJUnit4;

import org.junit.Test;
import org.junit.runner.RunWith;

@RunWith(AndroidJUnit4.class)
public class LoggerTest {

  private static final String TAG = "LoggerTest";

  @Test
  public void setDefaultHandler() {
    Logger.setDefaultHandler();
  }

  @Test
  public void log() {
    Logger.setLogLevel(Logger.LogLevel.LOG_TRACE);

    Logger.v(TAG, "test log");
    Logger.d(TAG, "test log");
    Logger.w(TAG, "test log");
    Logger.e(TAG, "test log");
  }
}
