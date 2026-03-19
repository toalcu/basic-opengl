cmake_minimum_required(VERSION 3.10)
project(opengl_tutorial)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

add_executable(
  ${PROJECT_NAME}
  src/main.cpp
  external/gl3w/src/gl3w.c
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${SDL2_INCLUDE_DIRS}
        external/gl3w/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${SDL2_LIBRARIES}
        OpenGL::GL
)
