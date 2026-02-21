const vertexShaderSource = `
attribute vec2 a_position;

uniform vec2 u_resolution;
uniform vec2 u_translation;
uniform vec2 u_rotation;
uniform vec2 u_scale;

void main() {
  // Rotate the position with a 2x2 rotation matrix.
  // u_rotation = vec2(sin(theta), cos(theta))
  mat2 rotationMatrix = mat2(
    u_rotation.y, u_rotation.x,
   -u_rotation.x, u_rotation.y
  );
  //scaling hte x and the y
  a_position = vec2(a_position.x * u_scale.x, a_position.y * u_scale.y);
  vec2 rotatedPosition = rotationMatrix * a_position;

  // Add in the translation
  vec2 position = rotatedPosition + u_translation;

  vec2 zeroToOne = position / u_resolution;
  vec2 zeroToTwo = zeroToOne * 2.0;
  vec2 clipSpace = zeroToTwo - 1.0;

  gl_Position = vec4(clipSpace * vec2(1.0, -1.0), 0.0, 1.0);
}
`;

export default vertexShaderSource;





