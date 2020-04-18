cmake_minimum_required(VERSION 3.2)

add_compile_options(-Wall -Wextra -Wpedantic)

project(PitchPerfector)
#add_custom_target(target COMMAND qmake && make run
#                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#)

include_directories(.)
include_directories(/usr/include)
include_directories(/usr/include/qwt)
include_directories(/usr/include/rtaudio)
include_directories(/usr/lib/qt5)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(PitchPerfector main.cpp window.h audioStreamer.h fft.h vocoder.h helper.h dispatch.h voxFilter.h window.cpp audioStreamer.cpp fft.cpp vocoder.cpp helper.cpp dispatch.cpp voxFilter.cpp)

target_link_libraries(PitchPerfector Qt5::Widgets)
target_link_libraries(PitchPerfector rtaudio)
target_link_libraries(PitchPerfector fftw3)
target_link_libraries(PitchPerfector /usr/lib/qt5)
target_link_libraries(PitchPerfector /usr/include/qwt)
target_link_libraries(PitchPerfector /usr/lib)

