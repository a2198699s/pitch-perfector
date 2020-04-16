cmake_minimum_required(VERSION 3.2)

add_compile_options(-Wall -Wextra -Wpedantic)

add_custom_target(pitch_perfector COMMAND make run
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Code/GPIO/
)


add_subdirectory(Code/fft_object)

add_custom_target(PitchPerfector COMMAND make run
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Code/fft_object/
)

add_subdirectory(Code/classfiles)

add_custom_target(tester COMMAND make run
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Code/classfiles/
)

#add_executable(pitch_perfector main.cpp)
