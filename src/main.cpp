#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cassert>

using namespace std;

#define HEIGHT 1080
#define WIDTH 1920

void error_callback(int error, const char* description);
void processInput(GLFWwindow* window);

int main() {
    // Set the error callback
    glfwSetErrorCallback(error_callback);

    // Initialize GLFW
    assert(glfwInit());
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGLProject", NULL, NULL);
    assert(window != nullptr);
    glfwMakeContextCurrent(window);

    // Fetch OpenGL function pointers with glad
    assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));


    // OpenGL info calls
    cout << "Vendor:   " << reinterpret_cast<const char*>(glGetString(GL_VENDOR)) << "\n";
    cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    cout << "Version:  " << glGetString(GL_VERSION) << "\n";

    glfwSwapInterval(1); // Enable vsync

    //  Set up triangle
    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.5f
    };

    //  Generate buffer ID
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    //  Bind this to G
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //  Copy vertices to buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        //  Inputs
        processInput(window);

        //  Rendering Commands
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0,0,WIDTH, HEIGHT);

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f); //    Bluish gray color
        glClear(GL_COLOR_BUFFER_BIT);

        //  Check events and swap buffers
        glfwPollEvents();
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


void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}