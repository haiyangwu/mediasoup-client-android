package org.mediasoup.droid.data;

public class Parameters {

  public static native String nativeGenRouterRtpCapabilities();

  public static native String nativeGenRouterRtpCapabilitiesExclude(String exclude);

  public static native String nativeGenRtpParametersByKind();

  public static native String nativeGenLocalDtlsParameters();

  public static native String nativeGenTransportRemoteParameters();

  public static native String nativeGenProducerRemoteId();

  public static native String nativeGenConsumerRemoteParameters(String codecMimeType);
}
