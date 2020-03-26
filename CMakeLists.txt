cmake_minimum_required(VERSION 3.2)

add_compile_options(-Wall -Wextra -Wpedantic)

add_subdirectory(Code)

add_custom_target(readin COMMAND make run
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Code/GPIO/
)

#add_executable(pitch_perfector main.cpp)
