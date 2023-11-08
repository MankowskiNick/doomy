# Doomy
Doomy is a Binary Space Partitioning (BSP) rendering engine currently being developed in C++ and leverages OpenGL/GLAD for enhanced rendering capabilities. This project aims to provide a robust and efficient solution for rendering simple 3D environments, leveraging the power of BSP trees for optimized spatial queries and rendering performance.

## Features
- Efficient rendering of simple 3D environments using BSP trees.
- Integration with OpenGL/GLAD for enhanced rendering capabilities.
- Level editor build in Python 3 with TKinter.

## Upcoming Features
- Support for textures
- Support for NPCs
- Shader support

## Level Editor
There is a level editor for this project present in the editor/ directory.  The editor is written in Python with TKinter.  Worlds can be saved to the lvl/ folder and you can modify main.cpp to load the level you would like.

## Dependencies
- C++11
- OpenGL
- GLAD
- Python 3(Level Editor)
- TKinter(Level Editor)

## Getting Started

1. Clone the repository:
```bash
git clone https://github.com/MankowskiNick/doomy
cd doomy
```

2. Build the project:
```bash
mkdir build
cd build
cmake ..
make
```

3. Run the Executable
Run the executable located in the build/ folder.

## Documentation
Detailed documentation can be found here and this document will be updated as new features are added.

## Contact
Questions regarding Doomy can be sent to my email(nnmankowski@owu.edu).