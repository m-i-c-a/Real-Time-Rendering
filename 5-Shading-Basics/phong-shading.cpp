#include <iostream>
#include <stdlib.h>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

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

struct Camera {
    double yaw = -1.0f * glm::half_pi<double>();
    double pitch = 0.0f;
    glm::mat4 matrix { 1.0f };
} g_camera;

static bool mouse_down = false;
static glm::vec2 prev_mouse_pos { 0.0f, 0.0f };

static void update_camera() {

    glm::vec3 pos {
        glm::cos(g_camera.yaw) * glm::cos(g_camera.pitch),
        glm::sin(g_camera.pitch),
        glm::sin(g_camera.yaw) * glm::cos(g_camera.pitch) };

    pos *= 2;
    
    glm::vec3 forward = glm::normalize(-1.0f * pos);
    glm::vec3 up { 0.0f, 1.0f, 0.0f };
    glm::vec3 right = glm::normalize(glm::cross(forward, up));
    up = glm::normalize(glm::cross(right, forward));

    g_camera.matrix = glm::lookAt(pos, forward, up);

    // This wasn't giving correct reutls, idk why
    // g_camera.matrix = {
    //     right.x, right.y, right.z, -g_camera.pos.x,
    //     up.x, up.y, up.z, -g_camera.pos.y,
    //     forward.x, forward.y, forward.z, -g_camera.pos.z, 
    //     0.0f, 0.0f, 0.0f, 1.0f
    // };
}


enum {
    PROGRAM_PHONG_SHADING,

    PROGRAM_COUNT,
};

enum {
    VERTEXARRAY_DRAGON,
    VERTEXARRAY_SPHERE,

    VERTEXARRAY_COUNT
};

enum {
    BUFFER_DRAGON_VERTICES,
    BUFFER_SPHERE_VERTICES,

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
    bool loaded = false;

    GLsizei vertex_counts[VERTEXARRAY_COUNT];

    int current_model = VERTEXARRAY_DRAGON;
    int current_shading_model = PROGRAM_PHONG_SHADING;
    
    // Uniforms
    struct DirLight {
        glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
        glm::vec3 ambient   = glm::vec3(0.2f);
        glm::vec3 diffuse   = glm::vec3(0.5f);
        glm::vec3 specular  = glm::vec3(0.2f);
    } dir_light;

    struct Material {
        glm::vec3 ambient   = glm::vec3(0.2f);
        glm::vec3 diffuse   = glm::vec3(0.5f);
        glm::vec3 specular  = glm::vec3(0.2f);
        float shininess     = 64.0f;
    } material;

    struct Phong {
        float light_ambient_factor  = 0.0f;
        float light_diffuse_factor  = 1.0f;
        float light_specular_factor = 1.0f;
        float light_shininess_factor = 1.0f;

        float mat_ambient_factor  = 0.0f;
        float mat_diffuse_factor  = 1.0f;
        float mat_specular_factor = 1.0f;
    } phong;

    struct Gooch {

    } gooch;


    float diffuse_factor = 1.0f;
    float ambient_factor = 0.0f;
} g_app;

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (mouse_down) {
        // drag across screen = pi radians of rotation
        const double delta_x = -1.0f * glm::pi<double>() * (prev_mouse_pos.x - xpos) / g_window.width ;
        const double delta_y = -1.0f * glm::pi<double>() * (prev_mouse_pos.y - ypos) / g_window.height;

        g_camera.yaw += delta_x;
        if (g_camera.yaw > glm::two_pi<double>()) g_camera.yaw -= glm::two_pi<double>();
        if (g_camera.yaw < -1.0f * glm::two_pi<double>()) g_camera.yaw += glm::two_pi<double>();

        g_camera.pitch = glm::clamp(g_camera.pitch + delta_y, -1.0f * glm::half_pi<double>() + 0.01f, glm::half_pi<double>() - 0.01f);

        update_camera();
    }

    prev_mouse_pos.x = xpos;
    prev_mouse_pos.y = ypos;
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (!g_app.loaded)
        return;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        auto io = ImGui::GetIO();
        if (!io.WantCaptureMouse)
            mouse_down = true;
        else
            mouse_down = false;
    }
    else {
        mouse_down = false;
    }
}

void Load() {
    glm::mat4 proj_matrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -50.0f, 50.0f);
    glm::mat4 view_matrix = glm::mat4 { 1.0f };

    update_camera();

    // create programs
    g_gl.programs[PROGRAM_PHONG_SHADING] = 
        create_program("../5-Shading-Basics/shaders/phong-shading.vert",
                       "../5-Shading-Basics/shaders/phong-shading.frag",
                       "Gooch Shading");

    for (uint8_t i = 0; i < PROGRAM_COUNT; ++i) {
        glUseProgram(g_gl.programs[i]);
        set_uni_mat4(g_gl.programs[i], "u_proj_matrix", &(proj_matrix[0][0]));
        set_uni_mat4(g_gl.programs[i], "u_view_matrix", &(g_camera.matrix[0][0]));
        glUseProgram(0u);
    }
    
    glGenVertexArrays(VERTEXARRAY_COUNT, g_gl.vertexarrays);
    glGenBuffers(BUFFER_COUNT, g_gl.buffers);

    load_model("../assets/dragon.obj",
               &g_gl.vertexarrays[VERTEXARRAY_DRAGON],
               &g_gl.buffers[BUFFER_DRAGON_VERTICES],
               &g_app.vertex_counts[VERTEXARRAY_DRAGON]);

    load_model("../assets/sphere.obj",
               &g_gl.vertexarrays[VERTEXARRAY_SPHERE],
               &g_gl.buffers[BUFFER_SPHERE_VERTICES],
               &g_app.vertex_counts[VERTEXARRAY_SPHERE]);
}

