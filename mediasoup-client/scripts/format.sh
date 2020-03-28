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

# Run clang-format -i on 'include' and 'src' folders.
for dir in "src/main/jni/* src/main/jni/include/* src/androidTest/jni"; do
    find ${dir} \
         -maxdepth 1 \
         \( -name '*.cpp' \
         -o -name '*.h' \) \
         -exec 'clang-format' -i '{}' \;
    popd &>/dev/null
done
