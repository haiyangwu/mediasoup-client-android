# mediasoup-client-android

mediasoup android client side library https://mediasoup.org

## Getting Started
_(Coming soon)_

## Documentation
_(Coming soon)_

## Building Native Android WebRTC
**WebRTC Android can only be built using Linux, Recommended OS is Ubuntu**

### Setup Depot Tools

```bash
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git

# Preferrably but the below in your shell file
export PATH=$PATH:/path/to/depot_tools
```

### Install Build Dependencies (Ubuntu)

```bash
cd /path/to/depot_tools
./build/install-build-deps.sh
./build/install-build-deps-android.sh
```

### Build Native WebRTC Libraries

```bash
mkdir webrtc-android-checkout
cd webrtc-android-checkout
fetch --nohooks webrtc_android
gclient sync
cd src

# Checkout branch m74
git checkout -b m74 refs/remotes/branch-heads/m74
gclient sync

# Accept Android SDK Licences
yes | ./third_party/android_tools/sdk/tools/bin/sdkmanager --licenses

# Apply the path located below

# Build Android WebRTC
# Recommended to use the below command to generate libs for all supported arch:
./tools_webrtc/android/build_aar.py

# Use the below to build for only a certain lib(change the target_cpu)
gn gen out/android-webrtc-m74 --args='target_os = "android" target_cpu = "x86" is_debug = false rtc_include_tests = false rtc_use_h264 = true proprietary_codecs = true ffmpeg_branding = "Chrome" use_openh264 = true is_component_build = false is_clang = true rtc_enable_protobuf = false use_rtti = true use_custom_libcxx = false'
ninja -C out/android-webrtc-m74

# Move libwebrtc to x86 directory (needed for android build)
mkdir -p out/android-webrtc-m74/x86/obj
cp out/android-webrtc-m74/obj/libwebrtc.a out/android-webrtc-m74/x86/obj/
```

### Note
* Build webrtc m74 with this [patch](https://gist.github.com/Piasy/4effa9057eb0faff8231d34e589478c3)

### Reference Native WebRTC Libraries
* local.properties

```
// Path to the checkout webrtc source folder
webrtc.include.path=/path/to/webrtc/src/directory
webrtc.binary.path=/path/to/built/x86/directory
webrtc.jar.path=/path/to/build/lib.java/sdk/directory
```

## Release History
_(Nothing yet)_

## License
Copyright (c) 2019 Haiyang Wu  
Licensed under the MIT license.