void SetUniforms() {
    switch (g_app.current_shading_model) {
        case PROGRAM_PHONG_SHADING:
            set_uni_mat4(g_gl.programs[g_app.current_shading_model], "u_view_matrix", &(g_camera.matrix[0][0]));
            set_uni_vec3(g_gl.programs[g_app.current_shading_model], "u_view_pos", &(g_camera.matrix[2][0]));

            set_uni_vec3(g_gl.programs[g_app.current_shading_model], "u_dir_light.direction", &(g_app.dir_light.direction[0]));
            set_uni_vec3(g_gl.programs[g_app.current_shading_model], "u_dir_light.ambient"  , &(g_app.dir_light.ambient[0]));
            set_uni_vec3(g_gl.programs[g_app.current_shading_model], "u_dir_light.diffuse"  , &(g_app.dir_light.diffuse[0]));
            set_uni_vec3(g_gl.programs[g_app.current_shading_model], "u_dir_light.specular" , &(g_app.dir_light.specular[0]));

            set_uni_vec3(g_gl.programs[g_app.current_shading_model] , "u_mat.ambient"   , &(g_app.material.ambient[0]));
            set_uni_vec3(g_gl.programs[g_app.current_shading_model] , "u_mat.diffuse"   , &(g_app.material.diffuse[0]));
            set_uni_vec3(g_gl.programs[g_app.current_shading_model] , "u_mat.specular"  , &(g_app.material.specular[0]));
            set_uni_float(g_gl.programs[g_app.current_shading_model], "u_mat.shininess" , g_app.material.shininess);
            break;
    };
}

void Draw() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(g_gl.programs[g_app.current_shading_model]);
    SetUniforms();
    glBindVertexArray(g_gl.vertexarrays[g_app.current_model]);
    glDrawArrays(GL_TRIANGLES, 0, g_app.vertex_counts[g_app.current_model]);
    glUseProgram(0u);
}

void DrawGui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Phong Shading");
    {
        if (ImGui::CollapsingHeader("Asset")) {
            const char* models[] = { "DRAGON", "SPHERE" };
            ImGui::Combo("Model", &g_app.current_model, models, IM_ARRAYSIZE(models));
        }

        if (ImGui::CollapsingHeader("Shading Model")) {
            const char* shading_models[] = { "PHONG" };
            ImGui::Combo("Shading Model", &g_app.current_shading_model, shading_models, IM_ARRAYSIZE(shading_models));

            switch (g_app.current_shading_model)
            {
                case PROGRAM_PHONG_SHADING:
                    ImGui::SliderFloat("Light Ambient", &g_app.dir_light.ambient[0], 0.0f, 1.0f, "%.2f");
                    ImGui::SliderFloat("Light Diffuse", &g_app.dir_light.diffuse[0], 0.0f, 1.0f, "%.2f");
                    ImGui::SliderFloat("Light Specular", &g_app.dir_light.specular[0], 0.0f, 1.0f, "%.2f");
                    ImGui::SliderFloat("Mat Ambient", &g_app.material.ambient[0], 0.0f, 1.0f, "%.2f");
                    ImGui::SliderFloat("Mat Diffuse", &g_app.material.diffuse[0], 0.0f, 1.0f, "%.2f");
                    ImGui::SliderFloat("Mat Specular", &g_app.material.specular[0], 0.0f, 1.0f, "%.2f");
                    ImGui::SliderFloat("Mat Shininess", &g_app.material.shininess, 1.0f, 128.0f, "%.2f");

                    g_app.dir_light.ambient[1] = g_app.dir_light.ambient[2] = g_app.dir_light.ambient[0];
                    g_app.dir_light.diffuse[1] = g_app.dir_light.diffuse[2] = g_app.dir_light.diffuse[0];
                    g_app.dir_light.specular[1] = g_app.dir_light.specular[2] = g_app.dir_light.specular[0];

                    g_app.material.ambient[1] = g_app.material.ambient[2] = g_app.material.ambient[0];
                    g_app.material.diffuse[1] = g_app.material.diffuse[2] = g_app.material.diffuse[0];
                    g_app.material.specular[1] = g_app.material.specular[2] = g_app.material.specular[0];

                    break;
            };
        }

    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
    glfwSetCursorPosCallback(g_window.handle, cursor_position_callback);
    glfwSetMouseButtonCallback(g_window.handle, mouse_button_callback);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        EXIT("Failed to initialize GLAD!");
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(g_window.handle, true);
    ImGui_ImplOpenGL3_Init("#version 450");

    Load();
    g_app.loaded = true;

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(g_window.handle)) {
        glfwPollEvents();
        Draw();
        DrawGui();
        glfwSwapBuffers(g_window.handle);
    }

    Release();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}