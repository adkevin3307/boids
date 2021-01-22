#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

#include "constant.h"

using namespace std;

void error_callback(int error, const char* description)
{
    cerr << "Error: " << description << '\n';
}

void APIENTRY debug_callback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    // ignore non-significant error / warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    cout << "Debug message (" << id << "): " << message << '\n';

    switch (source) {
        case GL_DEBUG_SOURCE_API:
            cout << "Source: API" << '\n';
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            cout << "Source: Window System" << '\n';
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            cout << "Source: Shader Compiler" << '\n';
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            cout << "Source: Third Party" << '\n';
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            cout << "Source: Application" << '\n';
            break;
        case GL_DEBUG_SOURCE_OTHER:
            cout << "Source: Other" << '\n';
            break;
    }

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            cout << "Type: Error" << '\n';
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            cout << "Type: Deprecated Behaviour" << '\n';
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            cout << "Type: Undefined Behaviour" << '\n';
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            cout << "Type: Portability" << '\n';
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            cout << "Type: Performance" << '\n';
            break;
        case GL_DEBUG_TYPE_MARKER:
            cout << "Type: Marker" << '\n';
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            cout << "Type: Push Group" << '\n';
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            cout << "Type: Pop Group" << '\b';
            break;
        case GL_DEBUG_TYPE_OTHER:
            cout << "Type: Other" << '\n';
            break;
    }

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            cout << "Severity: high" << '\n';
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            cout << "Severity: medium" << '\n';
            break;
        case GL_DEBUG_SEVERITY_LOW:
            cout << "Severity: low" << '\n';
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            cout << "Severity: notification" << '\n';
            break;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        // case GLFW_KEY_SPACE:
        // this->camera.reset();
        // break;
        default:
            break;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    // this->camera.process_mouse(BUTTON::LEFT, xpos - this->last_xpos, this->last_ypos - ypos);
    // }
    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
    // this->camera.process_mouse(BUTTON::RIGHT, this->last_xpos - xpos, ypos - this->last_ypos);
    // }

    // this->last_xpos = xpos;
    // this->last_ypos = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // this->rate += yoffset;
    // if (this->rate < 0.1) this->rate = 0.1;
}

void set_callback(GLFWwindow* window)
{
    // auto framebufferSizeCallback = [](GLFWwindow *window, int width, int height) {
    // static_cast<WindowManagement *>(glfwGetWindowUserPointer(window))->framebuffer_size_callback(window, width, height);
    // };

    // auto keyCallback = [](GLFWwindow *window, int key, int scancode, int action, int mods) {
    // static_cast<WindowManagement *>(glfwGetWindowUserPointer(window))->key_callback(window, key, scancode, action, mods);
    // };

    // auto mouseCallback = [](GLFWwindow *window, double xpos, double ypos) {
    // static_cast<WindowManagement *>(glfwGetWindowUserPointer(window))->mouse_callback(window, xpos, ypos);
    // };

    // auto scrollCallback = [](GLFWwindow *window, double xoffset, double yoffset) {
    // static_cast<WindowManagement *>(glfwGetWindowUserPointer(window))->scroll_callback(window, xoffset, yoffset);
    // };

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
}

int main()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) throw runtime_error("Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(CONSTANT::WIDTH, CONSTANT::HEIGHT, "window", nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        throw runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        throw runtime_error("Failed to initialize GLEW");
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debug_callback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_PROGRAM_POINT_SIZE);

    set_callback(window);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
