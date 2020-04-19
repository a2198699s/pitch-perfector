cmake_minimum_required(VERSION 3.2)

project(PitchPerfector VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)

add_compile_options(-Wall -Wextra -Wpedantic)

add_subdirectory(Code/fft_object)

#add_custom_target(PPerfector COMMAND make all
#                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Code/fft_object/
#)
add_custom_target(target COMMAND qmake && make run
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Code/fft_object/
)

#add_subdirectory(Code/rtaudio_probes/probe_devices)

#add_custom_target(audioProbe COMMAND make run
#                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Code/rtaudio_probes/probe_devices/
#)
