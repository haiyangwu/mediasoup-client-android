#!/usr/bin/env bash

set -e

PROJECT_PWD=${PWD}
DEP=$1

current_dir_name=${PROJECT_PWD##*/}
if [ "${current_dir_name}" != "mediasoup-client" ] ; then
	echo ">>> [ERROR] $(basename $0) must be called from mediasoup-client/ root directory" >&2
	exit 1
fi

function get_dep()
{
	GIT_REPO="$1"
	GIT_TAG="$2"
	DEST="$3"

	echo ">>> [INFO] getting dep '${DEP}' ..."

	if [ -d "${DEST}" ] ; then
		echo ">>> [INFO] deleting ${DEST} ..."
		git rm -rf --ignore-unmatch ${DEST} > /dev/null
		rm -rf ${DEST}
	fi

	echo ">>> [INFO] cloning ${GIT_REPO} ..."
	git clone ${GIT_REPO} ${DEST}

	cd ${DEST}

		if [ -z '${GIT_TAG}' ]
		then
				echo ">>> [INFO] setting '${GIT_TAG}' git tag ..."
				git checkout --quiet ${GIT_TAG}
				set -e
		fi

	echo ">>> [INFO] adding dep source code to the repository ..."
	rm -rf .git
	git add .
	git add .

	echo ">>> [INFO] got dep '${DEP}'"

	cd ${PROJECT_PWD}
}

function get_libmediasoupclient()
{
	GIT_REPO="https://github.com/versatica/libmediasoupclient.git"
	GIT_TAG="3.4.0"
	DEST="deps/libmediasoupclient"

	get_dep "${GIT_REPO}" "${GIT_TAG}" "${DEST}"
}

function get_webrtc()
{
	GIT_REPO="-b m94 --depth=1 https://github.com/haiyangwu/webrtc-mirror.git"
	DEST="deps/webrtc/src"

	get_dep "${GIT_REPO}" "${GIT_TAG}" "${DEST}"
}

function get_abseil-cpp()
{
	GIT_REPO="https://github.com/abseil/abseil-cpp.git"
	GIT_TAG="8e088c5f3c290c5ac53dd5010fd501d80b483115"
	DEST="deps/webrtc/src/third_party/abseil-cpp"

	get_dep "${GIT_REPO}" "${GIT_TAG}" "${DEST}"
}

function get_webrtc-libs()
{
	GIT_REPO="https://github.com/haiyangwu/webrtc-android-build.git"
	GIT_TAG="m94"
	DEST="deps/webrtc/lib"

	get_dep "${GIT_REPO}" "${GIT_TAG}" "${DEST}"
}

case "${DEP}" in
	'-h')
		echo "Usage:"
		echo "	./scripts/$(basename $0) [libmediasoupclient|webrtc|abseil-cpp|webrtc-libs]"
		echo
		;;
	libmediasoupclient)
		get_libmediasoupclient
		;;
	webrtc)
		get_webrtc
		;;
	abseil-cpp)
		get_abseil-cpp
		;;
	webrtc-libs)
		get_webrtc-libs
		;;
	*)
	echo ">>> [ERROR] unknown dep '${DEP}'" >&2
	exit 1
esac

if [ $? -eq 0 ] ; then
	echo ">>> [INFO] done"
else
	echo ">>> [ERROR] failed" >&2
	exit 1
fi
