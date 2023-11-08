cmake_minimum_required(VERSION 3.19)
project(stream_tools)
set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(AVCODEC		REQUIRED IMPORTED_TARGET libavcodec)
pkg_check_modules(AVFORMAT		REQUIRED IMPORTED_TARGET libavformat)
pkg_check_modules(AVFILTER		REQUIRED IMPORTED_TARGET libavfilter)
pkg_check_modules(AVDEVICE		REQUIRED IMPORTED_TARGET libavdevice)
pkg_check_modules(AVUTIL		REQUIRED IMPORTED_TARGET libavutil)
pkg_check_modules(SWRESAMPLE		REQUIRED IMPORTED_TARGET libswresample)
pkg_check_modules(SWSCALE		REQUIRED IMPORTED_TARGET libswscale)

find_package(OpenCV REQUIRED)

add_executable(camera_encoder main.cpp mat_utils.cpp)

add_executable(decoder decoder.cpp)

target_link_libraries(decoder 
    PkgConfig::AVFORMAT
    PkgConfig::AVCODEC
    PkgConfig::AVFILTER
    PkgConfig::AVUTIL
    PkgConfig::AVDEVICE
    PkgConfig::SWRESAMPLE
    PkgConfig::SWSCALE
    ${OpenCV_LIBS}
)


target_link_libraries(camera_encoder 
    PkgConfig::AVFORMAT
    PkgConfig::AVCODEC
    PkgConfig::AVFILTER
    PkgConfig::AVUTIL
    PkgConfig::AVDEVICE
    PkgConfig::SWRESAMPLE
    PkgConfig::SWSCALE
    ${OpenCV_LIBS}
)
