package org.mediasoup.droid;

import android.content.Context;
import android.util.Log;

import org.webrtc.AudioSource;
import org.webrtc.AudioTrack;
import org.webrtc.DefaultVideoDecoderFactory;
import org.webrtc.DefaultVideoEncoderFactory;
import org.webrtc.EglBase;
import org.webrtc.MediaConstraints;
import org.webrtc.PeerConnectionFactory;
import org.webrtc.VideoDecoderFactory;
import org.webrtc.VideoEncoderFactory;
import org.webrtc.VideoSource;
import org.webrtc.VideoTrack;
import org.webrtc.audio.AudioDeviceModule;
import org.webrtc.audio.JavaAudioDeviceModule;

public class PeerConnectionUtils {

  private static final String TAG = "PeerConnectionUtils";
  private static PeerConnectionFactory mPeerConnectionFactory;

  private static AudioSource mAudioSource;
  private static VideoSource mVideoSource;

  // PeerConnection factory creation.
  private static void createPeerConnectionFactory(Context context) {
    Logger.d(TAG, "createPeerConnectionFactory()");
    PeerConnectionFactory.Builder builder = PeerConnectionFactory.builder();
    builder.setOptions(null);

    EglBase eglBase = EglBase.create();

    AudioDeviceModule adm = createJavaAudioDevice(context);
    VideoEncoderFactory encoderFactory =
        new DefaultVideoEncoderFactory(
            eglBase.getEglBaseContext(), true /* enableIntelVp8Encoder */, true);
    VideoDecoderFactory decoderFactory =
        new DefaultVideoDecoderFactory(eglBase.getEglBaseContext());

    mPeerConnectionFactory =
        builder
            .setAudioDeviceModule(adm)
            .setVideoEncoderFactory(encoderFactory)
            .setVideoDecoderFactory(decoderFactory)
            .createPeerConnectionFactory();
  }

  static AudioDeviceModule createJavaAudioDevice(Context appContext) {
    // Enable/disable OpenSL ES playback.
    // Set audio record error callbacks.
    JavaAudioDeviceModule.AudioRecordErrorCallback audioRecordErrorCallback =
        new JavaAudioDeviceModule.AudioRecordErrorCallback() {
          @Override
          public void onWebRtcAudioRecordInitError(String errorMessage) {
            Logger.e(TAG, "onWebRtcAudioRecordInitError: " + errorMessage);
          }

          @Override
          public void onWebRtcAudioRecordStartError(
              JavaAudioDeviceModule.AudioRecordStartErrorCode errorCode, String errorMessage) {
            Logger.e(TAG, "onWebRtcAudioRecordStartError: " + errorCode + ". " + errorMessage);
          }

          @Override
          public void onWebRtcAudioRecordError(String errorMessage) {
            Logger.e(TAG, "onWebRtcAudioRecordError: " + errorMessage);
          }
        };

    JavaAudioDeviceModule.AudioTrackErrorCallback audioTrackErrorCallback =
        new JavaAudioDeviceModule.AudioTrackErrorCallback() {
          @Override
          public void onWebRtcAudioTrackInitError(String errorMessage) {
            Log.e(TAG, "onWebRtcAudioTrackInitError: " + errorMessage);
          }

          @Override
          public void onWebRtcAudioTrackStartError(
              JavaAudioDeviceModule.AudioTrackStartErrorCode errorCode, String errorMessage) {
            Log.e(TAG, "onWebRtcAudioTrackStartError: " + errorCode + ". " + errorMessage);
          }

          @Override
          public void onWebRtcAudioTrackError(String errorMessage) {
            Log.e(TAG, "onWebRtcAudioTrackError: " + errorMessage);
          }
        };

    return JavaAudioDeviceModule.builder(appContext)
        .setAudioRecordErrorCallback(audioRecordErrorCallback)
        .setAudioTrackErrorCallback(audioTrackErrorCallback)
        .createAudioDeviceModule();
  }

  public static PeerConnectionFactory getPeerConnectionFactory(Context context) {
    if (mPeerConnectionFactory == null) {
      createPeerConnectionFactory(context);
    }
    return mPeerConnectionFactory;
  }

  // Audio source creation.
  private static void createAudioSource(Context context) {
    Logger.d(TAG, "createAudioSource()");
    if (mPeerConnectionFactory == null) {
      createPeerConnectionFactory(context);
    }

    mAudioSource = mPeerConnectionFactory.createAudioSource(new MediaConstraints());
  }

  // Video source creation.
  private static void createVideoSource(Context context) {
    if (mPeerConnectionFactory == null) {
      createPeerConnectionFactory(context);
    }

    mVideoSource = mPeerConnectionFactory.createVideoSource(false);
  }

  // Audio track creation.
  public static AudioTrack createAudioTrack(Context context, String id) {
    Logger.d(TAG, "createAudioTrack()");
    if (mAudioSource == null) {
      createAudioSource(context);
    }
    Logger.d(TAG, mAudioSource.toString());
    return mPeerConnectionFactory.createAudioTrack(id, mAudioSource);
  }

  // Video track creation.
  public static VideoTrack createVideoTrack(Context context, String id) {
    if (mVideoSource == null) {
      createVideoSource(context);
    }

    return mPeerConnectionFactory.createVideoTrack(id, mVideoSource);
  }
}
