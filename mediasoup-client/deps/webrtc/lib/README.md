# webrtc-android-build
Precompiled binaries for webrtc android native

* Build m79 with command like below

```
./tools_webrtc/android/build_aar.py --extra-gn-args 'is_debug=false is_component_build=false is_clang=true rtc_include_tests=false rtc_use_h264=true rtc_enable_protobuf=false use_rtti=true use_custom_libcxx=false' --build-dir ./out/release-build/
```