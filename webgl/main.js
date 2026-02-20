import vertexShaderSource from './shaders/vertex-translation.js';
// import vertexShaderSource from './shaders/vertex-clipspace.js'; // normalized coordinates
import fragmentShaderSource from './shaders/fragment-shader.js';

var canvas = document.getElementById('demo-canvas');
var gl = canvas ? canvas.getContext('webgl') : null;

var translation = [0, 0];

if (!gl) {
  console.error('WebGL not available on this device/browser.');
} else {

var vertexShader = createShader(gl, gl.VERTEX_SHADER, vertexShaderSource);
var fragmentShader = createShader(gl, gl.FRAGMENT_SHADER, fragmentShaderSource);

var program = createProgram(gl, vertexShader, fragmentShader);

var positionAttributeLocation = gl.getAttribLocation(program, "a_position");

var positionBuffer = gl.createBuffer();

var resolutionUniformLocation = gl.getUniformLocation(program, "u_resolution");
var translationUniformLocation = gl.getUniformLocation(program, "u_translation");

gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);

canvas.width = canvas.clientWidth;
canvas.height = canvas.clientHeight;
gl.viewport(0, 0, canvas.width, canvas.height);

//boilerplate code for creating shaders and programs 
function createShader(gl, type, source) {
  var shader = gl.createShader(type);
  gl.shaderSource(shader, source);
  gl.compileShader(shader);
  var success = gl.getShaderParameter(shader, gl.COMPILE_STATUS);
  if (success) {
    return shader;
  }
  
  console.log(gl.getShaderInfoLog(shader));
  gl.deleteShader(shader);
}

function createProgram(gl, vertexShader, fragmentShader) {
  var program = gl.createProgram();
  gl.attachShader(program, vertexShader);
  gl.attachShader(program, fragmentShader);
  gl.linkProgram(program);
  var success = gl.getProgramParameter(program, gl.LINK_STATUS);
  if (success) {
    return program;
  }
  
  console.log(gl.getProgramInfoLog(program));
  gl.deleteProgram(program);
}

function updatePosition(index) {
  return function(event, ui) {
    translation[index] = ui.value;
    drawScene();
  };
}

function setupSliders() {
  if (typeof webglLessonsUI === 'undefined') {
    console.error('webglLessonsUI not loaded.');
    return;
  }

  webglLessonsUI.setupSlider("#x", {
    slide: updatePosition(0),
    max: gl.canvas.width
  });
  webglLessonsUI.setupSlider("#y", {
    slide: updatePosition(1),
    max: gl.canvas.height
  });
}
function drawScene() {
  //clearing hte canvas
  gl.clearColor(0, 0, 0, 0);
  gl.clear(gl.COLOR_BUFFER_BIT);

  // Tell it to use our program (pair of shaders)
  gl.useProgram(program);
  // set the resolution
  gl.uniform2f(resolutionUniformLocation, gl.canvas.width, gl.canvas.height);
  // set the translation
  gl.uniform2fv(translationUniformLocation, translation);

  //enabling the attribute on
  gl.enableVertexAttribArray(positionAttributeLocation);

  // Bind the position buffer.
  gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);

  // Tell the attribute how to get data out of positionBuffer (ARRAY_BUFFER)
  var size = 2;          // 2 components per iteration
  var type = gl.FLOAT;   // the data is 32bit floats
  var normalize = false; // don't normalize the data
  var stride = 0;        // 0 = move forward size * sizeof(type) each iteration to get the next position
  var offset = 0;        // start at the beginning of the buffer
  gl.vertexAttribPointer(
      positionAttributeLocation, size, type, normalize, stride, offset)

  var primitiveType = gl.TRIANGLES;
  var offset = 0;
  var count = 18;
  gl.drawArrays(primitiveType, offset, count);
}

function main() {
  setupSliders();
  setGeometry(gl);
  drawScene();
}

main();
}

function setRectangle(gl, buffer, x, y, width, height) {
  var x1 = x;
  var x2 = x + width;
  var y1 = y;
  var y2 = y + height;
  gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
  gl.bufferData(
      gl.ARRAY_BUFFER,
      new Float32Array([
          x1, y1,
          x2, y1,
          x1, y2,
          x1, y2,
          x2, y1,
          x2, y2,
      ]),
      gl.STATIC_DRAW);
}

function setGeometry(gl) {
  gl.bufferData(
      gl.ARRAY_BUFFER,
      new Float32Array([
          // left column
          0, 0,
          30, 0,
          0, 150,
          0, 150,
          30, 0,
          30, 150,
 
          // top rung
          30, 0,
          100, 0,
          30, 30,
          30, 30,
          100, 0,
          100, 30,
 
          // middle rung
          30, 60,
          67, 60,
          30, 90,
          30, 90,
          67, 60,
          67, 90,
      ]),
      gl.STATIC_DRAW);
}


