#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Shader.h"

class Window {
private:
    GLFWwindow* window;
    Camera camera;
    Shader shader;
    double last_xpos, last_ypos, rate;

    static void error_callback(int error, const char* description);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    void set_callback();

public:
    Window();
    ~Window();

    void init();
    void main_loop();
};
