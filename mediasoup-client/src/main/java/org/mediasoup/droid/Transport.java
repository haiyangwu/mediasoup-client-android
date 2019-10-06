package org.mediasoup.droid;

public class Transport {

    public interface Listener {

        void onConnect(Transport transport, String dtlsParameters);

        void OnConnectionStateChange(Transport transport, String connectionState);
    }

    public static class SendTransport extends Transport {

        public interface Listener extends Transport.Listener {

        }

    }

    public static class RecvTransport extends Transport {

        public interface Listener extends Transport.Listener {

        }
    }
}
