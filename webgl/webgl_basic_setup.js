/* eslint no-console:0 consistent-return:0 */
"use strict";

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

// Matrix math functions
var m3 = {
  identity: function() {
    return [
      1, 0, 0,
      0, 1, 0,
      0, 0, 1,
    ];
  },

  translation: function(tx, ty) {
    return [
      1, 0, 0,
      0, 1, 0,
      tx, ty, 1,
    ];
  },

  rotation: function(angleInRadians) {
    var c = Math.cos(angleInRadians);
    var s = Math.sin(angleInRadians);
    return [
      c, -s, 0,
      s, c, 0,
      0, 0, 1,
    ];
  },

  scaling: function(sx, sy) {
    return [
      sx, 0, 0,
      0, sy, 0,
      0, 0, 1,
    ];
  },

  multiply: function(a, b) {
    var a00 = a[0 * 3 + 0];
    var a01 = a[0 * 3 + 1];
    var a02 = a[0 * 3 + 2];
    var a10 = a[1 * 3 + 0];
    var a11 = a[1 * 3 + 1];
    var a12 = a[1 * 3 + 2];
    var a20 = a[2 * 3 + 0];
    var a21 = a[2 * 3 + 1];
    var a22 = a[2 * 3 + 2];
    var b00 = b[0 * 3 + 0];
    var b01 = b[0 * 3 + 1];
    var b02 = b[0 * 3 + 2];
    var b10 = b[1 * 3 + 0];
    var b11 = b[1 * 3 + 1];
    var b12 = b[1 * 3 + 2];
    var b20 = b[2 * 3 + 0];
    var b21 = b[2 * 3 + 1];
    var b22 = b[2 * 3 + 2];

    return [
      b00 * a00 + b01 * a10 + b02 * a20,
      b00 * a01 + b01 * a11 + b02 * a21,
      b00 * a02 + b01 * a12 + b02 * a22,
      b10 * a00 + b11 * a10 + b12 * a20,
      b10 * a01 + b11 * a11 + b12 * a21,
      b10 * a02 + b11 * a12 + b12 * a22,
      b20 * a00 + b21 * a10 + b22 * a20,
      b20 * a01 + b21 * a11 + b22 * a21,
      b20 * a02 + b21 * a12 + b22 * a22,
    ];
  },
};

