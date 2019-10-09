package org.mediasoup.droid;

public class Device {

  private long mNativeDevice;

  public Device() {
    mNativeDevice = nativeNewDevice();
  }

  public void dispose() {
    nativeFreeDevice(mNativeDevice);
    mNativeDevice = 0;
  }

  public void load(String routerRtpCapabilities) {
    nativeLoad(mNativeDevice, routerRtpCapabilities);
  }

  public boolean isLoaded() {
    return nativeIsLoaded(mNativeDevice);
  }

  public String GetRtpCapabilities() {
    return nativeGetRtpCapabilities(mNativeDevice);
  }

  public boolean canProduce(String kind) {
    return nativeCanProduce(mNativeDevice, kind);
  }

  private Transport.SendTransport createSendTransport() {
    // TODO:
    return null;
  }

  private Transport.RecvTransport createRecvTransport() {
    // TODO:
    return null;
  }

  private static native long nativeNewDevice();

  private static native void nativeFreeDevice(long device);

  private native void nativeLoad(long device, String routerRtpCapabilities);

  private native boolean nativeIsLoaded(long device);

  private native String nativeGetRtpCapabilities(long device);

  private native boolean nativeCanProduce(long device, String kind);
}
