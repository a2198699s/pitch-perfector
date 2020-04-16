cmake_minimum_required(VERSION 3.2)

add_compile_options(-Wall -Wextra -Wpedantic)


add_custom_target(PitchPerfector COMMAND qmake && make run
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
