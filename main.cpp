// Mini-Project: Indian Flag
// Renders the Indian national flag with Ashoka Chakra using OpenGL 
// Arnav Sinha https://github.com/arnav43
// Linkedin https://www.linkedin.com/in/arnavsinha4334/

// Note-
// comments added via chatgpt
// I am using multiple vao's, vbo's for practice as I am learning OpenGL, you can use one if you want

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <iostream>

// Function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Window settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800; // Matches Indian flag proportions

// Ashoka Chakra circle parameters
const int numSegments = 50;    // Number of segments for circle approximation
const float radius = 0.25f;    // Radius of Ashoka Chakra
const float M_PI = 3.14f;      // Pi constant
std::vector<float> circlePoints;

// Function to generate circle vertices for Ashoka Chakra
void makeCircle() {
    circlePoints.push_back(0.0f);  // Center x
    circlePoints.push_back(0.0f);  // Center y
    circlePoints.push_back(0.0f);  // Center z

    // Generate circle points using parametric equation
    for (int i = 0; i <= numSegments; i++) {
        float angle = 2.0f * M_PI * i / numSegments;
        circlePoints.push_back(radius * cos(angle));       // x coordinate
        circlePoints.push_back(radius * sin(angle) * 1.25); // y coordinate (stretched vertically)
        circlePoints.push_back(0.0f);                      // z coordinate
    }
}

// Vertex shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Fragment shaders for flag colors
const char* fragmentShader1Source = "#version 330 core\n" // Saffron
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.00f, 0.60f, 0.20f, 1.0f);\n"
"}\n\0";

const char* fragmentShader2Source = "#version 330 core\n" // Green
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.07f, 0.53f, 0.03f, 1.0f);\n"
"}\n\0";

const char* fragmentShader3Source = "#version 330 core\n" // Navy blue
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.00f, 0.00f, 0.60f, 1.0f);\n"
"}\n\0";

int main() {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Indian Flag", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Compile shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader1, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragmentShader1);

    unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShader2Source, NULL);
    glCompileShader(fragmentShader2);

    unsigned int fragmentShader3 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader3, 1, &fragmentShader3Source, NULL);
    glCompileShader(fragmentShader3);

    // Link shader programs
    unsigned int shaderProgram1 = glCreateProgram();
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1);

    unsigned int shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);

    unsigned int shaderProgram3 = glCreateProgram();
    glAttachShader(shaderProgram3, vertexShader);
    glAttachShader(shaderProgram3, fragmentShader3);
    glLinkProgram(shaderProgram3);

    // Clean up shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader1);
    glDeleteShader(fragmentShader2);
    glDeleteShader(fragmentShader3);

    // Vertex data for flag stripes
    float saffron[] = {
        -1.0f,  1.0f,  0.0f,    // Top-left
         1.0f,  1.0f,  0.0f,    // Top-right
        -1.0f,  0.33f, 0.0f,    // Bottom-left
         1.0f,  1.0f,  0.0f,    // Top-right
         1.0f,  0.33f, 0.0f,    // Bottom-right
        -1.0f,  0.33f, 0.0f     // Bottom-left
    };
    float green[] = {
        -1.0f, -0.33f, 0.0f,    // Top-left
         1.0f, -0.33f, 0.0f,    // Top-right
        -1.0f, -1.0f,  0.0f,    // Bottom-left
         1.0f, -0.33f, 0.0f,    // Top-right
         1.0f, -1.0f,  0.0f,    // Bottom-right
        -1.0f, -1.0f,  0.0f     // Bottom-left
    };

    // Generate circle points for Ashoka Chakra
    makeCircle();

    // Generate spoke points for Ashoka Chakra (24 spokes)
    std::vector<float> spokes;
    const int numSpokes = 24;
    for (int i = 0; i < numSpokes; i++) {
        float angle = 2.0f * M_PI * i / numSpokes;
        spokes.push_back(0.0f);              // Center x
        spokes.push_back(0.0f);              // Center y
        spokes.push_back(0.0f);              // Center z
        spokes.push_back(radius * cos(angle)); // End x
        spokes.push_back(radius * sin(angle)); // End y
        spokes.push_back(0.0f);              // End z
    }

    // Setup VAOs and VBOs
    unsigned int VAOs[3], VBOs[3];
    glGenVertexArrays(3, VAOs);
    glGenBuffers(3, VBOs);

    unsigned int circleVAO, circleVBO;
    unsigned int spokesVAO, spokesVBO;

    // Saffron stripe setup
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(saffron), saffron, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Green stripe setup
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(green), green, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Ashoka Chakra circle setup
    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);
    glBindVertexArray(circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
    glBufferData(GL_ARRAY_BUFFER, circlePoints.size() * sizeof(float), circlePoints.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Ashoka Chakra spokes setup
    glGenVertexArrays(1, &spokesVAO);
    glGenBuffers(1, &spokesVBO);
    glBindVertexArray(spokesVAO);
    glBindBuffer(GL_ARRAY_BUFFER, spokesVBO);
    glBufferData(GL_ARRAY_BUFFER, spokes.size() * sizeof(float), spokes.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // Clear screen with white background
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw saffron stripe
        glUseProgram(shaderProgram1);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Draw green stripe
        glUseProgram(shaderProgram2);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Draw Ashoka Chakra (circle and spokes)
        glUseProgram(shaderProgram3);
        glBindVertexArray(circleVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, circlePoints.size() / 3);
        glBindVertexArray(spokesVAO);
        glDrawArrays(GL_LINES, 0, spokes.size() / 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(3, VAOs);
    glDeleteBuffers(3, VBOs);
    glDeleteVertexArrays(1, &circleVAO);
    glDeleteBuffers(1, &circleVBO);
    glDeleteVertexArrays(1, &spokesVAO);
    glDeleteBuffers(1, &spokesVBO);
    glDeleteProgram(shaderProgram1);
    glDeleteProgram(shaderProgram2);
    glDeleteProgram(shaderProgram3);

    glfwTerminate();
    return 0;
}

// Handle keyboard input
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Adjust viewport when window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
