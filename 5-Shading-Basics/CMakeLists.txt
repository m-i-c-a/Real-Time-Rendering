cmake_minimum_required(VERSION 3.0)
project(gooch-shading)

include_directories(${PROJECT_SOURCE_DIR}/common)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(glfw3 3.3 REQUIRED)

add_executable(${PROJECT_NAME} 
    common/glad/glad.c
    gooch-shading.cpp)

target_link_libraries(${PROJCET_NAME} glfw GL dl)
