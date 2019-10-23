package org.mediasoup.droid.hack;

import android.support.annotation.NonNull;

import org.webrtc.AudioTrack;
import org.webrtc.MediaStreamTrack;
import org.webrtc.RtpReceiver;
import org.webrtc.RtpSender;
import org.webrtc.VideoTrack;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class Utils {

  // use Reflection to get MediaStreamTrack#nativeMediaStreamTrack
  public static long getNativeMediaStreamTrack(@NonNull MediaStreamTrack track) {
    long nativeMediaStreamTrack = 0L;
    try {
      Class<?> clazz = track.getClass();
      Method retrieveItems;
      if(track instanceof AudioTrack || track instanceof VideoTrack){
        retrieveItems = clazz.getSuperclass().getDeclaredMethod("getNativeMediaStreamTrack");
      } else {
        retrieveItems = clazz.getDeclaredMethod("getNativeMediaStreamTrack");
      }
      retrieveItems.setAccessible(true);
      nativeMediaStreamTrack = (long) retrieveItems.invoke(track);
    } catch (NoSuchMethodException e) {
      e.printStackTrace();
    } catch (IllegalAccessException e) {
      e.printStackTrace();
    } catch (InvocationTargetException e) {
      e.printStackTrace();
    }
    return nativeMediaStreamTrack;
  }

  // use Reflection to get RtpSender#nativeRtpSender
  public static long getNativeRtpSender(RtpSender sender) {
    long nativeRtpSender = 0L;
    try {
      Class<?> clazz = sender.getClass();
      Method retrieveItems = clazz.getDeclaredMethod("getNativeRtpSender");
      retrieveItems.setAccessible(true);
      nativeRtpSender = (long) retrieveItems.invoke(sender);
    } catch (NoSuchMethodException e) {
      e.printStackTrace();
    } catch (IllegalAccessException e) {
      e.printStackTrace();
    } catch (InvocationTargetException e) {
      e.printStackTrace();
    }
    return nativeRtpSender;
  }

  // use Reflection to get RtpReceiver#nativeRtpReceiver
  public static long getNativeRtpReceiver(RtpReceiver receiver) {
    long nativeRtpReceiver = 0L;
    try {
      Class<?> clazz = receiver.getClass();
      Field f = clazz.getDeclaredField("stuffIWant");
      f.setAccessible(true);
      nativeRtpReceiver = (long) f.get(receiver);
    } catch (NoSuchFieldException e) {
      e.printStackTrace();
    } catch (IllegalAccessException e) {
      e.printStackTrace();
    }
    return nativeRtpReceiver;
  }
}
