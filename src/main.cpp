#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cassert>

using namespace std;

constexpr unsigned int WIDTH = 1920;
constexpr unsigned int HEIGHT = 1080;

// Shaders
static const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

static const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

static void error_callback(int error, const char* description);
static void processInput(GLFWwindow* window);

int main() {
    glfwSetErrorCallback(error_callback);

    assert(glfwInit());
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGLProject", NULL, NULL);
    assert(window != nullptr);
    glfwMakeContextCurrent(window);
    // Fetch OpenGL function pointers with glad
    assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));


    cout << "Vendor:   " << reinterpret_cast<const char*>(glGetString(GL_VENDOR)) << "\n";
    cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    cout << "Version:  " << glGetString(GL_VERSION) << "\n";
    glfwSwapInterval(1); // Enable vsync


    //  Set up triangle
    float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
    };
    GLuint indices[] = { 
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };


    //  Shader init lambda
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


    //  Compile shaders
    GLuint vertexShader;
    shaderInit(vertexShaderSource, vertexShader, GL_VERTEX_SHADER);
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


    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //  Bind VAO first
    glBindVertexArray(VAO);
    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // EBO (bind while VAO is bound so the association is stored in VAO)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // Vertex attributes (also while VAO is bound)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Unbind VAO
    glBindVertexArray(0);


    // Main loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0,0,width, height);

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

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