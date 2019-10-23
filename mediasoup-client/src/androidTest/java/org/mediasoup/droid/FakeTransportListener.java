package org.mediasoup.droid;

import org.mediasoup.droid.data.Parameters;

public class FakeTransportListener {

  public static class FakeSendTransportListener implements SendTransport.Listener {

    private static final String TAG = "FakeSendTransportListener";

    @Override
    public void onConnect(String dtlsParameters) {
      Logger.v(TAG, "onConnect() " + dtlsParameters);
    }

    @Override
    public void OnConnectionStateChange(String connectionState) {
      Logger.v(TAG, "OnConnectionStateChange() " + connectionState);
    }

    @Override
    public String onProduce(String kind, String rtpParameters, String appData) {
      Logger.v(TAG, "onProduce() " + kind + "," + rtpParameters + "," + appData);
      return Parameters.generateProducerRemoteId();
    }
  }

  public static class FakeRecvTransportListener implements RecvTransport.Listener {

    private static final String TAG = "FakeRecvTransportListener";

    @Override
    public void onConnect(String dtlsParameters) {
      Logger.v(TAG, "onConnect() " + dtlsParameters);
    }

    @Override
    public void OnConnectionStateChange(String connectionState) {
      Logger.v(TAG, "OnConnectionStateChange() " + connectionState);
    }
  }

  public static class FakeProducerListener implements Producer.Listener {

    private static final String TAG = "FakeProducerListener";

    @Override
    public void onTransportClose() {
      Logger.v(TAG, "onTransportClose() ");
    }
  }
}
