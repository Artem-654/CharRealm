cmake_minimum_required(VERSION 3.15)
project(Launcher)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

file(GLOB SRC_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Pages/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Pages/*.h
)
add_executable(CharRealm ${SRC_FILES})

target_link_libraries(CharRealm PRIVATE Core Game)

target_include_directories(CharRealm PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}
${PROJECT_SOURCE_DIR}/src/Core
)