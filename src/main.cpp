#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cassert>

using namespace std;

#define HEIGHT 1080
#define WIDTH 1920

// Shaders
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

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

    //  Compile vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //  Check for successful compilation
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    assert(success);

    //  Compile fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    assert(success);

    //  Create shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //  Check linking status
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    assert(success);

    //  activate shaders
    glUseProgram(shaderProgram);

    //  Delete shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //  Tell GL how to interpret vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


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

        //  Draw object
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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