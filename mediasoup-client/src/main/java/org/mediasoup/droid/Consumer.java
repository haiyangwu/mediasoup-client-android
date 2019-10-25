package org.mediasoup.droid;

import org.webrtc.CalledByNative;

public class Consumer {

  public interface Listener {

    @CalledByNative
    void onTransportClose(Consumer consumer);
  }

  private long mNativeConsumer;

  @CalledByNative
  public Consumer(long nativeProducer) {
    mNativeConsumer = nativeProducer;
  }
}
