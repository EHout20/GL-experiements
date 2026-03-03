// Vertex shader that applies 2D scale, rotation, and translation to each vertex,
// then converts the result from pixel space into clip space for rendering.
const vertexShaderSource = `
attribute vec2 a_position;

uniform vec2 u_resolution;
uniform mat3 u_rotationAroundPoint;

void main() {
  vec3 pos = u_rotationAroundPoint * vec3(a_position, 1.0);
  vec2 position = pos.xy;

  // Convert from pixel space to clip space (-1 to +1).
  vec2 zeroToOne = position / u_resolution;
  vec2 zeroToTwo = zeroToOne * 2.0;
  vec2 clipSpace = zeroToTwo - 1.0;

  // Flip Y so pixel-space top-left origin matches WebGL clip-space orientation.
  gl_Position = vec4(clipSpace * vec2(1.0, -1.0), 0.0, 1.0);
}
`;

export default vertexShaderSource;
