#include "Window.h"

#include <iostream>
#include <stdexcept>

#include "constant.h"
#include "Transformation.h"

using namespace std;

Window::Window()
    : last_xpos(0.0), last_ypos(0.0), rate(7.0)
{
}

Window::~Window()
{
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

void Window::error_callback(int error, const char* description)
{
    cerr << "Error: " << description << '\n';
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);

            break;
        case GLFW_KEY_SPACE:
            this->camera.reset();

            break;
        default:
            break;
    }
}

void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        this->camera.process_mouse(CONSTANT::BUTTON::LEFT, xpos - this->last_xpos, this->last_ypos - ypos);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        this->camera.process_mouse(CONSTANT::BUTTON::RIGHT, this->last_xpos - xpos, ypos - this->last_ypos);
    }

    this->last_xpos = xpos;
    this->last_ypos = ypos;
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    this->rate += yoffset;
    if (this->rate < 0.1) this->rate = 0.1;
}

void Window::set_callback()
{
    glfwSetWindowUserPointer(this->window, this);

    auto framebufferSizeCallback = [](GLFWwindow* window, int width, int height) {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->framebuffer_size_callback(window, width, height);
    };

    auto keyCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->key_callback(window, key, scancode, action, mods);
    };

    auto mouseCallback = [](GLFWwindow* window, double xpos, double ypos) {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->mouse_callback(window, xpos, ypos);
    };

    auto scrollCallback = [](GLFWwindow* window, double xoffset, double yoffset) {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->scroll_callback(window, xoffset, yoffset);
    };

    glfwSetFramebufferSizeCallback(this->window, framebufferSizeCallback);
    glfwSetKeyCallback(this->window, keyCallback);
    glfwSetCursorPosCallback(this->window, mouseCallback);
    glfwSetScrollCallback(this->window, scrollCallback);
}

void Window::init()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) throw runtime_error("Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    this->window = glfwCreateWindow(CONSTANT::WIDTH, CONSTANT::HEIGHT, "Window", nullptr, nullptr);

    if (!(this->window)) {
        glfwTerminate();
        throw runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(this->window);

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

    this->set_callback();

    this->shader = Shader(
        "./src/shader/vertex.glsl",
        "./src/shader/fragment.glsl"
    );
}

void Window::main_loop()
{
    float vertices[] = {
        -100.0, -100.0, 0.0,
        100.0, -100.0, 0.0,
        0.0, 100.0, 0.0
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

    while (!glfwWindowShouldClose(this->window)) {
        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->shader.use();

        Transformation transformation(this->shader);
        transformation.set_projection(CONSTANT::WIDTH, CONSTANT::HEIGHT, this->rate, -5000.0, 5000.0);
        transformation.set_view(this->camera.view_matrix());

        transformation.set();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(this->window);
        glfwPollEvents();
    }

    glfwTerminate();
}
