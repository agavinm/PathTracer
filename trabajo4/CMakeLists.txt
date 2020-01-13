cmake_minimum_required(VERSION 3.15)
project(trabajo4)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS_RELEASE "-march=native -O3")
# -march=native: It makes the binary possible not compatible on a lot of hardware (basically any CPU that is not a
# direct descendent of the one used for compilation). IMPORTANT: Optimize the program using all the cpu instructions.

include_directories(.)
find_package (Threads)

add_executable(trabajo4
        Geometry.cpp
        Geometry.hpp
        HCoord.cpp
        HCoord.hpp
        Image.cpp
        Image.hpp
        pathtracer.cpp
        Progress.cpp
        Progress.hpp
        Scene.cpp
        Scene.hpp
        Transform.cpp
        Transform.hpp
        Render.cpp
        Render.hpp
        Texture.cpp
        Texture.hpp
        Material.cpp
        Material.hpp
        Color.cpp
        Color.hpp Object.cpp Object.hpp)

target_link_libraries (trabajo4 ${CMAKE_THREAD_LIBS_INIT})
