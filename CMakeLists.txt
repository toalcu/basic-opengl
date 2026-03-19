cmake_minimum_required(VERSION 3.10)
project(opengl_tutorial)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

set(SHADER_DIR "${CMAKE_SOURCE_DIR}/resources/shaders")

configure_file(
  ${CMAKE_SOURCE_DIR}/include/config.hpp.in
  ${CMAKE_BINARY_DIR}/generated/config.hpp
  @ONLY
)

add_executable(
  ${PROJECT_NAME}
  src/main.cpp
  src/shader.cpp
  external/gl3w/src/gl3w.c
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${SDL2_INCLUDE_DIRS}
        external/gl3w/include
        ${CMAKE_BINARY_DIR}/generated
        include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${SDL2_LIBRARIES}
        OpenGL::GL
)
