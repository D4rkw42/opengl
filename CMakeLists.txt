cmake_minimum_required(VERSION 3.29.7)

project(
    opengl
    VERSION 1.0
    DESCRIPTION "aprendendo opengl"
    LANGUAGES C CXX)

# setting cxx standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

# looking for packages
find_package(GLFW3 CONFIG REQUIRED)

# adding headers source
include_directories(
    "${CMAKE_SOURCE_DIR}/deps/glad/include"
)

# generating executables
add_executable(
    opengl
    
    src/main.cpp
    
    deps/glad/src/glad.c)

# linking libraries
target_link_libraries(opengl PRIVATE ${OPENGL_gl_LIBRARY} glfw)
