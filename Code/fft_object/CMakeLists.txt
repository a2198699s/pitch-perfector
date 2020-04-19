cmake_minimum_required(VERSION 3.2)

add_compile_options(-Wall -Wextra -Wpedantic)

project(PitchPerfector)
#add_custom_target(target COMMAND qmake && make run
#                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake/Qt5/")
set(Qt5_DIR "/usr/lib/x86_64-linux-gnu/cmake/Qt5/")

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

#install(TARGETS librtaudio-dev)
#install(TARGETS libfftw3-dev)

#find_package(Qt5 5.1.0 CONFIG REQUIRED Core Widgets Gui)
#find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories(.)
include_directories(/usr/include)
include_directories(/usr/include/rtaudio)
include_directories(/usr/lib)

add_executable(PitchPerfector main.cpp window.h audioStreamer.h fft.h vocoder.h helper.h dispatch.h voxFilter.h window.cpp audioStreamer.cpp fft.cpp vocoder.cpp helper.cpp dispatch.cpp voxFilter.cpp)

target_link_libraries(PitchPerfector Qt5::Widgets)
target_link_libraries(PitchPerfector rtaudio)
target_link_libraries(PitchPerfector fftw3)
 
target_link_libraries(PitchPerfector /usr/lib/libqwt-qt5.so)


