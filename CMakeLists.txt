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


#if(CMAKE_SIZEOF_VOID_P EQUAL 8)
#	set(_platform_suffix x64)
#else()
#	set(_platform_suffix x86)
#endif()

#set(CMAKE_VS_PLATFORM_TOOLSET "v110_xp" CACHE STRING "Platform Toolset" FORCE)

if("${CMAKE_GENERATOR}" MATCHES "^([Vv][Ii][Ss][Uu][Aa][Ll] [Ss][Tt][Uu][Dd][Ii][Oo] 15 2017 [Ww][Ii][Nn]64)$")
  message("CMAKE_GENERATOR:  " ${CMAKE_GENERATOR})
elseif("${CMAKE_GENERATOR}" MATCHES "^([Vv][Ii][Ss][Uu][Aa][Ll] [Ss][Tt][Uu][Dd][Ii][Oo] 15 2017)$")
  message("CMAKE_GENERATOR:  " ${CMAKE_GENERATOR})
elseif("${CMAKE_GENERATOR}" MATCHES "^([Vv][Ii][Ss][Uu][Aa][Ll] [Ss][Tt][Uu][Dd][Ii][Oo] 14 2015 [Ww][Ii][Nn]64)$")
	install(CODE "
	  if(\"\${CMAKE_INSTALL_CONFIG_NAME}\" MATCHES \"^([Dd][Ee][Bb][Uu][Gg])$\")
		file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/debug_nonredist/x64/Microsoft.VC140.DebugCRT/vcruntime140d.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/debug_nonredist/x64/Microsoft.VC140.DebugCRT/msvcp140d.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/debug_nonredist/x64/Microsoft.VC140.DebugCRT/concrt140d.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/debug_nonredist/x64/Microsoft.VC140.DebugCRT/vccorlib140d.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
	  elseif(\"\${CMAKE_INSTALL_CONFIG_NAME}\" MATCHES \"^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$\")
		file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x64/Microsoft.VC140.CRT/vcruntime140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x64/Microsoft.VC140.CRT/msvcp140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x64/Microsoft.VC140.CRT/concrt140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x64/Microsoft.VC140.CRT/vccorlib140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
	  elseif(\"\${CMAKE_INSTALL_CONFIG_NAME}\" MATCHES \"^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$\")
		file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x64/Microsoft.VC140.CRT/vcruntime140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x64/Microsoft.VC140.CRT/msvcp140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x64/Microsoft.VC140.CRT/concrt140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x64/Microsoft.VC140.CRT/vccorlib140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
	  elseif(\"\${CMAKE_INSTALL_CONFIG_NAME}\" MATCHES \"^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$\")
		file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x64/Microsoft.VC140.CRT/vcruntime140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x64/Microsoft.VC140.CRT/msvcp140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x64/Microsoft.VC140.CRT/concrt140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x64/Microsoft.VC140.CRT/vccorlib140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
	  endif()
	")
elseif("${CMAKE_GENERATOR}" MATCHES "^([Vv][Ii][Ss][Uu][Aa][Ll] [Ss][Tt][Uu][Dd][Ii][Oo] 14 2015)$")
	install(CODE "
	  if(\"\${CMAKE_INSTALL_CONFIG_NAME}\" MATCHES \"^([Dd][Ee][Bb][Uu][Gg])$\")
		file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/debug_nonredist/x86/Microsoft.VC140.DebugCRT/vcruntime140d.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/debug_nonredist/x86/Microsoft.VC140.DebugCRT/msvcp140d.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/debug_nonredist/x86/Microsoft.VC140.DebugCRT/concrt140d.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/debug_nonredist/x86/Microsoft.VC140.DebugCRT/vccorlib140d.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
	  elseif(\"\${CMAKE_INSTALL_CONFIG_NAME}\" MATCHES \"^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$\")
		file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x86/Microsoft.VC140.CRT/vcruntime140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x86/Microsoft.VC140.CRT/msvcp140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x86/Microsoft.VC140.CRT/concrt140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x86/Microsoft.VC140.CRT/vccorlib140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
	  elseif(\"\${CMAKE_INSTALL_CONFIG_NAME}\" MATCHES \"^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$\")
		file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x86/Microsoft.VC140.CRT/vcruntime140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x86/Microsoft.VC140.CRT/msvcp140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x86/Microsoft.VC140.CRT/concrt140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x86/Microsoft.VC140.CRT/vccorlib140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
	  elseif(\"\${CMAKE_INSTALL_CONFIG_NAME}\" MATCHES \"^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$\")
		file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x86/Microsoft.VC140.CRT/vcruntime140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x86/Microsoft.VC140.CRT/msvcp140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x86/Microsoft.VC140.CRT/concrt140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
		#file(COPY \"$ENV{VS140COMNTOOLS}../../VC/redist/x86/Microsoft.VC140.CRT/vccorlib140.dll\" DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin/\${CMAKE_INSTALL_CONFIG_NAME}\")
	  endif()
	")
elseif("${CMAKE_GENERATOR}" MATCHES "^([Vv][Ii][Ss][Uu][Aa][Ll] [Ss][Tt][Uu][Dd][Ii][Oo] 12 2013 [Ww][Ii][Nn]64)$")
  message("CMAKE_GENERATOR:  " ${CMAKE_GENERATOR})
elseif("${CMAKE_GENERATOR}" MATCHES "^([Vv][Ii][Ss][Uu][Aa][Ll] [Ss][Tt][Uu][Dd][Ii][Oo] 12 2013)$")
  message("CMAKE_GENERATOR:  " ${CMAKE_GENERATOR})
elseif("${CMAKE_GENERATOR}" MATCHES "^([Vv][Ii][Ss][Uu][Aa][Ll] [Ss][Tt][Uu][Dd][Ii][Oo] 11 2012 [Ww][Ii][Nn]64)$")
  message("CMAKE_GENERATOR:  " ${CMAKE_GENERATOR})
elseif("${CMAKE_GENERATOR}" MATCHES "^([Vv][Ii][Ss][Uu][Aa][Ll] [Ss][Tt][Uu][Dd][Ii][Oo] 11 2012)$")
  message("CMAKE_GENERATOR:  " ${CMAKE_GENERATOR})
elseif("${CMAKE_GENERATOR}" MATCHES "^([Vv][Ii][Ss][Uu][Aa][Ll] [Ss][Tt][Uu][Dd][Ii][Oo] 10 2010 [Ww][Ii][Nn]64)$")
  message("CMAKE_GENERATOR:  " ${CMAKE_GENERATOR})
elseif("${CMAKE_GENERATOR}" MATCHES "^([Vv][Ii][Ss][Uu][Aa][Ll] [Ss][Tt][Uu][Dd][Ii][Oo] 10 2010)$")
  message("CMAKE_GENERATOR:  " ${CMAKE_GENERATOR})
elseif("${CMAKE_GENERATOR}" MATCHES "^([Vv][Ii][Ss][Uu][Aa][Ll] [Ss][Tt][Uu][Dd][Ii][Oo] 9 2008 [Ww][Ii][Nn]64)$")
  message("CMAKE_GENERATOR:  " ${CMAKE_GENERATOR})
elseif("${CMAKE_GENERATOR}" MATCHES "^([Vv][Ii][Ss][Uu][Aa][Ll] [Ss][Tt][Uu][Dd][Ii][Oo] 9 2008)$")
  message("CMAKE_GENERATOR:  " ${CMAKE_GENERATOR})
elseif("${CMAKE_GENERATOR}" MATCHES "^([Vv][Ii][Ss][Uu][Aa][Ll] [Ss][Tt][Uu][Dd][Ii][Oo] 8 2005 [Ww][Ii][Nn]64)$")
  message("CMAKE_GENERATOR:  " ${CMAKE_GENERATOR})
elseif("${CMAKE_GENERATOR}" MATCHES "^([Vv][Ii][Ss][Uu][Aa][Ll] [Ss][Tt][Uu][Dd][Ii][Oo] 8 2005)$")
  message("CMAKE_GENERATOR:  " ${CMAKE_GENERATOR})
endif()
