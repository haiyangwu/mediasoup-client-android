package org.mediasoup.droid;

import org.webrtc.CalledByNative;

public class Producer {

  public interface Listener {

    @CalledByNative
    void onTransportClose();
  }

  private long mNativeProducer;

  public Producer(long nativeProducer) {
    mNativeProducer = nativeProducer;
  }
}
