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
    GLuint VBO;
    glGenBuffers(1, &VBO);
    //  Bind this to G
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //  Copy vertices to buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    int success;
    auto shaderInit = [&success](const char* source, GLuint& name, GLenum type){
        name = glCreateShader(type);
        glShaderSource(name, 1, &source, NULL);
        glCompileShader(name);

        //  Check for success
        glGetShaderiv(name, GL_COMPILE_STATUS, &success);

        if (!success) {
            GLint len = 0; glGetShaderiv(name, GL_INFO_LOG_LENGTH, &len);
            std::string log(len, '\0');
            glGetShaderInfoLog(name, len, nullptr, log.data());
            std::cerr << "Shader compile failed (" << name << "):\n" << log << "\n";
            std::abort();
        }
        };

    //  Compile vertex shader
    GLuint vertexShader;
    shaderInit(vertexShaderSource, vertexShader, GL_VERTEX_SHADER);

    //  Compile fragment shader
    GLuint fragmentShader;
    shaderInit(fragmentShaderSource, fragmentShader, GL_FRAGMENT_SHADER);

    //  Create shader program
    GLuint shaderProgram;
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

    GLuint VAO;
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
        glViewport(0,0,width, height);

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