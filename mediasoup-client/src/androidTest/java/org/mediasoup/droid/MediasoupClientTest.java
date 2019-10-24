package org.mediasoup.droid;

import android.support.test.runner.AndroidJUnit4;
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
import static org.mediasoup.droid.data.Parameters.generateTransportRemoteParameters;

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
      JSONObject transportRemoteParameters = new JSONObject(generateTransportRemoteParameters());
      mId = transportRemoteParameters.getString("id");
      mIceParameters = transportRemoteParameters.getString("iceParameters");
      mIceCandidates = transportRemoteParameters.getString("iceCandidates");
      mDtlsParameters = transportRemoteParameters.getString("dtlsParameters");
    } catch (JSONException e) {
      e.printStackTrace();
    }
  }

  @Test
  public void mediasoupclient() throws JSONException {
    Device device;
    String routerRtpCapabilities;

    FakeTransportListener.FakeSendTransportListener sendTransportListener;
    FakeTransportListener.FakeRecvTransportListener recvTransportListener;

    SendTransport sendTransport;
    RecvTransport recvTransport;

    AudioTrack audioTrack;
    VideoTrack videoTrack;

    Producer audioProducer;
    Producer videoProducer;

    // create a Device succeeds.
    {
      device = new Device();
      assertFalse(device.isLoaded());
    }

    // "device->GetRtpCapabilities throws if not loaded.
    {
      exceptionException(device::GetRtpCapabilities);
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
      final String cap = Parameters.generateRouterRtpCapabilitiesExclude("mimeType");
      exceptionException(() -> device.load(cap));
    }

    // device->load() succeeds.
    {
      routerRtpCapabilities = Parameters.generateRouterRtpCapabilities();
      device.load(routerRtpCapabilities);
      assertTrue(device.isLoaded());
    }

    // device->load() rejects if already loaded.
    {
      final String cap = routerRtpCapabilities;
      exceptionException(() -> device.load(cap));
    }

    // 'device->GetRtpCapabilities()' succeeds".
    {
      assertFalse(TextUtils.isEmpty(device.GetRtpCapabilities()));
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
      final FakeTransportListener.FakeProducerListener producerListener =
          new FakeTransportListener.FakeProducerListener();
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
      assertEquals(RTCUtils.getNativeMediaStreamTrack(audioTrack), audioProducer.getTrack());
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
      assertEquals(RTCUtils.getNativeMediaStreamTrack(videoTrack), videoProducer.getTrack());

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
      // TODO:
    }

    // transport.consume() succeeds.
    {
      // TODO:
    }

    // transport.consume() with unsupported consumerRtpParameters throws.
    {
      // TODO:
    }

    // 'sendTransport.GetStats()' succeeds.
    {
      // TODO:
    }

    // sendTransport.RestartIce()' succeeds.
    {
      // TODO:
    }

    // 'sendTransport.UpdateIceServers()' succeeds.
    {
      // TODO:
    }

    // 'producer->Pause()' succeeds.
    {
      // TODO:
    }

    // 'producer->Resume()' succeeds.
    {
      // TODO:
    }

    // 'producer->ReplaceTrack()' succeeds.
    {
      // TODO:
    }

    // 'producer->ReplaceTrack()' fails if null track is provided.
    {
      // TODO:
    }

    // 'producer->SetMaxSpatialLayer()' succeeds.
    {
      // TODO:
    }

    // 'producer->SetMaxSpatialLayer()' in an audio Producer throws.
    {
      // TODO:
    }

    // 'producer->GetStats()' succeeds.
    {
      // TODO:
    }

    // 'consumer->Resume()' succeeds.
    {
      // TODO:
    }

    // 'consumer->Pause()' succeeds.
    {
      // TODO:
    }

    // 'consumer->GetStats()' succeeds.
    {
      // TODO:
    }

    // 'producer->Close()' succeeds.
    {
      // TODO:
    }

    // producer->getStats() throws if closed.
    {
      // TODO:
    }

    // consumer->Close()' succeeds.
    {
      // TODO:
    }

    // consumer->getStats() throws if closed.
    {
      // TODO:
    }

    // transport->Close() fires 'OnTransportClose' in live Producers/Consumers.
    {
      // TODO:
    }

    // transport.produce() throws if closed.
    {
      // TODO:
    }

    // transport.consume() throws if closed.
    {
      // TODO:
    }

    // transport.getStats() throws if closed.
    {
      // TODO:
    }

    // transport.restartIce() throws if closed".
    {
      // TODO:
    }

    // transport.restartIce() throws if closed".
    {
      // TODO:
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
