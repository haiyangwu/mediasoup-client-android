#!/usr/bin/env bash

set -e

PROJECT_PWD=${PWD}

# Import utils // OS, NUM_CORES
. scripts/common.sh

if [ "${OS}" != "Darwin" ] && [ "${OS}" != "Linux" ] ; then
	echo "Only available for MacOS and Linux"
	exit 1;
fi

current_dir_name=${PROJECT_PWD##*/}
if [ "${current_dir_name}" != "mediasoup-client" ] ; then
	echo ">>> [ERROR] $(basename $0) must be called from mediasoup-client/ root directory" >&2
	exit 1
fi

function jni_generator()
{
	file="$1"
	namespace="$2"
	python  ./deps/webrtc/src/base/android/jni_generator/jni_generator.py \
    --input_file=${file} \
    --ptr_type=long  \
    --includes ../include/jni_generator_helper.h \
    --output_dir ./src/main/jni/generated_mediasoupclient_jni/jni \
    -n ${namespace}
}

jni_generator "./src/main/java/org/mediasoup/droid/Consumer.java" mediasoupclient
jni_generator "./src/main/java/org/mediasoup/droid/Device.java" mediasoupclient
jni_generator "./src/main/java/org/mediasoup/droid/PeerConnection.java" mediasoupclient
jni_generator "./src/main/java/org/mediasoup/droid/Producer.java" mediasoupclient
jni_generator "./src/main/java/org/mediasoup/droid/RecvTransport.java" mediasoupclient
jni_generator "./src/main/java/org/mediasoup/droid/SendTransport.java" mediasoupclient
jni_generator "./src/main/java/org/mediasoup/droid/Transport.java" mediasoupclient
jni_generator "./src/main/java/org/mediasoup/droid/Logger.java" mediasoupclient
jni_generator "./src/main/java/org/mediasoup/droid/MediasoupClient.java" mediasoupclient
jni_generator "./src/androidTest/java/org/mediasoup/droid/data/Parameters.java" mediasoupclient::test