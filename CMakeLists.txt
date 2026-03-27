cmake_minimum_required(VERSION 4.1)
project(graphics_coursework)

set(CMAKE_CXX_STANDARD 20)

# 1. Find the packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glm CONFIG REQUIRED)

include_directories(${GLUT_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS})

add_executable(graphics_coursework main.cpp)

target_link_libraries(graphics_coursework ${OPENGL_LIBRARIES} GLUT::GLUT glm::glm)