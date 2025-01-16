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
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/deps/glad/include"
    "${CMAKE_SOURCE_DIR}/deps/stb_image/include"
)

# generating executables
add_executable(
    opengl
    
    src/main.cpp

    src/utils/opengl/Shader.cpp
    src/utils/opengl/VBO.cpp
    src/utils/opengl/VAO.cpp
    src/utils/opengl/EBO.cpp
    
    deps/glad/src/glad.c
    deps/stb_image/src/stb_image.c)

# linking libraries
target_link_libraries(opengl PRIVATE ${OPENGL_gl_LIBRARY} glfw)
