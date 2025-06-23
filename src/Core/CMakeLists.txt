cmake_minimum_required(VERSION 3.15)
project(Core)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/UI/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UI/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ConsoleHandlers/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ConsoleHandlers/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Interfaces/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Interfaces/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PageManagers/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PageManagers/*.h
)

add_library(Core STATIC ${SRC_FILES})

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                       ${CMAKE_CURRENT_SOURCE_DIR}/UI
                                       ${CMAKE_CURRENT_SOURCE_DIR}/ConsoleHandlers
                                       )