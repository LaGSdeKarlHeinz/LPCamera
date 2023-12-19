cmake_minimum_required(VERSION 3.19)
project(stream_tools)
set(CMAKE_CXX_STANDARD 17)

if(NOT LINUX)
     message(FATAL_ERROR "Vaut mieux FF fast, impossible d'arriver à installer ces librairies sur autre chose que linux")  
endif()
    
find_package(PkgConfig REQUIRED)
pkg_check_modules(AVCODEC		REQUIRED IMPORTED_TARGET libavcodec)
pkg_check_modules(AVFORMAT		REQUIRED IMPORTED_TARGET libavformat)
pkg_check_modules(AVFILTER		REQUIRED IMPORTED_TARGET libavfilter)
pkg_check_modules(AVDEVICE		REQUIRED IMPORTED_TARGET libavdevice)
pkg_check_modules(AVUTIL		REQUIRED IMPORTED_TARGET libavutil)
pkg_check_modules(SWRESAMPLE		REQUIRED IMPORTED_TARGET libswresample)
pkg_check_modules(SWSCALE		REQUIRED IMPORTED_TARGET libswscale)

set(BOOST_ENABLE_CMAKE ON)

include(FetchContent)
FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_PROGRESS TRUE
  GIT_TAG boost-1.82.0
)
FetchContent_MakeAvailable(Boost)


find_package(OpenCV REQUIRED)
# find_package(Boost 1.40.0 REQUIRED COMPONENTS asio)


add_executable(camera_encoder encoder.cpp)

add_executable(decoder decoder.cpp)

target_link_libraries(decoder 
    PkgConfig::AVFORMAT
    PkgConfig::AVCODEC
    PkgConfig::AVFILTER
    PkgConfig::AVUTIL
    PkgConfig::AVDEVICE
    PkgConfig::SWRESAMPLE
    PkgConfig::SWSCALE
    Boost::asio
    Boost::filesystem
    Boost::thread
    Boost::program_options
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
    Boost::asio
    Boost::filesystem
    Boost::thread
    Boost::program_options
    ${OpenCV_LIBS}
)
