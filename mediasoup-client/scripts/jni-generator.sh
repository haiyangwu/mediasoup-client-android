#!/usr/bin/env bash

set -e

if [ "$(basename "$PWD")" != "mediasoup-client" ] ; then
	echo ">>> [ERROR] $(basename "$0") must be called from mediasoup-client/ root directory" >&2
	exit 1
fi

function jni_generator()
{
	local input_files output_files

	namespace="$1"
	output_dir="$2"
	shift 2

	for f in "$@"; do
		input_files+=("--input_file")
		input_files+=("$f")
		output_files+=("--output_file")
		output_files+=("$output_dir/$(basename "$f" .java)_jni.h")
	done

	./deps/webrtc/src/base/android/jni_generator/jni_generator.py \
		--ptr_type=long  \
		--includes ../include/jni_generator_helper.h \
		"${input_files[@]}" \
		"${output_files[@]}" \
		-n "$namespace"
}

jni_generator mediasoupclient ./src/main/jni/generated_mediasoupclient_jni/jni/ \
		"./src/main/java/org/mediasoup/droid/Consumer.java" \
		"./src/main/java/org/mediasoup/droid/Device.java" \
		"./src/main/java/org/mediasoup/droid/PeerConnection.java" \
		"./src/main/java/org/mediasoup/droid/Producer.java" \
		"./src/main/java/org/mediasoup/droid/RecvTransport.java" \
		"./src/main/java/org/mediasoup/droid/SendTransport.java" \
		"./src/main/java/org/mediasoup/droid/Transport.java" \
		"./src/main/java/org/mediasoup/droid/Logger.java" \
		"./src/main/java/org/mediasoup/droid/MediasoupClient.java"

jni_generator mediasoupclient::test ./src/androidTest/jni/generated_mediasoupclient_jni/jni/ \
		"./src/androidTest/java/org/mediasoup/droid/data/Parameters.java"