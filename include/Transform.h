#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <cmath>

// Simple 4x4 matrix for transformations
struct Mat4 {
    float data[16];
    
    Mat4();
    static Mat4 identity();
    static Mat4 translate(float x, float y, float z);
    static Mat4 rotateZ(float angleRadians);
    static Mat4 scale(float sx, float sy, float sz);
    static Mat4 shear(float shx, float shy);
    Mat4 operator*(const Mat4& other) const;
};

// Linear transformations (preserve origin)
namespace LinearTransform {
    Mat4 rotation(float angle);
    Mat4 scaling(float sx, float sy);
    Mat4 reflection(bool x, bool y);
    Mat4 shearing(float shx, float shy);
}

// Affine transformations (linear + translation)
namespace AffineTransform {
    Mat4 translate(float x, float y);
    Mat4 rotateAndTranslate(float angle, float tx, float ty);
    Mat4 scaleAndTranslate(float sx, float sy, float tx, float ty);
}

#endif
