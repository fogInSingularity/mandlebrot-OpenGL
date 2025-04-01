cmake_minimum_required(VERSION 3.20)
project(mandelbrot C CXX)

include(cmake/third_party.cmake)
include(cmake/defaults.cmake)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        src/source/main.cpp
        src/source/mandelbrot.cpp
        src/source/opengl_log.cpp
        src/source/shader.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        src/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        mandelbrot-defaults
        glad
        m 
        glfw 
        GL 
        X11 
        pthread 
        Xrandr 
        Xi
        glm
)
