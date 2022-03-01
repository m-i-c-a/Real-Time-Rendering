add_executable(gooch-shading
    ${PROJECT_SOURCE_DIR}/common/glad/glad.c
    ${IMGUI_SOURCES}
    gooch-shading.cpp)

target_link_libraries(gooch-shading glfw GL dl)