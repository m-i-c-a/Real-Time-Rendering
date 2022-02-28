#include <iostream>
#include <stdlib.h>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Defines.hpp"
#include "Program.hpp"
#include "Loader.hpp"

struct Window {
    const char *name;
    int32_t width, height;
    struct {
        int32_t major, minor;
    } glversion;
    GLFWwindow* handle;
} g_window = {
    "Gooch Shading",
    720, 720,
    {4, 5},
    NULL
};

enum {
    PROGRAM_GOOCH_SHADING,

    PROGRAM_COUNT
};

enum {
    VERTEXARRAY_TRIANGLE,
    VERTEXARRAY_DRAGON,

    VERTEXARRAY_COUNT
};

enum {
    BUFFER_TRIANGLE,
    BUFFER_DRAGON_VERTICES,

    BUFFER_COUNT
};

struct OpenGL {
    GLuint programs[PROGRAM_COUNT];
    GLuint vertexarrays[VERTEXARRAY_COUNT];
    GLuint buffers[BUFFER_COUNT];
} g_gl = {
    {0},
    {0},
    {0},
};

struct App {
    GLsizei vertex_counts[VERTEXARRAY_COUNT];
} g_app;

void Load() {
    // create programs
    g_gl.programs[PROGRAM_GOOCH_SHADING] = 
        create_program("../5-Shading-Basics/shaders/gooch-shading.vert",
                       "../5-Shading-Basics/shaders/gooch-shading.frag",
                       "Gooch Shading");

    // load in a gltf file
    load_model("../assets/dragon.obj",
               &g_gl.vertexarrays[VERTEXARRAY_DRAGON],
               &g_gl.buffers[BUFFER_DRAGON_VERTICES],
               &g_app.vertex_counts[VERTEXARRAY_DRAGON]);

    float triangle_vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.f ,  0.5f, 0.0f,
    };

    glGenVertexArrays(1, &g_gl.vertexarrays[VERTEXARRAY_TRIANGLE]);
    glBindVertexArray(g_gl.vertexarrays[VERTEXARRAY_TRIANGLE]);

    glCreateBuffers(1, &g_gl.buffers[BUFFER_TRIANGLE]);
    glBindBuffer(GL_ARRAY_BUFFER, g_gl.buffers[BUFFER_TRIANGLE]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), (void*)triangle_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0u, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0); 
    glEnableVertexAttribArray(0u);

    g_app.vertex_counts[VERTEXARRAY_TRIANGLE] = 3;
}

void Draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    glUseProgram(g_gl.programs[PROGRAM_GOOCH_SHADING]);
    // glBindVertexArray(g_gl.vertexarrays[VERTEXARRAY_DRAGON]);
    // glDrawArrays(GL_TRIANGLES, 0, g_app.vertex_counts[VERTEXARRAY_DRAGON]);
    glBindVertexArray(g_gl.vertexarrays[VERTEXARRAY_TRIANGLE]);
    glDrawArrays(GL_TRIANGLES, 0, g_app.vertex_counts[VERTEXARRAY_TRIANGLE]);
}

void Release() {
    glDeleteVertexArrays(VERTEXARRAY_COUNT, g_gl.vertexarrays);
    glDeleteBuffers(BUFFER_COUNT, g_gl.buffers);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    g_window.handle = glfwCreateWindow(g_window.width, g_window.height, g_window.name, NULL, NULL);

    if (g_window.handle == NULL) {
        glfwTerminate();
        EXIT("Failed to create glfw window!");
    }
    glfwMakeContextCurrent(g_window.handle);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        EXIT("Failed to initialize GLAD!");
    }

    Load();

    while (!glfwWindowShouldClose(g_window.handle)) {
        glfwPollEvents();
        Draw();
        glfwSwapBuffers(g_window.handle);
    }

    Release();
    glfwTerminate();

    return 0;
}