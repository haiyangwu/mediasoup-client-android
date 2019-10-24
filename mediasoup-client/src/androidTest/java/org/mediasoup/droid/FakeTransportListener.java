package org.mediasoup.droid;

import org.mediasoup.droid.data.Parameters;

public class FakeTransportListener {

  public static class FakeSendTransportListener implements SendTransport.Listener {

    private static final String TAG = "FakeSendTransportListener";

    @Override
    public void onConnect(Transport transport, String dtlsParameters) {
      Logger.v(TAG, "onConnect() " + dtlsParameters);
      mOnConnectTimesCalled++;
      mId = transport.getId();
      mDtlsParameters = dtlsParameters;
    }

    @Override
    public void onConnectionStateChange(Transport transport, String connectionState) {
      Logger.v(TAG, "onConnectionStateChange() " + connectionState);
      mOnConnectionStateChangeTimesCalled++;
    }

    @Override
    public String onProduce(
        Transport transport, String kind, String rtpParameters, String appData) {
      Logger.v(TAG, "onProduce() " + kind + "," + rtpParameters + "," + appData);
      mOnProduceTimesCalled++;
      mAppData = appData;
      if ("audio".equals(kind)) {
        mAudioProducerLocalParameters = rtpParameters;
        mAudioProducerId = Parameters.generateProducerRemoteId();
        return mAudioProducerId;
      } else if ("video".equals(kind)) {
        mVideoProducerLocalParameters = rtpParameters;
        mVideoProducerId = Parameters.generateProducerRemoteId();
        return mVideoProducerId;
      } else {
        throw new RuntimeException("Unknown producerLocalParameters[\\\"kind\\\"]");
      }
    }

    public String mId;
    public String mDtlsParameters;

    public String mAudioProducerLocalParameters;
    public String mAudioProducerId;
    public String mVideoProducerLocalParameters;
    public String mVideoProducerId;
    public String mAppData;

    public int mOnProduceTimesCalled = 0;
    public int mOnConnectTimesCalled = 0;
    public int mOnConnectionStateChangeTimesCalled = 0;

    public int mOnProduceExpectedTimesCalled = 0;
    public int mOnConnectExpectedTimesCalled = 0;
    public int mOnConnectionStateChangeExpectedTimesCalled = 0;
  }

  public static class FakeRecvTransportListener implements RecvTransport.Listener {

    private static final String TAG = "FakeRecvTransportListener";

    @Override
    public void onConnect(Transport transport, String dtlsParameters) {
      Logger.v(TAG, "onConnect() " + dtlsParameters);
      mOnConnectTimesCalled++;
      mId = transport.getId();
      mDtlsParameters = dtlsParameters;
    }

    @Override
    public void onConnectionStateChange(Transport transport, String connectionState) {
      Logger.v(TAG, "onConnectionStateChange() " + connectionState);
      this.mOnConnectionStateChangeTimesCalled++;
    }

    public String mId;
    public String mDtlsParameters;

    public int mOnConnectTimesCalled = 0;
    public int mOnConnectionStateChangeTimesCalled = 0;

    public int mOnConnectExpectedTimesCalled = 0;
    public int mOnConnectionStateChangeExpectedTimesCalled = 0;
  }

  public static class FakeProducerListener implements Producer.Listener {

    private static final String TAG = "FakeProducerListener";

    @Override
    public void onTransportClose() {
      Logger.v(TAG, "onTransportClose() ");
    }
  }
}
