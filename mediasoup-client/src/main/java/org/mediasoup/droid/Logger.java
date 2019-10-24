package org.mediasoup.droid;

import android.util.Log;

import java.io.PrintWriter;
import java.io.StringWriter;

public class Logger {

  static {
    System.loadLibrary("mediasoupclient_so");
  }

  private static LogLevel loggableLevel = LogLevel.LOG_NONE;

  public enum LogLevel {
    LOG_NONE,
    LOG_ERROR,
    LOG_WARN,
    LOG_DEBUG,
    LOG_TRACE,
  }

  public static void setLogLevel(LogLevel level) {
    if (level == null) {
      throw new IllegalArgumentException("Logging level may not be null.");
    }
    loggableLevel = level;
    if (loggableLevel.ordinal() <= LogLevel.LOG_DEBUG.ordinal()) {
      nativeSetLogLevel(level.ordinal());
    } else {
      // TODO(haiyangwu): native log level means different.
      nativeSetLogLevel(LogLevel.LOG_DEBUG.ordinal());
    }
  }

  public static void setDefaultHandler() {
    nativeSetDefaultHandler();
  }

  public static void d(String tag, String message) {
    log(LogLevel.LOG_DEBUG, tag, message);
  }

  public static void e(String tag, String message) {
    log(LogLevel.LOG_ERROR, tag, message);
  }

  public static void w(String tag, String message) {
    log(LogLevel.LOG_WARN, tag, message);
  }

  public static void e(String tag, String message, Throwable e) {
    log(LogLevel.LOG_ERROR, tag, message);
    log(LogLevel.LOG_ERROR, tag, e.toString());
    log(LogLevel.LOG_ERROR, tag, getStackTraceString(e));
  }

  public static void w(String tag, String message, Throwable e) {
    log(LogLevel.LOG_WARN, tag, message);
    log(LogLevel.LOG_WARN, tag, e.toString());
    log(LogLevel.LOG_WARN, tag, getStackTraceString(e));
  }

  public static void v(String tag, String message) {
    log(LogLevel.LOG_TRACE, tag, message);
  }

  private static String getStackTraceString(Throwable e) {
    if (e == null) {
      return "";
    }

    StringWriter sw = new StringWriter();
    PrintWriter pw = new PrintWriter(sw);
    e.printStackTrace(pw);
    return sw.toString();
  }

  public static void log(LogLevel level, String tag, String message) {
    if (tag == null || message == null) {
      throw new IllegalArgumentException("Log tag or message may not be null.");
    }

    // Filter log messages below logLevel.
    if (LogLevel.LOG_NONE.equals(loggableLevel) || level.ordinal() > loggableLevel.ordinal()) {
      return;
    }

    switch (level) {
      case LOG_ERROR:
        Log.e(tag, message);
        break;
      case LOG_WARN:
        Log.w(tag, message);
        break;
      case LOG_DEBUG:
        Log.d(tag, message);
        break;
      case LOG_TRACE:
        Log.v(tag, message);
        break;
    }
  }

  private static native void nativeSetLogLevel(int level);

  private static native void nativeSetDefaultHandler();
}
