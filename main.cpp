#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "Shader.h"
#include "Shape.h"
#include "Transform.h"

// Global state for transformations
float rotationAngle = 0.0f;
float scaleValue = 1.0f;
float translateX = 0.0f;
float translateY = 0.0f;
float shearX = 0.0f;
bool reflectX = false;
bool reflectY = false;
int currentTransform = 0; // 0=linear rotation, 1=scale, 2=affine translate, 3=shear, 4=reflect

static bool keyPressed[GLFW_KEY_LAST] = {false};

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        keyPressed[key] = true;
        
        // Transform selection
        if (key == GLFW_KEY_1) currentTransform = 0;
        if (key == GLFW_KEY_2) currentTransform = 1;
        if (key == GLFW_KEY_3) currentTransform = 2;
        if (key == GLFW_KEY_4) currentTransform = 3;
        if (key == GLFW_KEY_5) currentTransform = 4;
        
        // Toggle reflections
        if (key == GLFW_KEY_X && currentTransform == 4) reflectX = !reflectX;
        if (key == GLFW_KEY_Y && currentTransform == 4) reflectY = !reflectY;
        
        // Reset
        if (key == GLFW_KEY_R) {
            rotationAngle = 0.0f; scaleValue = 1.0f;
            translateX = 0.0f; translateY = 0.0f; shearX = 0.0f;
            reflectX = false; reflectY = false;
        }
        
        // Exit
        if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, true);
    } else if (action == GLFW_RELEASE) {
        keyPressed[key] = false;
    }
}

void processInput(GLFWwindow* window) {
    // Continuous input for arrow keys
    if (keyPressed[GLFW_KEY_LEFT]) {
        if (currentTransform == 0) rotationAngle -= 0.02f;
        if (currentTransform == 2) translateX -= 0.01f;
        if (currentTransform == 3) shearX -= 0.01f;
    }
    if (keyPressed[GLFW_KEY_RIGHT]) {
        if (currentTransform == 0) rotationAngle += 0.02f;
        if (currentTransform == 2) translateX += 0.01f;
        if (currentTransform == 3) shearX += 0.01f;
    }
    if (keyPressed[GLFW_KEY_UP]) {
        if (currentTransform == 1) scaleValue += 0.01f;
        if (currentTransform == 2) translateY += 0.01f;
    }
    if (keyPressed[GLFW_KEY_DOWN]) {
        if (currentTransform == 1) scaleValue -= 0.01f;
        if (currentTransform == 2) translateY -= 0.01f;
    }
}

void printCurrentState() {
    static int frameCount = 0;
    if (++frameCount % 30 == 0) { // Update display every 30 frames
        std::cout << "\r"; // Carriage return to overwrite line
        switch(currentTransform) {
            case 0:
                std::cout << "[ROTATION] Angle: " << std::fixed << std::setprecision(2) 
                         << (rotationAngle * 180.0f / 3.14159f) << "°   ";
                break;
            case 1:
                std::cout << "[SCALE] Factor: " << std::fixed << std::setprecision(2) 
                         << scaleValue << "x   ";
                break;
            case 2:
                std::cout << "[TRANSLATION] X: " << std::fixed << std::setprecision(2) 
                         << translateX << " Y: " << translateY << "   ";
                break;
            case 3:
                std::cout << "[SHEAR] X: " << std::fixed << std::setprecision(2) 
                         << shearX << "   ";
                break;
            case 4:
                std::cout << "[REFLECTION] X: " << (reflectX ? "ON" : "OFF") 
                         << " Y: " << (reflectY ? "ON" : "OFF") << "   ";
                break;
        }
        std::cout << std::flush;
    }
}

int main() {
    // Seed random number generator
    srand(static_cast<unsigned>(time(0)));
    
    // Initialize GLFW
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(1200, 900, "Linear & Affine Transformations", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }
    
    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Controls:\n";
    std::cout << "  1: Linear Rotation (LEFT/RIGHT arrows)\n";
    std::cout << "  2: Linear Scale (UP/DOWN arrows)\n";
    std::cout << "  3: Affine Translation (ARROW keys)\n";
    std::cout << "  4: Linear Shear (LEFT/RIGHT arrows)\n";
    std::cout << "  5: Linear Reflection (X/Y keys)\n";
    std::cout << "  R: Reset transformations\n";
    std::cout << "  ESC: Exit\n\n";
    
    // Setup shader and shape
    Shader shader("triangle.vert", "triangle.frag");
    Shape triangle;
    triangle.setupMesh(Shapes::Triangle());
    
    // Render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        printCurrentState();
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Apply transformation based on current mode
        Mat4 transform;
        switch(currentTransform) {
            case 0: // Linear: Rotation
                transform = LinearTransform::rotation(rotationAngle);
                break;
            case 1: // Linear: Scale
                transform = LinearTransform::scaling(scaleValue, scaleValue);
                break;
            case 2: // Affine: Translation
                transform = AffineTransform::translate(translateX, translateY);
                break;
            case 3: // Linear: Shear
                transform = LinearTransform::shearing(shearX, 0.0f);
                break;
            case 4: // Linear: Reflection
                transform = LinearTransform::reflection(reflectX, reflectY);
                break;
        }
        
        shader.use();
        shader.setMat4("transform", transform.data);
        triangle.draw();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
