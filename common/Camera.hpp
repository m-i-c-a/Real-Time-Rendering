#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct Camera {
    glm::vec3 pos     = { 0.0f, 0.0f,  1.0f };
    glm::vec3 forward = { 0.0f, 0.0f, -1.0f };
    glm::vec3 up      = { 0.0f, 0.0f,  1.0f };
} g_camera;

static bool mouse_down = false;
static glm::vec2 prev_mouse_pos { 0.0f, 0.0f };

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (mouse_down) {
        const double delta_x = prev_mouse_pos.x - xpos;
        const double delta_y = prev_mouse_pos.y - ypos;

    }

    prev_mouse_pos.x = xpos;
    prev_mouse_pos.y = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        mouse_down = true;
    }
    else {
        mouse_down = false;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}


#endif // CAMERA_HPP