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

  public Transport.SendTransport createSendTransport(
      Transport.SendTransport.Listener listener,
      String id,
      String iceParameters,
      String iceCandidates,
      String dtlsParameters) {
    return nativeCreateSendTransport(
        mNativeDevice, listener, id, iceParameters, iceCandidates, dtlsParameters);
  }

  public Transport.RecvTransport createRecvTransport(
      Transport.RecvTransport.Listener listener,
      String id,
      String iceParameters,
      String iceCandidates,
      String dtlsParameters) {
    return nativeCreateRecvTransport(
        mNativeDevice, listener, id, iceParameters, iceCandidates, dtlsParameters);
  }

  private static native long nativeNewDevice();

  private static native void nativeFreeDevice(long device);

  private native void nativeLoad(long device, String routerRtpCapabilities);

  private native boolean nativeIsLoaded(long device);

  private native String nativeGetRtpCapabilities(long device);

  private native boolean nativeCanProduce(long device, String kind);

  private native Transport.SendTransport nativeCreateSendTransport(
      long device,
      Transport.SendTransport.Listener listener,
      String id,
      String iceParameters,
      String iceCandidates,
      String dtlsParameters);

  private native Transport.RecvTransport nativeCreateRecvTransport(
      long device,
      Transport.RecvTransport.Listener listener,
      String id,
      String iceParameters,
      String iceCandidates,
      String dtlsParameters);
}
