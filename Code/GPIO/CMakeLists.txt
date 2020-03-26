cmake_minimum_required(VERSION 3.2)

add_compile_warnings(-Wall -Wextra -Wpedantic)

add_custom_target(readin COMMAND make run
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
