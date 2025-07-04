cmake_minimum_required(VERSION 3.15)
project(MyProject)

set(CMAKE_CXX_STANDARD 20)

#add_subdirectory(src)

# Статична бібліотека
#add_library(math STATIC lib/math/math.cpp)
#target_include_directories(math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/math)

# Список з бібліотеками
#set(MY_STATIC_LIBS
#    src/main
#    /path/to/lib2.a
#    /path/to/lib3.a
#    # Додавай інші бібліотеки
#)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++")

add_subdirectory(src/Core)
add_subdirectory(src/Game)
add_subdirectory(src/Launcher)

# Додаємо бібліотеки до цілі
#target_link_libraries(MyTarget PRIVATE ${MY_STATIC_LIBS})