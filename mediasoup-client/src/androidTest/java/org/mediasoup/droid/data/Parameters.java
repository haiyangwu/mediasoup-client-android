package org.mediasoup.droid.data;

public class Parameters {

  public static native String generateRouterRtpCapabilities();

  public static native String generateRouterRtpCapabilitiesExclude(String exclude);

  public static native String generateRtpParametersByKind();

  public static native String generateLocalDtlsParameters();

  public static native String generateTransportRemoteParameters();

  public static native String generateProducerRemoteId();

  public static native String generateConsumerRemoteParameters(String codecMimeType);
}
