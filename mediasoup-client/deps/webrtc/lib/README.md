# webrtc-android-build

Precompiled binaries for webrtc android native

## Build m94 with command like below

```shell
$ ./tools_webrtc/android/build_aar.py --extra-gn-args 'is_debug=false is_component_build=false is_clang=true rtc_include_tests=false rtc_use_h264=true rtc_enable_protobuf=false use_rtti=true use_custom_libcxx=false' --build-dir ./out/release-build/
```

## To build `libwebrtc.a` for each ABI

* armeabi-v7a
```shell
$ autoninja -C ../out/release-build/armeabi-v7a webrtc
```

* arm64-v8a
```shell
$ autoninja -C ../out/release-build/arm64-v8a webrtc
```

* x86

```shell
$ autoninja -C ../out/release-build/x86 webrtc
```

* x86_64

```shell
$ autoninja -C ../out/release-build/x86_64 webrtc
```