function main() {
  // Get A WebGL context
  var canvas = document.getElementById("webgl-canvas");
  if (!canvas) {
    throw new Error('Canvas element with id "webgl-canvas" not found.');
  }
  var gl = canvas.getContext("webgl");
  if (!gl) {
    throw new Error('Unable to initialize WebGL. Your browser may not support it.');
  }

  // Get the strings for our GLSL shaders
  var vertexShaderSource = document.querySelector("#vertex-shader-2d").text;
  var fragmentShaderSource = document.querySelector("#fragment-shader-2d").text;

  // create GLSL shaders, upload the GLSL source, compile the shaders
  var vertexShader = createShader(gl, gl.VERTEX_SHADER, vertexShaderSource);
  var fragmentShader = createShader(gl, gl.FRAGMENT_SHADER, fragmentShaderSource);

  // Link the two shaders into a program
  var program = createProgram(gl, vertexShader, fragmentShader);

  // look up where the vertex data needs to go.
  var positionAttributeLocation = gl.getAttribLocation(program, "a_position");
  var matrixLocation = gl.getUniformLocation(program, "u_matrix");

  // Create a buffer and put three 2d clip space points in it
  var positionBuffer = gl.createBuffer();

  // Bind it to ARRAY_BUFFER (think of it as ARRAY_BUFFER = positionBuffer)
  gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);

  // Create a triangle shape
  var positions = [
    0, 0.5,
    -0.4, -0.3,
    0.4, -0.3,
  ];
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);

  // UI Controls
  var transformType = 'rotate';
  var transformValue = 45;

  var transformTypeSelect = document.getElementById('transformType');
  var transformSlider = document.getElementById('transformSlider');
  var sliderValueDisplay = document.getElementById('sliderValue');

  transformTypeSelect.addEventListener('change', function(e) {
    transformType = e.target.value;
    updateSliderRange();
    drawScene();
  });

  transformSlider.addEventListener('input', function(e) {
    transformValue = parseFloat(e.target.value);
    updateSliderDisplay();
    drawScene();
  });

  function updateSliderRange() {
    switch(transformType) {
      case 'translate':
        transformSlider.min = -1;
        transformSlider.max = 1;
        transformSlider.value = 0;
        transformSlider.step = 0.01;
        break;
      case 'rotate':
        transformSlider.min = 0;
        transformSlider.max = 360;
        transformSlider.value = 45;
        transformSlider.step = 1;
        break;
      case 'scale':
      case 'scaleX':
      case 'scaleY':
        transformSlider.min = 10;
        transformSlider.max = 300;
        transformSlider.value = 100;
        transformSlider.step = 5;
        break;
      case 'shear':
        transformSlider.min = -150;
        transformSlider.max = 150;
        transformSlider.value = 0;
        transformSlider.step = 5;
        break;
      case 'reflectX':
      case 'reflectY':
        transformSlider.min = 0;
        transformSlider.max = 1;
        transformSlider.value = 0;
        transformSlider.step = 1;
        break;
    }
    transformValue = parseFloat(transformSlider.value);
    updateSliderDisplay();
    updateDescription();
  }

  function updateSliderDisplay() {
    switch(transformType) {
      case 'translate':
        sliderValueDisplay.textContent = transformValue.toFixed(2) + ' (x)';
        break;
      case 'rotate':
        sliderValueDisplay.textContent = transformValue.toFixed(0) + '°';
        break;
      case 'scale':
      case 'scaleX':
      case 'scaleY':
        sliderValueDisplay.textContent = (transformValue / 100).toFixed(2) + 'x';
        break;
      case 'shear':
        sliderValueDisplay.textContent = (transformValue / 100).toFixed(2);
        break;
      case 'reflectX':
      case 'reflectY':
        sliderValueDisplay.textContent = transformValue === 0 ? 'Off' : 'On';
        break;
    }
  }

  function updateDescription() {
    var descElement = document.getElementById('transformDescription');
    switch(transformType) {
      case 'translate':
        descElement.textContent = 'Move the triangle horizontally along the X-axis (direct x value)';
        break;
      case 'rotate':
        descElement.textContent = 'Rotate the triangle around its center (default 45°)';
        break;
      case 'scale':
        descElement.textContent = 'Scale the triangle uniformly in all directions';
        break;
      case 'scaleX':
        descElement.textContent = 'Scale the triangle along the X-axis (horizontal stretch/compress)';
        break;
      case 'scaleY':
        descElement.textContent = 'Scale the triangle along the Y-axis (vertical stretch/compress)';
        break;
      case 'shear':
        descElement.textContent = 'Shear/skew the triangle (parallel lines remain parallel)';
        break;
      case 'reflectX':
        descElement.textContent = 'Flip the triangle horizontally across the Y-axis';
        break;
      case 'reflectY':
        descElement.textContent = 'Flip the triangle vertically across the X-axis';
        break;
    }
  }

  function drawScene() {
    webglUtils.resizeCanvasToDisplaySize(gl.canvas);

    // Tell WebGL how to convert from clip space to pixels
    gl.viewport(0, 0, gl.canvas.width, gl.canvas.height);

    // Clear the canvas
    gl.clearColor(0, 0, 0, 1);
    gl.clear(gl.COLOR_BUFFER_BIT);

    // Tell it to use our program (pair of shaders)
    gl.useProgram(program);

    // Turn on the attribute
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
        positionAttributeLocation, size, type, normalize, stride, offset);

    // Compute transformation matrix based on current settings
    var matrix = m3.identity();
    
    switch(transformType) {
      case 'translate':
        var tx = transformValue; // Direct x value in clip space
        var ty = 0;
        matrix = m3.multiply(matrix, m3.translation(tx, ty));
        break;
      case 'rotate':
        var angleInRadians = transformValue * Math.PI / 180;
        matrix = m3.multiply(matrix, m3.rotation(angleInRadians));
        break;
      case 'scale':
        var scale = transformValue / 100;
        matrix = m3.multiply(matrix, m3.scaling(scale, scale));
        break;
      case 'scaleX':
        var scaleX = transformValue / 100;
        matrix = m3.multiply(matrix, m3.scaling(scaleX, 1));
        break;
      case 'scaleY':
        var scaleY = transformValue / 100;
        matrix = m3.multiply(matrix, m3.scaling(1, scaleY));
        break;
      case 'shear':
        var shearAmount = transformValue / 100;
        // Shear matrix
        var shearMatrix = [
          1, 0, 0,
          shearAmount, 1, 0,
          0, 0, 1,
        ];
        matrix = m3.multiply(matrix, shearMatrix);
        break;
      case 'reflectX':
        var reflectXMatrix = transformValue === 0 ? 
          [1, 0, 0, 0, 1, 0, 0, 0, 1] : 
          [-1, 0, 0, 0, 1, 0, 0, 0, 1];
        matrix = m3.multiply(matrix, reflectXMatrix);
        break;
      case 'reflectY':
        var reflectYMatrix = transformValue === 0 ? 
          [1, 0, 0, 0, 1, 0, 0, 0, 1] : 
          [1, 0, 0, 0, -1, 0, 0, 0, 1];
        matrix = m3.multiply(matrix, reflectYMatrix);
        break;
    }

    // Set the matrix
    gl.uniformMatrix3fv(matrixLocation, false, matrix);

    // draw
    var primitiveType = gl.TRIANGLES;
    var offset = 0;
    var count = 3;
    gl.drawArrays(primitiveType, offset, count);
  }

  // Initial draw
  updateSliderDisplay();
  updateDescription();
  drawScene();
}

main();
