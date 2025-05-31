# 3DViewer v2.1

## Description

3DViewer is a program for visualizing three-dimensional models in .obj format, developed in C++ using Qt 5 and OpenGL. The application allows you to load, view, and transform 3D models with an intuitive user interface.

## Functionality

- Loading models in .obj format
- Moving, rotating, and scaling the model
- Displaying model information (number of vertices and edges)
- Control using mouse and interface elements
- Dark gray background for comfortable model viewing
- Command-line interface for direct file loading

## Architecture

The project is implemented using the Model-View-Controller (MVC) pattern:

- **Model**: Loading, storing, and transforming 3D model data
- **View**: Graphical interface and visualization using OpenGL
- **Controller**: Processing user actions and communication between Model and View

## System Requirements

- C++17 compatible compiler
- CMake 3.10 or higher
- Qt 5 (Widgets, Core, Gui, OpenGL)
- OpenGL, GLU
- For Linux: GLUT, libgl1-mesa-dev
- For macOS: Homebrew with Qt 5 installed (`brew install qt@5`)

## Building and Running

### Quick Start

```bash
make run
```

This command will build the project and launch the application.

### Manual Build

```bash
# Create build directory
mkdir -p build

# Build the project
cd build
cmake ..
make

# Run the application
./bin/viewer
```

### Command-line Options

The application supports several command-line options:
```bash
# Show help
./bin/viewer -h

# Show version
./bin/viewer -v

# Load a model file at startup
./bin/viewer -f path/to/model.obj
```

### Additional Commands

```bash
# Clean build files
make clean
```

## Project Structure

```
3DViewer/
├── CMakeLists.txt       # CMake configuration
├── Makefile             # Makefile for convenient building
├── README.md            # Project documentation (English)
├── README_RUS.md        # Project documentation (Russian)
├── .clang-format        # Code formatting configuration
├── src/                 # Source code
│   ├── main.cpp         # Program entry point
│   ├── controller/      # Controller (MVC)
│   │   ├── controller.h
│   │   └── controller.cpp
│   ├── model/           # Model (MVC)
│   │   ├── model.h
│   │   └── model.cpp
│   └── view/            # View (MVC)
│       ├── mainwindow.h
│       ├── mainwindow.cpp
│       ├── glwidget.h
│       └── glwidget.cpp
└── resources/           # Resources
    ├── cube.obj         # Example 3D cube model
    ├── dragon.obj       # Dragon 3D model
    └── madara.obj       # Madara 3D model
```

## Usage

1. Run the program with the command `make run` or `./bin/viewer`
2. Click the "Open File" button to load an .obj model, or use the command-line option `-f filename.obj`
3. Use the mouse to manipulate the model:
   - Left mouse button + movement: rotate the model
   - Mouse wheel: scaling
4. Use the interface controls for:
   - Moving the model along X, Y, Z axes
   - Rotating the model around X, Y, Z axes
   - Scaling the model

## OBJ Format

The program supports the standard OBJ format for 3D models, including:

- Vertices (v): coordinates of points in 3D space
- Faces (f): definition of polygons through vertex indices

Example of a simple cube in OBJ format (resources/cube.obj):
```
v -1.0 -1.0 -1.0  # vertex 1
v -1.0 -1.0  1.0  # vertex 2
...
f 1 3 7 5  # face 1
f 2 6 8 4  # face 2
...
```

## Namespace

The project uses the `viewer3d` namespace for all its components to avoid name conflicts. 