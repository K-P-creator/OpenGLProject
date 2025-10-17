#define GLFW_INCLUDE_NONE

#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cassert>

using namespace std;

void error_callback(int error, const char* description);

int main() {
    // Set the error callback
    glfwSetErrorCallback(error_callback);

    // Initialize GLFW
    assert(glfwInit());
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "OpenGLProject", NULL, NULL);
    assert(window != nullptr);

    // Make context current
    glfwMakeContextCurrent(window);

    // Fetch OpenGL function pointers with glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // OpenGL info calls
    cout << "Vendor:   " << reinterpret_cast<const char*>(glGetString(GL_VENDOR)) << "\n";
    cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    cout << "Version:  " << glGetString(GL_VERSION) << "\n";

    glfwSwapInterval(1); // Enable vsync

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f); //    Bluish gray color
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
        Sleep(10);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}

// Error callback
void error_callback(int error, const char* description) {
    cerr << "GLFW Error [" << error << "]: " << description << "\n";
}
