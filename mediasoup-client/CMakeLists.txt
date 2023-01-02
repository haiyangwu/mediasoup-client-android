cmake_minimum_required(VERSION 3.5)

project(mediasoupclient_so LANGUAGES CXX)

# Set version number.
set(mediasoup_client_so_VERSION_MAJOR 3)
set(mediasoup_client_so_VERSION_MINOR 0)
set(mediasoup_client_so_VERSION_PATCH 8)

# C++ standard requirements.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(
	SOURCE_DIR
	${mediasoupclient_so_SOURCE_DIR}/src/main/jni/
)

set(
	SOURCE_FILES
	${SOURCE_DIR}/jni_generator_helper.cpp
	${SOURCE_DIR}/jni_onload.cpp
	${SOURCE_DIR}/scoped_java_ref.cpp
	${SOURCE_DIR}/java_types.cpp
	${SOURCE_DIR}/consumer_jni.cpp
	${SOURCE_DIR}/data_consumer_jni.cpp
	${SOURCE_DIR}/data_producer_jni.cpp
	${SOURCE_DIR}/device_jni.cpp
	${SOURCE_DIR}/logger_jni.cpp
	${SOURCE_DIR}/mediasoup_client_jni.cpp
	${SOURCE_DIR}/peerconnection_jni.cpp
	${SOURCE_DIR}/producer_jni.cpp
	${SOURCE_DIR}/transport_jni.cpp
)

# TODO(haiyang wu): add condition
list(APPEND SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/androidTest/jni/parameters_jni.cpp)
list(APPEND SOURCE_FILES ${PROJECT_SOURCE_DIR}/deps/libmediasoupclient/test/src/fakeParameters.cpp)

# Create target.
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

if(${MEDIASOUPCLIENT_LOG_TRACE})
	target_compile_definitions(${PROJECT_NAME}
		PRIVATE MSC_LOG_TRACE=1
	)
endif()

if(${MEDIASOUPCLIENT_LOG_DEV})
	target_compile_definitions(${PROJECT_NAME}
		PRIVATE MSC_LOG_DEV=1
	)
endif()

# Source Dependencies. override LIBWEBRTC_BINARY_PATH
# TODO(HaiyangWu) add libwebrtc.a without change CMakeLists in libmediasoupclient
# set(LIBWEBRTC_BINARY_PATH ${LIBWEBRTC_BINARY_ANDROID_PATH}/${ANDROID_ABI} CACHE STRING "libwebrtc binary path" FORCE)
add_subdirectory("deps/libmediasoupclient")

# Add some compile flags to our source files.
set_source_files_properties(${SOURCE_FILES}
	PROPERTIES COMPILE_FLAGS -Wall -Wextra -Wpedantic)

include_directories(SYSTEM "${LIBWEBRTC_INCLUDE_PATH}/third_party/abseil-cpp")

# Private (implementation) header files.
target_include_directories(${PROJECT_NAME} PUBLIC
	"${SOURCE_DIR}/"
	"${SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/deps/libmediasoupclient/include"
	"${PROJECT_SOURCE_DIR}/deps/libmediasoupclient/test/include"
	"${PROJECT_SOURCE_DIR}/deps/libmediasoupclient/deps/libsdptransform/include"
	"${LIBWEBRTC_INCLUDE_PATH}"
	"${LIBWEBRTC_INCLUDE_PATH}/third_party/abseil-cpp"
)

# Compile definitions for libwebrtc.
target_compile_definitions(${PROJECT_NAME} PUBLIC
	$<$<NOT:$<PLATFORM_ID:Windows>>:WEBRTC_POSIX>
	$<$<PLATFORM_ID:Windows>:WEBRTC_WIN>
	$<$<PLATFORM_ID:Windows>:NOMINMAX>
	$<$<PLATFORM_ID:Windows>:WIN32_LEAN_AND_MEAN>
	$<$<PLATFORM_ID:Darwin>:WEBRTC_MAC>
)

# Public (interface) dependencies.
target_link_libraries(${PROJECT_NAME} PUBLIC
	android
	log
	OpenSLES
	mediasoupclient
)
