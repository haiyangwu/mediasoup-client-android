cmake_minimum_required(VERSION 3.0)

project(sdptransform VERSION 1.2.4)

# For CMake >= 3.1.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# For CMake < 3.1.
add_compile_options(-std=c++11)

# TODO(haiyangwu): new pull request.
# Project options.
option(SDPTRANSFORM_BUILD_TESTS "Build unit tests" ON)
option(SDPTRANSFORM_BUILD_READMEHELPER "Build readme-helper" ON)

if(${SDPTRANSFORM_BUILD_TESTS})
	subdirs(test)
endif()

if(${SDPTRANSFORM_BUILD_READMEHELPER})
	subdirs(readme-helper)
endif()

include_directories(${sdptransform_SOURCE_DIR}/include)

set(
	SOURCE_FILES
	src/grammar.cpp
	src/parser.cpp
	src/writer.cpp
)

set(
	HEADER_FILES
	include/sdptransform.hpp
	include/json.hpp
)

add_library(sdptransform STATIC ${SOURCE_FILES} ${HEADER_FILES})

install(TARGETS sdptransform DESTINATION lib)
install(FILES ${HEADER_FILES} DESTINATION include/sdptransform)
