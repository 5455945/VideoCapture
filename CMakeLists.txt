cmake_minimum_required (VERSION 3.6)

project (VideoCapture)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_UNICODE -DUNICODE")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

set(srcs
    src/CaptrueAudio.cpp
	src/CaptureVideo.cpp
	src/common.cpp
	src/ImageFormatConversion.cpp
	src/Main.cpp
	src/SampleGrabberCallback.cpp
	res/VideoCapture.rc
	res/VideoCapture.ico
	)
set(incs
    inc/CaptureAudio.h
	inc/CaptureVideo.h
	inc/common.h
	inc/ImageFormatConversion.h
	inc/qedit.h
	inc/resource.h
	inc/SampleGrabberCallback.h
	)

include_directories(inc)
add_executable(VideoCapture ${srcs} ${incs})

if (MSVC)
	if(NOT EXISTS "${CMAKE_BINARY_DIR}/ALL_BUILD.vcxproj.user")
		file(GENERATE
			OUTPUT "${CMAKE_BINARY_DIR}/ALL_BUILD.vcxproj.user"
			INPUT "${CMAKE_SOURCE_DIR}/cmake/ALL_BUILD.vcxproj.user.in")
	endif()
	if(NOT EXISTS "${CMAKE_BINARY_DIR}/VideoCapture.vcxproj.user")
		file(GENERATE
			OUTPUT "${CMAKE_BINARY_DIR}/VideoCapture.vcxproj.user"
			INPUT "${CMAKE_SOURCE_DIR}/cmake/VideoCapture.vcxproj.user.in")
	endif()
endif()

install(TARGETS VideoCapture RUNTIME DESTINATION "bin/\${CMAKE_INSTALL_CONFIG_NAME}" LIBRARY DESTINATION "lib/\${CMAKE_INSTALL_CONFIG_NAME}" ARCHIVE DESTINATION "lib/\${CMAKE_INSTALL_CONFIG_NAME}")

include("cmake/install_vs_runtime.cmake")