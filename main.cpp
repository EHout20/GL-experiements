#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
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

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // Transform selection
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) currentTransform = 0;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) currentTransform = 1;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) currentTransform = 2;
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) currentTransform = 3;
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) currentTransform = 4;
    
    // Controls
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (currentTransform == 0) rotationAngle -= 0.02f;
        if (currentTransform == 2) translateX -= 0.01f;
        if (currentTransform == 3) shearX -= 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (currentTransform == 0) rotationAngle += 0.02f;
        if (currentTransform == 2) translateX += 0.01f;
        if (currentTransform == 3) shearX += 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (currentTransform == 1) scaleValue += 0.01f;
        if (currentTransform == 2) translateY += 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (currentTransform == 1) scaleValue -= 0.01f;
        if (currentTransform == 2) translateY -= 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        if (currentTransform == 4) reflectX = !reflectX;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        if (currentTransform == 4) reflectY = !reflectY;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        rotationAngle = 0.0f; scaleValue = 1.0f;
        translateX = 0.0f; translateY = 0.0f; shearX = 0.0f;
        reflectX = false; reflectY = false;
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(1200, 900, "Linear & Affine Transformations", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    
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
