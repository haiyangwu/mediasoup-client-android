# mediasoup-client-android

[![Bintray][bintray-shield-mediasoup-client-android]][bintray-mediasoup-client-android]
[![Codacy Badge][codacy-grade-shield-mediasoup-client-android]][codacy-grade-mediasoup-client-android]

mediasoup android client side library https://mediasoup.org

## Getting Started

### Setting up the dependency

Include `mediasoup-client-android`  into your project, for example, as a Gradle compile dependency:

```groovy
implementation 'org.mediasoup.droid:mediasoup-client:3.0.8-beta-3'
```
### Example

* Initialize `MediasoupClient`
```Java
MediasoupClient.initialize(getApplicationContext());
```

* Create `Device` and load `routerRtpCapabilities`

```Java
Device mMediasoupDevice = new Device();
// ...
// routerRtpCapabilities, the response of request `getRouterRtpCapabilities` from mediasoup-demo server
mMediasoupDevice.load(routerRtpCapabilities)
```

* Create `SendTransport` and produce `MediaStreamTrack`
```Java
// ...
// res, the response of request `createWebRtcTransport` from mediasoup-demo server
JSONObject info = new JSONObject(res);
String id = info.optString("id");
String iceParameters = info.optString("iceParameters");
String iceCandidates = info.optString("iceCandidates");
String dtlsParameters = info.optString("dtlsParameters");
String sctpParameters = info.optString("sctpParameters");

SendTransport mSendTransport =
  mMediasoupDevice.createSendTransport(
  sendTransportListener, id, iceParameters, iceCandidates, dtlsParameters);
// ...
// mLocalVideoTrack, created by `org.webrtc.PeerConnectionFactory`
Producer mCamProducer =
  mSendTransport.produce(
  producer -> {
    Logger.e(TAG, "onTransportClose(), camProducer");
  },
  mLocalVideoTrack, null, null);
```

* Create `RecvTransport` and consume `MediaStreamTrack`
```Java
// ...
// res, the response of request `createWebRtcTransport` from mediasoup-demo server
JSONObject info = new JSONObject(res);
String id = info.optString("id");
String iceParameters = info.optString("iceParameters");
String iceCandidates = info.optString("iceCandidates");
String dtlsParameters = info.optString("dtlsParameters");
String sctpParameters = info.optString("sctpParameters");

RecvTransport mRecvTransport =
  mMediasoupDevice.createRecvTransport(
  recvTransportListener, id, iceParameters, iceCandidates, dtlsParameters);

// ...
// request, the request `newConsumer` from mediasoup-demo server
JSONObject data = request.getData();
String peerId = data.optString("peerId");
String producerId = data.optString("producerId");
String id = data.optString("id");
String kind = data.optString("kind");
String rtpParameters = data.optString("rtpParameters");
String type = data.optString("type");
String appData = data.optString("appData");
Consumer consumer =
  mRecvTransport.consume(
  c -> {
    Logger.w(TAG, "onTransportClose for consume");
  },
  id, producerId, kind, rtpParameters, appData);
```
## Dependencies

* [libmediasoupclient][libmediasoupclient] mediasoup client side C++ library which supported by mediasoup team
* [webrtc-android-build][webrtc-android-build] my personal `webrtc` android prebuilt static library which may be compiled with some patch 


## Demo Project

[mediasoup-demo-android][mediasoup-demo-android]

## TODO
* [X] sync to the latest version of [libmediasoupclient][libmediasoupclient] and adapter to `webrtc` m79
* [ ] support `DataProducer` etc
* [ ] add convenient `Gradle Tasks` for sync, compile and so on
* [X] add script to use `generate-jni` to automatic generate `xxx-jni.h
* [ ] add java MediaSoupClientException mapping to `MediaSoupClientErrors.hpp
* [ ] document


## Bugs and Feedback

For bugs, feature requests, and discussion please use [GitHub Issues][issues].

## Author
Haiyang Wu([@haiyangwu](https://github.com/haiyangwu/) at Github)

## License
[MIT](./LICENSE)





[bintray-mediasoup-client-android]: https://mvnrepository.com/artifact/org.mediasoup.droid/mediasoup-client
[bintray-shield-mediasoup-client-android]: https://img.shields.io/bintray/v/haiyangwu/maven/mediasoup-client

[codacy-grade-shield-mediasoup-client-android]: https://api.codacy.com/project/badge/Grade/506c48c5457b462a82afe786762956dc
[codacy-grade-mediasoup-client-android]: https://app.codacy.com/manual/haiyangwu/mediasoup-client-android?utm_source=github.com&utm_medium=referral&utm_content=haiyangwu/mediasoup-client-android&utm_campaign=Badge_Grade_Dashboard

[libmediasoupclient]: https://github.com/versatica/libmediasoupclient
[webrtc-android-build]: https://github.com/haiyangwu/webrtc-android-build

[mediasoup-demo-android]: https://github.com/haiyangwu/mediasoup-demo-android
[issues]: https://github.com/haiyangwu/mediasoup-client-android/issues
