add_executable(phong-shading
    ${PROJECT_SOURCE_DIR}/common/glad/glad.c
    ${IMGUI_SOURCES}
    phong-shading.cpp)
target_link_libraries(phong-shading glfw GL dl)

add_executable(punctual-lights
    ${PROJECT_SOURCE_DIR}/common/glad/glad.c
    ${IMGUI_SOURCES}
    punctual-lights.cpp)
target_link_libraries(punctual-lights glfw GL dl)