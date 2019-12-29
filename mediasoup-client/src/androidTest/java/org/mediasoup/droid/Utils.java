package org.mediasoup.droid;

import android.text.TextUtils;

import java.io.ByteArrayOutputStream;
import java.io.InputStream;

public class Utils {

  public static String readTextStream(InputStream inputStream) throws Exception {
    ByteArrayOutputStream result = new ByteArrayOutputStream();
    byte[] buffer = new byte[1024];
    int length;
    while ((length = inputStream.read(buffer)) != -1) {
      result.write(buffer, 0, length);
    }
    return result.toString("UTF-8");
  }

  public interface MediaSoupRunnable {
    void run() throws MediasoupException;
  }

  public static boolean exceptionException(MediaSoupRunnable runnable, String errorMessage) {
    try {
      runnable.run();
    } catch (MediasoupException e) {
      return TextUtils.isEmpty(errorMessage) || e.getMessage().endsWith(errorMessage);
    } catch (Exception e) {
      return false;
    }
    return false;
  }

  public static boolean exceptionException(MediaSoupRunnable runnable) {
    return exceptionException(runnable, null);
  }
}
