package org.mediasoup.droid;

public class Device {

  private long mNativeDevice;

  public Device() {
    mNativeDevice = nativeNewDevice();
  }

  public void dispose() {
    checkDeviceExists();
    nativeFreeDevice(mNativeDevice);
    mNativeDevice = 0;
  }

  public void load(String routerRtpCapabilities) throws MediasoupException {
    checkDeviceExists();
    nativeLoad(mNativeDevice, routerRtpCapabilities);
  }

  public boolean isLoaded() {
    checkDeviceExists();
    return nativeIsLoaded(mNativeDevice);
  }

  public String getRtpCapabilities() throws MediasoupException {
    checkDeviceExists();
    return nativeGetRtpCapabilities(mNativeDevice);
  }

  public boolean canProduce(String kind) throws MediasoupException {
    checkDeviceExists();
    return nativeCanProduce(mNativeDevice, kind);
  }

  public SendTransport createSendTransport(
      SendTransport.Listener listener,
      String id,
      String iceParameters,
      String iceCandidates,
      String dtlsParameters)
      throws MediasoupException {
    return createSendTransport(
        listener, id, iceParameters, iceCandidates, dtlsParameters, null, null);
  }

  public SendTransport createSendTransport(
      SendTransport.Listener listener,
      String id,
      String iceParameters,
      String iceCandidates,
      String dtlsParameters,
      PeerConnection.Options options,
      String appData)
      throws MediasoupException {
    checkDeviceExists();
    return nativeCreateSendTransport(
        mNativeDevice,
        listener,
        id,
        iceParameters,
        iceCandidates,
        dtlsParameters,
        (options != null ? options.mRTCConfig : null),
        (options != null && options.mFactory != null
            ? options.mFactory.getNativePeerConnectionFactory()
            : 0L),
        appData);
  }

  public RecvTransport createRecvTransport(
      RecvTransport.Listener listener,
      String id,
      String iceParameters,
      String iceCandidates,
      String dtlsParameters)
      throws MediasoupException {
    return createRecvTransport(
        listener, id, iceParameters, iceCandidates, dtlsParameters, null, null);
  }

  public RecvTransport createRecvTransport(
      RecvTransport.Listener listener,
      String id,
      String iceParameters,
      String iceCandidates,
      String dtlsParameters,
      PeerConnection.Options options,
      String appData)
      throws MediasoupException {
    checkDeviceExists();
    return nativeCreateRecvTransport(
        mNativeDevice,
        listener,
        id,
        iceParameters,
        iceCandidates,
        dtlsParameters,
        (options != null ? options.mRTCConfig : null),
        (options != null && options.mFactory != null
            ? options.mFactory.getNativePeerConnectionFactory()
            : 0L),
        appData);
  }

  private void checkDeviceExists() {
    if (mNativeDevice == 0) {
      throw new IllegalStateException("Device has been disposed.");
    }
  }

  private static native long nativeNewDevice();

  private static native void nativeFreeDevice(long device);

  // may throws MediasoupException;
  private static native void nativeLoad(long device, String routerRtpCapabilities);

  private static native boolean nativeIsLoaded(long device);

  // may throws MediasoupException;
  private static native String nativeGetRtpCapabilities(long device);

  // may throws MediasoupException;
  private static native boolean nativeCanProduce(long device, String kind);

  // may throws MediasoupException;
  private static native SendTransport nativeCreateSendTransport(
      long device,
      SendTransport.Listener listener,
      String id,
      String iceParameters,
      String iceCandidates,
      String dtlsParameters,
      org.webrtc.PeerConnection.RTCConfiguration configuration,
      long peerConnectionFactory,
      String appData);

  // may throws MediasoupException;
  private static native RecvTransport nativeCreateRecvTransport(
      long device,
      RecvTransport.Listener listener,
      String id,
      String iceParameters,
      String iceCandidates,
      String dtlsParameters,
      org.webrtc.PeerConnection.RTCConfiguration configuration,
      long peerConnectionFactory,
      String appData);
}
