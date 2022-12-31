package org.mediasoup.droid;

import android.util.Log;

import org.webrtc.CalledByNative;

import java.io.PrintWriter;
import java.io.StringWriter;

public class Logger {

  static {
    System.loadLibrary("mediasoupclient_so");
  }

  private static LogHandlerInterface logHandler;
  private static long nativeHandler;

  public enum LogLevel {
    LOG_NONE(0),
    LOG_ERROR(1),
    LOG_WARN(2),
    LOG_DEBUG(3),
    LOG_TRACE(4);

    private final int value;

    LogLevel(int value) {
      this.value = value;
    }

    int getValue() {
      return value;
    }

    @CalledByNative("LogLevel")
    public static LogLevel getLogLevel(int level) {
      LogLevel[] levels = LogLevel.values();
      for (int i = 0; i < levels.length; i++) {
        if (levels[i].getValue() == level) {
          return levels[i];
        }
      }

      throw new IllegalArgumentException("wrong log level");
    }
  }

  public interface LogHandlerInterface {
    @CalledByNative("LogHandlerInterface")
    void OnLog(LogLevel logLevel, String tag, String message);
  }

  private static class DefaultLogHandler implements LogHandlerInterface {

    @Override
    public void OnLog(LogLevel level, String tag, String message) {
      final int chunkSize = 2048;
      switch (level) {
        case LOG_ERROR:
          for (int i = 0; i < message.length(); i += chunkSize) {
            Log.e(tag, message.substring(i, Math.min(message.length(), i + chunkSize)));
          }
          break;
        case LOG_WARN:
          for (int i = 0; i < message.length(); i += chunkSize) {
            Log.w(tag, message.substring(i, Math.min(message.length(), i + chunkSize)));
          }
          break;
        case LOG_DEBUG:
          for (int i = 0; i < message.length(); i += chunkSize) {
            Log.d(tag, message.substring(i, Math.min(message.length(), i + chunkSize)));
          }
          break;
        case LOG_TRACE:
          for (int i = 0; i < message.length(); i += chunkSize) {
            Log.i(tag, message.substring(i, Math.min(message.length(), i + chunkSize)));
          }
          break;
      }
    }
  }

  public static void setLogLevel(LogLevel level) {
    if (level == null) {
      throw new IllegalArgumentException("Logging level may not be null.");
    }
    nativeSetLogLevel(level.getValue());
  }

  public static void setDefaultHandler() {
    setHandler(new DefaultLogHandler());
  }

  public static void setHandler(LogHandlerInterface handler) {
    logHandler = handler;
    nativeHandler = nativeSetHandler(handler);
  }

  public static void freeHandler() {
    nativeFreeLogHandler(nativeHandler);
    nativeHandler = 0;
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

    if (logHandler != null) {
      logHandler.OnLog(level, tag, message);
    }
  }

  private static native void nativeSetLogLevel(int level);

  private static native long nativeSetHandler(LogHandlerInterface handler);

  private static native void nativeFreeLogHandler(long handler);
}
