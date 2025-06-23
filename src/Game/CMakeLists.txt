cmake_minimum_required(VERSION 3.15)
project(Game)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_library(Game STATIC ${SRC_FILES})

target_include_directories(Game PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Game PRIVATE Core)