#include "Transform.h"
#include <cstring>

Mat4::Mat4() {
    memset(data, 0, sizeof(data));
}

Mat4 Mat4::identity() {
    Mat4 m;
    m.data[0] = 1.0f; m.data[5] = 1.0f; m.data[10] = 1.0f; m.data[15] = 1.0f;
    return m;
}

Mat4 Mat4::translate(float x, float y, float z) {
    Mat4 m = identity();
    m.data[12] = x;
    m.data[13] = y;
    m.data[14] = z;
    return m;
}

Mat4 Mat4::rotateZ(float angleRadians) {
    Mat4 m = identity();
    float c = cos(angleRadians);
    float s = sin(angleRadians);
    m.data[0] = c;  m.data[1] = s;
    m.data[4] = -s; m.data[5] = c;
    return m;
}

Mat4 Mat4::scale(float sx, float sy, float sz) {
    Mat4 m = identity();
    m.data[0] = sx;
    m.data[5] = sy;
    m.data[10] = sz;
    return m;
}

Mat4 Mat4::shear(float shx, float shy) {
    Mat4 m = identity();
    m.data[4] = shx;  // shear X by Y
    m.data[1] = shy;  // shear Y by X
    return m;
}

Mat4 Mat4::operator*(const Mat4& other) const {
    Mat4 result;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            float sum = 0.0f;
            for (int i = 0; i < 4; i++) {
                sum += data[row + i * 4] * other.data[i + col * 4];
            }
            result.data[row + col * 4] = sum;
        }
    }
    return result;
}

// Linear transformations
namespace LinearTransform {
    Mat4 rotation(float angle) {
        return Mat4::rotateZ(angle);
    }
    
    Mat4 scaling(float sx, float sy) {
        return Mat4::scale(sx, sy, 1.0f);
    }
    
    Mat4 reflection(bool x, bool y) {
        return Mat4::scale(x ? -1.0f : 1.0f, y ? -1.0f : 1.0f, 1.0f);
    }
    
    Mat4 shearing(float shx, float shy) {
        return Mat4::shear(shx, shy);
    }
}

// Affine transformations
namespace AffineTransform {
    Mat4 translate(float x, float y) {
        return Mat4::translate(x, y, 0.0f);
    }
    
    Mat4 rotateAndTranslate(float angle, float tx, float ty) {
        return Mat4::translate(tx, ty, 0.0f) * Mat4::rotateZ(angle);
    }
    
    Mat4 scaleAndTranslate(float sx, float sy, float tx, float ty) {
        return Mat4::translate(tx, ty, 0.0f) * Mat4::scale(sx, sy, 1.0f);
    }
}
