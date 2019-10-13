package org.mediasoup.droid;

public class FakeTransportListener {

  public static class FakeSendTransportListener implements Transport.SendTransport.Listener {
    @Override
    public void onConnect(Transport transport, String dtlsParameters) {}

    @Override
    public void OnConnectionStateChange(Transport transport, String connectionState) {}
  }

  public static class FakeRecvTransportListener implements Transport.RecvTransport.Listener {
    @Override
    public void onConnect(Transport transport, String dtlsParameters) {}

    @Override
    public void OnConnectionStateChange(Transport transport, String connectionState) {}
  }
}
