package org.mediasoup.droid;

import androidx.test.ext.junit.runners.AndroidJUnit4;
import android.text.TextUtils;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mediasoup.droid.data.Parameters;
import org.webrtc.AudioTrack;
import org.webrtc.RTCUtils;
import org.webrtc.RtpParameters;
import org.webrtc.VideoTrack;

import java.util.ArrayList;
import java.util.List;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.mediasoup.droid.Utils.exceptionException;
import static org.mediasoup.droid.data.Parameters.nativeGenTransportRemoteParameters;

@RunWith(AndroidJUnit4.class)
public class MediasoupClientTest extends BaseTest {

  private String mId;
  private String mIceParameters;
  private String mIceCandidates;
  private String mDtlsParameters;

  @Override
  @Before
  public void setUp() {
    super.setUp();
    try {
      JSONObject transportRemoteParameters = new JSONObject(nativeGenTransportRemoteParameters());
      mId = transportRemoteParameters.getString("id");
      mIceParameters = transportRemoteParameters.getString("iceParameters");
      mIceCandidates = transportRemoteParameters.getString("iceCandidates");
      mDtlsParameters = transportRemoteParameters.getString("dtlsParameters");
    } catch (JSONException e) {
      e.printStackTrace();
    }
  }

  @Test
  public void mediasoupclient() throws JSONException, MediasoupException {
    Device device;
    String routerRtpCapabilities;

    FakeTransportListener.FakeSendTransportListener sendTransportListener;
    FakeTransportListener.FakeRecvTransportListener recvTransportListener;

    SendTransport sendTransport;
    RecvTransport recvTransport;

    AudioTrack audioTrack;
    VideoTrack videoTrack;

    final FakeTransportListener.FakeProducerListener producerListener =
        new FakeTransportListener.FakeProducerListener();
    final FakeTransportListener.FakeConsumerListener consumerListener =
        new FakeTransportListener.FakeConsumerListener();

    Producer audioProducer;
    Producer videoProducer;

    Consumer audioConsumer;
    Consumer videoConsumer;

    // create a Device succeeds.
    {
      device = new Device();
      assertFalse(device.isLoaded());
    }

    // "device->getRtpCapabilities throws if not loaded.
    {
      exceptionException(device::getRtpCapabilities);
    }

    // device->CanProduce() throws if not loaded.
    {
      exceptionException(() -> device.canProduce("audio"));
      exceptionException(() -> device.canProduce("video"));
    }

    // 'device->CreateSendTransport()' throws if not loaded.
    {
      final FakeTransportListener.FakeRecvTransportListener listener =
          new FakeTransportListener.FakeRecvTransportListener();
      exceptionException(
          () ->
              device.createRecvTransport(
                  listener, mId, mIceParameters, mIceCandidates, mDtlsParameters));
    }

    // device->load() with invalid routerRtpCapabilities throws.
    {
      final String cap = Parameters.nativeGenRouterRtpCapabilitiesExclude("mimeType");
      exceptionException(() -> device.load(cap));
    }

    // device->load() succeeds.
    {
      routerRtpCapabilities = Parameters.nativeGenRouterRtpCapabilities();
      device.load(routerRtpCapabilities);
      assertTrue(device.isLoaded());
    }

    // device->load() rejects if already loaded.
    {
      final String cap = routerRtpCapabilities;
      exceptionException(() -> device.load(cap));
    }

    // 'device->getRtpCapabilities()' succeeds".
    {
      assertFalse(TextUtils.isEmpty(device.getRtpCapabilities()));
    }

    // device->CanProduce() with 'audio'/'video' kind returns true.
    {
      assertTrue(device.canProduce("audio"));
      assertTrue(device.canProduce("video"));
    }

    // device->CanProduce() with invalid kind throws exception.
    {
      exceptionException(() -> device.canProduce("chicken"));
    }
    // "device->createSendTransport() for sending media succeeds.
    {
      String appData = "{\"baz\":\"BAZ\"}";

      sendTransportListener = new FakeTransportListener.FakeSendTransportListener();
      sendTransport =
          device.createSendTransport(
              sendTransportListener,
              mId,
              mIceParameters,
              mIceCandidates,
              mDtlsParameters,
              null,
              appData);

      assertEquals(mId, sendTransport.getId());
      assertFalse(sendTransport.isClosed());
      assertEquals(appData, sendTransport.getAppData());
      assertEquals("new", sendTransport.getConnectionState());
    }
    // device->createRecvTransport() for receiving media succeeds.
    {
      recvTransportListener = new FakeTransportListener.FakeRecvTransportListener();
      recvTransport =
          device.createRecvTransport(
              recvTransportListener, mId, mIceParameters, mIceCandidates, mDtlsParameters);

      assertEquals(mId, recvTransport.getId());
      assertFalse(recvTransport.isClosed());
      assertEquals("{}", recvTransport.getAppData());
      assertEquals("new", recvTransport.getConnectionState());
    }

    // "transport.produce() succeeds.
    {
      String appData = "{\"baz\":\"BAZ\"}";

      List<RtpParameters.Encoding> encodings = new ArrayList<>();
      encodings.add(RTCUtils.genRtpEncodingParameters(false, 0, 0, 0, 0, 0.0d, 0L));
      encodings.add(RTCUtils.genRtpEncodingParameters(false, 0, 0, 0, 0, 0.0d, 0L));
      encodings.add(RTCUtils.genRtpEncodingParameters(false, 0, 0, 0, 0, 0.0d, 0L));

      audioTrack = PeerConnectionUtils.createAudioTrack(mContext, "audio-track-id");
      assertNotEquals(0, RTCUtils.getNativeMediaStreamTrack(audioTrack));
      videoTrack = PeerConnectionUtils.createVideoTrack(mContext, "video-track-id");
      assertNotEquals(0, RTCUtils.getNativeMediaStreamTrack(videoTrack));

      // Pause the audio track before creating its Producer.
      audioTrack.setEnabled(false);

      String codecOptions = "[{\"opusStereo\":true},{\"opusDtx\":true}]";
      audioProducer =
          sendTransport.produce(producerListener, audioTrack, null, codecOptions, appData);

      assertEquals(
          ++sendTransportListener.mOnConnectExpectedTimesCalled,
          sendTransportListener.mOnConnectTimesCalled);

      assertEquals(sendTransport.getId(), sendTransportListener.mId);
      assertEquals(
          ++sendTransportListener.mOnProduceExpectedTimesCalled,
          sendTransportListener.mOnProduceTimesCalled);
      assertEquals(appData, sendTransportListener.mAppData);

      assertEquals(audioProducer.getId(), sendTransportListener.mAudioProducerId);
      assertFalse(audioProducer.isClosed());
      assertEquals("audio", audioProducer.getKind());
      assertEquals(
          RTCUtils.getNativeMediaStreamTrack(audioTrack),
          RTCUtils.getNativeMediaStreamTrack(audioProducer.getTrack()));
      assertTrue(audioProducer.isPaused());
      assertEquals(0, audioProducer.getMaxSpatialLayer());
      assertEquals(appData, audioProducer.getAppData());

      JSONObject rtpParameters = new JSONObject(audioProducer.getRtpParameters());
      assertTrue(rtpParameters.has("codecs"));
      JSONArray codecs = rtpParameters.getJSONArray("codecs");
      assertEquals(1, codecs.length());
      assertNotNull(codecs.getJSONObject(0));

      JSONArray headerExtensions = rtpParameters.getJSONArray("headerExtensions");
      assertNotNull(headerExtensions);

      JSONArray enc = rtpParameters.getJSONArray("encodings");
      assertNotNull(enc);
      assertEquals(1, enc.length());
      JSONObject firstEnc = enc.getJSONObject(0);
      assertNotNull(firstEnc);
      assertTrue(firstEnc.has("ssrc"));
      assertTrue(firstEnc.getLong("ssrc") != 0);

      JSONObject rtcp = rtpParameters.getJSONObject("rtcp");
      assertNotNull(rtcp);
      assertFalse(TextUtils.isEmpty(rtcp.getString("cname")));

      audioProducer.resume();

      videoProducer = sendTransport.produce(producerListener, videoTrack, encodings, null, null);

      assertEquals(
          sendTransportListener.mOnConnectExpectedTimesCalled,
          sendTransportListener.mOnConnectTimesCalled);
      assertEquals(
          ++sendTransportListener.mOnProduceExpectedTimesCalled,
          sendTransportListener.mOnProduceTimesCalled);
      assertEquals(videoProducer.getId(), sendTransportListener.mVideoProducerId);
      assertFalse(videoProducer.isClosed());
      assertEquals("video", videoProducer.getKind());
      assertEquals(
          RTCUtils.getNativeMediaStreamTrack(videoTrack),
          RTCUtils.getNativeMediaStreamTrack(videoProducer.getTrack()));

      rtpParameters = new JSONObject(videoProducer.getRtpParameters());
      assertTrue(rtpParameters.has("codecs"));
      codecs = rtpParameters.getJSONArray("codecs");
      assertNotEquals(0, codecs.length());
      assertNotNull(codecs.getJSONObject(0));

      headerExtensions = rtpParameters.getJSONArray("headerExtensions");
      assertNotNull(headerExtensions);

      enc = rtpParameters.getJSONArray("encodings");
      assertNotNull(enc);
      assertEquals(3, enc.length());
      firstEnc = enc.getJSONObject(0);
      assertNotNull(firstEnc);
      assertTrue(firstEnc.has("ssrc"));
      assertTrue(firstEnc.has("rtx"));
      assertTrue(firstEnc.getLong("ssrc") != 0);
      JSONObject rtx = firstEnc.getJSONObject("rtx");
      assertNotNull(rtx);
      assertTrue(rtx.has("ssrc"));
      assertTrue(rtx.getLong("ssrc") != 0);

      rtcp = rtpParameters.getJSONObject("rtcp");
      assertNotNull(rtcp);
      assertFalse(TextUtils.isEmpty(rtcp.getString("cname")));

      videoProducer.setMaxSpatialLayer(2);
      assertFalse(videoProducer.isPaused());
      assertEquals(2, videoProducer.getMaxSpatialLayer());
      assertEquals("{}", videoProducer.getAppData());
    }

    // transport.produce() without track throws.
    {
      exceptionException(() -> sendTransport.produce(producerListener, null, null, null));
    }

    // transport.consume() succeeds.
    {
      String appData = "{\"baz\":\"BAZ\"}";

      JSONObject audioConsumerRemoteParameters =
          new JSONObject(Parameters.nativeGenConsumerRemoteParameters("audio/opus"));
      JSONObject videoConsumerRemoteParameters =
          new JSONObject(Parameters.nativeGenConsumerRemoteParameters("video/VP8"));

      audioConsumer =
          recvTransport.consume(
              consumerListener,
              audioConsumerRemoteParameters.getString("id"),
              audioConsumerRemoteParameters.getString("producerId"),
              audioConsumerRemoteParameters.getString("kind"),
              audioConsumerRemoteParameters.getString("rtpParameters"),
              appData);

      assertEquals(
          ++recvTransportListener.mOnConnectExpectedTimesCalled,
          recvTransportListener.mOnConnectTimesCalled);
      assertEquals(recvTransport.getId(), recvTransportListener.mId);
      new JSONObject(recvTransportListener.mDtlsParameters);

      assertEquals(audioConsumerRemoteParameters.getString("id"), audioConsumer.getId());
      assertEquals(
          audioConsumerRemoteParameters.getString("producerId"), audioConsumer.getProducerId());
      assertFalse(audioConsumer.isClosed());
      assertEquals("audio", audioConsumer.getKind());
      assertNotNull(audioConsumer.getTrack());

      JSONObject rtpParameters = new JSONObject(audioConsumer.getRtpParameters());

      assertTrue(rtpParameters.has("codecs"));
      JSONArray codecs = rtpParameters.getJSONArray("codecs");
      assertNotNull(codecs);
      // JSONObject toString may add extra slash. replace it.
      // https://stackoverflow.com/q/13939925/2085408
      assertEquals(
          "{\"channels\":2,\"clockRate\":48000,\"mimeType\":\"audio/opus\",\"parameters\":{\"useinbandfec\":\"1\"},\"payloadType\":100,\"rtcpFeedback\":[]}",
          codecs.getJSONObject(0).toString().replace("\\", ""));

      JSONObject rtcp = rtpParameters.getJSONObject("rtcp");
      assertNotNull(rtcp);
      assertFalse(TextUtils.isEmpty(rtcp.getString("cname")));
      assertFalse(audioConsumer.isPaused());
      assertEquals(appData, audioConsumer.getAppData());

      videoConsumer =
          recvTransport.consume(
              consumerListener,
              videoConsumerRemoteParameters.getString("id"),
              videoConsumerRemoteParameters.getString("producerId"),
              videoConsumerRemoteParameters.getString("kind"),
              videoConsumerRemoteParameters.getString("rtpParameters"));

      assertEquals(
          recvTransportListener.mOnConnectExpectedTimesCalled,
          recvTransportListener.mOnConnectTimesCalled);
      assertEquals(videoConsumerRemoteParameters.getString("id"), videoConsumer.getId());
      assertEquals(
          videoConsumerRemoteParameters.getString("producerId"), videoConsumer.getProducerId());
      assertFalse(videoConsumer.isClosed());
      assertEquals("video", videoConsumer.getKind());
      assertNotNull(videoConsumer.getTrack());

      rtpParameters = new JSONObject(videoConsumer.getRtpParameters());
      assertTrue(rtpParameters.has("codecs"));
      codecs = rtpParameters.getJSONArray("codecs");
      assertNotNull(codecs);
      assertEquals(2, codecs.length());
      assertEquals(
          "{\"clockRate\":90000,\"mimeType\":\"video/VP8\",\"parameters\":{\"x-google-start-bitrate\":\"1500\"},\"payloadType\":101,\"rtcpFeedback\":[{\"type\":\"nack\"},{\"parameter\":\"pli\",\"type\":\"nack\"},{\"parameter\":\"sli\",\"type\":\"nack\"},{\"parameter\":\"rpsi\",\"type\":\"nack\"},{\"parameter\":\"app\",\"type\":\"nack\"},{\"parameter\":\"fir\",\"type\":\"ccm\"},{\"type\":\"goog-remb\"}]}",
          codecs.getJSONObject(0).toString().replace("\\", ""));

      assertEquals(
          "{\"clockRate\":90000,\"mimeType\":\"video/rtx\",\"parameters\":{\"apt\":\"101\"},\"payloadType\":102,\"rtcpFeedback\":[]}",
          codecs.getJSONObject(1).toString().replace("\\", ""));

      JSONArray headerExtensions = rtpParameters.getJSONArray("headerExtensions");
      assertEquals(
          "[{\"id\":2,\"uri\":\"urn:ietf:params:rtp-hdrext:toffset\"},{\"id\":3,\"uri\":\"http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time\"}]",
          headerExtensions.toString().replace("\\", ""));

      JSONArray enc = rtpParameters.getJSONArray("encodings");
      assertNotNull(enc);
      assertEquals(1, enc.length());
      JSONObject firstEnc = enc.getJSONObject(0);
      assertNotNull(firstEnc);
      assertTrue(firstEnc.has("ssrc"));
      assertTrue(firstEnc.has("rtx"));
      assertTrue(firstEnc.getLong("ssrc") != 0);
      JSONObject rtx = firstEnc.getJSONObject("rtx");
      assertNotNull(rtx);
      assertTrue(rtx.has("ssrc"));
      assertTrue(rtx.getLong("ssrc") != 0);

      rtcp = rtpParameters.getJSONObject("rtcp");
      assertNotNull(rtcp);
      assertFalse(TextUtils.isEmpty(rtcp.getString("cname")));

      assertFalse(videoConsumer.isPaused());
      assertEquals("{}", videoConsumer.getAppData());
    }

    // transport.consume() with unsupported consumerRtpParameters throws.
    {
      JSONObject consumerRemoteParameters =
          new JSONObject(Parameters.nativeGenConsumerRemoteParameters("audio/ISAC"));
      exceptionException(
          () -> {
            try {
              recvTransport.consume(
                  consumerListener,
                  consumerRemoteParameters.getString("id"),
                  consumerRemoteParameters.getString("producerId"),
                  consumerRemoteParameters.getString("kind"),
                  consumerRemoteParameters.getString("rtpParameters"));
            } catch (JSONException e) {
              e.printStackTrace();
            }
          });
    }

    // 'sendTransport.GetStats()' succeeds.
    {
      assertFalse(TextUtils.isEmpty(sendTransport.getStats()));
    }

    // sendTransport.RestartIce()' succeeds.
    {
      sendTransport.restartIce(mIceParameters);
    }

    // 'sendTransport.UpdateIceServers()' succeeds.
    {
      sendTransport.updateIceServers(new JSONArray().toString());
    }

    // 'producer->Pause()' succeeds.
    {
      videoProducer.pause();
      assertTrue(videoProducer.isPaused());
    }

    // 'producer->Resume()' succeeds.
    {
      videoProducer.resume();
      assertFalse(videoProducer.isPaused());
    }

    // 'producer->ReplaceTrack()' succeeds.
    {
      // Have the audio Producer paused.
      audioProducer.pause();
      AudioTrack newAudioTrack = PeerConnectionUtils.createAudioTrack(mContext, "audio-track-id-2");
      audioProducer.replaceTrack(newAudioTrack);
      assertEquals(
          RTCUtils.getNativeMediaStreamTrack(newAudioTrack),
          RTCUtils.getNativeMediaStreamTrack(audioProducer.getTrack()));

      // Producer was already paused.
      assertTrue(audioProducer.isPaused());

      // Reset the audio paused state.
      audioProducer.resume();

      VideoTrack newVideoTrack = PeerConnectionUtils.createVideoTrack(mContext, "video-track-id-2");
      videoProducer.replaceTrack(newVideoTrack);
      assertEquals(
          RTCUtils.getNativeMediaStreamTrack(newVideoTrack),
          RTCUtils.getNativeMediaStreamTrack(videoProducer.getTrack()));
      assertFalse(videoProducer.isPaused());

      videoTrack.dispose();
      videoTrack = newVideoTrack;
    }

    // 'producer->ReplaceTrack()' fails if null track is provided.
    {
      exceptionException(() -> videoProducer.replaceTrack(null));
    }

    // 'producer->SetMaxSpatialLayer()' succeeds.
    {
      videoProducer.setMaxSpatialLayer(1);
      assertEquals(1, videoProducer.getMaxSpatialLayer());
    }

    // 'producer->SetMaxSpatialLayer()' in an audio Producer throws.
    {
      exceptionException(() -> audioProducer.setMaxSpatialLayer(1));
    }

    // 'producer->GetStats()' succeeds.
    {
      assertFalse(TextUtils.isEmpty(videoProducer.getStats()));
    }

    // 'consumer->Resume()' succeeds.
    {
      videoConsumer.resume();
      assertFalse(videoConsumer.isPaused());
    }

    // 'consumer->Pause()' succeeds.
    {
      videoConsumer.pause();
      assertTrue(videoConsumer.isPaused());
    }

    // 'consumer->GetStats()' succeeds.
    {
      assertFalse(TextUtils.isEmpty(videoConsumer.getStats()));
    }

    // 'producer->Close()' succeeds.
    {
      audioProducer.close();
      assertTrue(audioProducer.isClosed());
    }

    // producer->getStats() throws if closed.
    {
      exceptionException(() -> audioProducer.getStats());
    }

    // consumer->Close()' succeeds.
    {
      audioConsumer.close();
      assertTrue(audioConsumer.isClosed());
    }

    // consumer->getStats() throws if closed.
    {
      exceptionException(() -> audioConsumer.getStats());
    }

    // transport->Close() fires 'OnTransportClose' in live Producers/Consumers.
    {
      // Audio Producer was already closed.
      assertTrue(audioProducer.isClosed());
      assertFalse(videoProducer.isClosed());

      sendTransport.close();
      assertTrue(sendTransport.isClosed());
      assertTrue(videoProducer.isClosed());
      // Audio Producer was already closed.
      assertEquals(
          ++producerListener.mOnTransportCloseExpetecTimesCalled,
          producerListener.mOnTransportCloseTimesCalled);

      // Audio Consumer was already closed.
      assertTrue(audioConsumer.isClosed());
      assertFalse(videoConsumer.isClosed());

      recvTransport.close();
      assertTrue(recvTransport.isClosed());
      assertTrue(videoConsumer.isClosed());
      // Audio Producer was already closed.
      assertEquals(
          ++consumerListener.mOnTransportCloseExpetecTimesCalled,
          consumerListener.mOnTransportCloseTimesCalled);
    }

    // transport.produce() throws if closed.
    {
      exceptionException(() -> sendTransport.produce(producerListener, audioTrack, null, null));
    }

    // transport.consume() throws if closed.
    {
      final JSONObject audioConsumerRemoteParameters =
          new JSONObject(Parameters.nativeGenConsumerRemoteParameters("audio/opus"));
      exceptionException(
          () -> {
            try {
              recvTransport.consume(
                  consumerListener,
                  audioConsumerRemoteParameters.getString("id"),
                  audioConsumerRemoteParameters.getString("producerId"),
                  audioConsumerRemoteParameters.getString("kind"),
                  audioConsumerRemoteParameters.getString("rtpParameters"));
            } catch (JSONException e) {
              e.printStackTrace();
            }
          });
    }

    // transport.getStats() throws if closed.
    {
      exceptionException(() -> sendTransport.getStats());
    }

    // transport.restartIce() throws if closed".
    {
      exceptionException(() -> sendTransport.restartIce(null));
    }

    // transport.updateIceServers() throws if closed".
    {
      exceptionException(() -> sendTransport.updateIceServers(null));
    }

    // dispose.
    sendTransport.close();
    sendTransport.dispose();
    recvTransport.close();
    recvTransport.dispose();
    device.dispose();
    audioTrack.dispose();
    videoTrack.dispose();
  }

  @Test
  public void version() {
    assertEquals(MediasoupClient.version(), BuildConfig.VERSION_NAME);
  }
}
