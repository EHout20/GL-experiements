#include "Shape.h"
#include <cmath>

Shape::Shape() : VAO(0), VBO(0), vertexCount(0) {}

Shape::~Shape() {
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
}

void Shape::setupMesh(const std::vector<float>& vertices) {
    vertexCount = vertices.size() / 6; // 6 floats per vertex (x,y,z, r,g,b)
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
}

void Shape::draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}

// Predefined shapes
namespace Shapes {
    std::vector<float> Triangle() {
        return {
            // positions        // colors
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Red
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Green
             0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // Blue
        };
    }
    
    std::vector<float> Square() {
        return {
            // First triangle
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
            // Second triangle
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f
        };
    }
    
    std::vector<float> LetterF() {
        return {
            // Positions scaled to normalized coordinates
            // Left column
            -0.5f, -0.5f, 0.0f,  1.0f, 0.5f, 0.0f,
            -0.3f, -0.5f, 0.0f,  1.0f, 0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f,  1.0f, 0.5f, 0.0f,
            
            -0.5f,  0.5f, 0.0f,  1.0f, 0.5f, 0.0f,
            -0.3f, -0.5f, 0.0f,  1.0f, 0.5f, 0.0f,
            -0.3f,  0.5f, 0.0f,  1.0f, 0.5f, 0.0f,
            
            // Top rung
            -0.3f,  0.3f, 0.0f,  0.5f, 1.0f, 0.5f,
             0.2f,  0.3f, 0.0f,  0.5f, 1.0f, 0.5f,
            -0.3f,  0.5f, 0.0f,  0.5f, 1.0f, 0.5f,
            
            -0.3f,  0.5f, 0.0f,  0.5f, 1.0f, 0.5f,
             0.2f,  0.3f, 0.0f,  0.5f, 1.0f, 0.5f,
             0.2f,  0.5f, 0.0f,  0.5f, 1.0f, 0.5f,
            
            // Middle rung
            -0.3f, -0.1f, 0.0f,  0.5f, 0.5f, 1.0f,
             0.0f, -0.1f, 0.0f,  0.5f, 0.5f, 1.0f,
            -0.3f,  0.1f, 0.0f,  0.5f, 0.5f, 1.0f,
            
            -0.3f,  0.1f, 0.0f,  0.5f, 0.5f, 1.0f,
             0.0f, -0.1f, 0.0f,  0.5f, 0.5f, 1.0f,
             0.0f,  0.1f, 0.0f,  0.5f, 0.5f, 1.0f
        };
    }
    
    std::vector<float> Pentagon() {
        float cx = 0.0f, cy = 0.0f;
        float radius = 0.5f;
        return {
            // Triangle 1
            cx, cy, 0.0f,  1.0f, 0.0f, 0.0f,
            cx + radius * cos(0.0f), cy + radius * sin(0.0f), 0.0f,  1.0f, 0.5f, 0.0f,
            cx + radius * cos(1.256f), cy + radius * sin(1.256f), 0.0f,  1.0f, 1.0f, 0.0f,
            
            // Triangle 2
            cx, cy, 0.0f,  0.0f, 1.0f, 0.0f,
            cx + radius * cos(1.256f), cy + radius * sin(1.256f), 0.0f,  0.5f, 1.0f, 0.0f,
            cx + radius * cos(2.512f), cy + radius * sin(2.512f), 0.0f,  0.0f, 1.0f, 0.5f,
            
            // Triangle 3
            cx, cy, 0.0f,  0.0f, 0.0f, 1.0f,
            cx + radius * cos(2.512f), cy + radius * sin(2.512f), 0.0f,  0.0f, 0.5f, 1.0f,
            cx + radius * cos(3.768f), cy + radius * sin(3.768f), 0.0f,  0.5f, 0.0f, 1.0f,
            
            // Triangle 4
            cx, cy, 0.0f,  1.0f, 0.0f, 1.0f,
            cx + radius * cos(3.768f), cy + radius * sin(3.768f), 0.0f,  1.0f, 0.5f, 0.5f,
            cx + radius * cos(5.024f), cy + radius * sin(5.024f), 0.0f,  0.5f, 1.0f, 0.5f,
            
            // Triangle 5
            cx, cy, 0.0f,  1.0f, 1.0f, 0.0f,
            cx + radius * cos(5.024f), cy + radius * sin(5.024f), 0.0f,  1.0f, 0.5f, 1.0f,
            cx + radius * cos(0.0f), cy + radius * sin(0.0f), 0.0f,  0.5f, 0.5f, 1.0f
        };
    }
}
