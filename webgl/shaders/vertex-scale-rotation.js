const vertexShaderSource = `
attribute vec2 a_position;

uniform vec2 u_resolution;
uniform vec2 u_translation;
uniform vec2 u_rotation;
uniform vec2 u_scale;
uniform vec2 u_rotationCenter;

void main() {
  // Build generalized 2D rigid transform in homogeneous coordinates:
  // T(translation) * T(rotationCenter) * R * T(-rotationCenter) * S
  mat3 scaleMatrix = mat3(
    u_scale.x, 0.0, 0.0,
    0.0, u_scale.y, 0.0,
    0.0, 0.0, 1.0
  );

  // u_rotation = vec2(sin(theta), cos(theta))
  mat3 rotationMatrix = mat3(
    u_rotation.y, u_rotation.x, 0.0,
   -u_rotation.x, u_rotation.y, 0.0,
    0.0,          0.0,          1.0
  );

  mat3 toCenter = mat3(
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    u_rotationCenter.x, u_rotationCenter.y, 1.0
  );

  mat3 fromCenter = mat3(
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
   -u_rotationCenter.x, -u_rotationCenter.y, 1.0
  );

  mat3 translationMatrix = mat3(
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    u_translation.x, u_translation.y, 1.0
  );

  vec3 worldPosition =
      translationMatrix * toCenter * rotationMatrix * fromCenter * scaleMatrix * vec3(a_position, 1.0);
  vec2 position = worldPosition.xy;

  // Convert to clip space
  vec2 zeroToOne = position / u_resolution;
  vec2 zeroToTwo = zeroToOne * 2.0;
  vec2 clipSpace = zeroToTwo - 1.0;

  gl_Position = vec4(clipSpace * vec2(1.0, -1.0), 0.0, 1.0);
}
`;

export default vertexShaderSource;
