#ifndef SHAPE_H
#define SHAPE_H

#include <glad/glad.h>
#include <vector>

class Shape {
public:
    unsigned int VAO, VBO;
    int vertexCount;
    
    Shape();
    ~Shape();
    void setupMesh(const std::vector<float>& vertices);
    void draw();
};

// Predefined shapes
namespace Shapes {
    std::vector<float> Triangle();
    std::vector<float> Square();
    std::vector<float> LetterF();
    std::vector<float> Pentagon();
}

#endif
