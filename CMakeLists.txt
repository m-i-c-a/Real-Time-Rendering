cmake_minimum_required(VERSION 3.16)
project(Real-Time-Rendering-Examples)

set(CMAKE_BUILD_TYPE DEBUG)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(submodules)

include_directories(common/)
include_directories(submodules/)
include_directories(submodules/imgui/)

set(IMGUI_CORE_FILES
    ${PROJECT_SOURCE_DIR}/submodules/imgui/imgui_demo.cpp
    ${PROJECT_SOURCE_DIR}/submodules/imgui/imgui_draw.cpp
    ${PROJECT_SOURCE_DIR}/submodules/imgui/imgui_tables.cpp
    ${PROJECT_SOURCE_DIR}/submodules/imgui/imgui_widgets.cpp
    ${PROJECT_SOURCE_DIR}/submodules/imgui/imgui.cpp
)
set(IMGUI_BACKEND_FILES
    ${PROJECT_SOURCE_DIR}/submodules/imgui/backends/imgui_impl_glfw.cpp
    ${PROJECT_SOURCE_DIR}/submodules/imgui/backends/imgui_impl_opengl3.cpp)

set(IMGUI_SOURCES ${IMGUI_CORE_FILES} ${IMGUI_BACKEND_FILES})

add_subdirectory(5-Shading-Basics)