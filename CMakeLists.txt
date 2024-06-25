cmake_minimum_required(VERSION 3.0)

project(osff_core VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)

if(WIN32)
    set(SHARED_POSTFIX ".dll")
    set(STATIC_POSTFIX ".lib")
elseif(APPLE)
    set(SHARED_POSTFIX ".dylib")
    set(STATIC_POSTFIX ".a")
elseif(LINUX)
    set(SHARED_POSTFIX ".so")
    set(STATIC_POSTFIX ".a")
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

set(FFMPEG_INCLUDE /usr/local/include)
set(FFMPEG_LIBRARIES
    /usr/local/lib/libavformat${SHARED_POSTFIX}
    /usr/local/lib/libavcodec${SHARED_POSTFIX}
    /usr/local/lib/libavdevice${SHARED_POSTFIX}
    /usr/local/lib/libavfilter${SHARED_POSTFIX}
    /usr/local/lib/libavutil${SHARED_POSTFIX}
    /usr/local/lib/libpostproc${SHARED_POSTFIX}
    /usr/local/lib/libswresample${SHARED_POSTFIX}
    /usr/local/lib/libswscale${SHARED_POSTFIX}
    )

add_subdirectory(thirdparty)
add_subdirectory(src)