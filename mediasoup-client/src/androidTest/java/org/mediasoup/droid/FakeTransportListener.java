package org.mediasoup.droid;

import org.mediasoup.droid.data.Parameters;

public class FakeTransportListener {

  public static class FakeSendTransportListener implements SendTransport.Listener {

    @Override
    public void onConnect(Transport transport, String dtlsParameters) {}

    @Override
    public void OnConnectionStateChange(Transport transport, String connectionState) {}

    @Override
    public String onProduce(
        Transport transport, String kind, String rtpParameters, String appData) {
      return Parameters.generateProducerRemoteId();
    }
  }

  public static class FakeRecvTransportListener implements RecvTransport.Listener {
    @Override
    public void onConnect(Transport transport, String dtlsParameters) {}

    @Override
    public void OnConnectionStateChange(Transport transport, String connectionState) {}
  }
}
