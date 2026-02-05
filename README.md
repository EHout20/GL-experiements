# GL Experiments

Learning OpenGL/WebGL and exploring linear & affine transformations.

## Project Structure

```
.
├── main.cpp              # Minimal main program with transformation demo
├── include/              # Header files
│   ├── Shader.h         # Shader loading and management
│   ├── Shape.h          # Shape definitions
│   ├── Transform.h      # Linear and affine transformations
│   ├── glad/            # OpenGL loader
│   └── KHR/             # Khronos headers
├── src/                 # Implementation files
│   ├── Shader.cpp
│   ├── Shape.cpp
│   ├── Transform.cpp
│   └── glad.c
├── webgl/              # WebGL experiments
│   ├── index.html
│   ├── index.css
│   └── webgl_basic_setup.js
├── triangle.vert       # Vertex shader
├── triangle.frag       # Fragment shader
└── Makefile           # Build configuration
```

## C++ OpenGL Application

### Building

```bash
make
```

### Running

```bash
./opengl_app
```

### Controls

- **1**: Linear Rotation (LEFT/RIGHT arrows to rotate)
- **2**: Linear Scale (UP/DOWN arrows to scale)
- **3**: Affine Translation (ARROW keys to move)
- **4**: Linear Shear (LEFT/RIGHT arrows to skew)
- **5**: Linear Reflection (X/Y keys to flip)
- **R**: Reset all transformations
- **ESC**: Exit

## WebGL Application

### Running Locally

```bash
cd webgl
python3 -m http.server 8000
```

Then open http://localhost:8000 in your browser.

### Features

- Interactive slider controls
- Multiple transformation types:
  - Translation
  - Rotation
  - Uniform scaling
  - Axis-specific scaling
  - Shearing
  - Reflections

## Transformations

### Linear Transformations
Transformations that preserve the origin (0,0):
- **Rotation**: Rotate around origin
- **Scaling**: Stretch/compress axes
- **Reflection**: Mirror across axes
- **Shearing**: Slant/skew shape

### Affine Transformations
Linear transformations + translation:
- **Translation**: Move position
- **Combined**: Rotation + translation, scale + translation, etc.

## Dependencies

### C++ OpenGL
- GLFW3
- OpenGL 4.6+
- GLAD (included)

### WebGL
- Modern browser with WebGL support

## Learning Resources

- WebGL Fundamentals: https://webglfundamentals.org/
- LearnOpenGL: https://learnopengl.com/
