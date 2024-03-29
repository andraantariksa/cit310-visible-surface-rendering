# Visible Surface Rendering

Software 3D visible surface detection from scratch.

![Preview](Assets/screenshot.png)

Project specification: https://drive.google.com/file/d/1P8CvyMibf9ybc6awHf14rL9yykMASFDB/view?usp=sharing
Documentation: https://docs.google.com/document/d/1R-9qPYU4d2uEHgacrtewMTQuhr-W_LVN5OjD2--VuDs/edit?usp=sharing

## Features

- Switch between [Z-Buffer](https://en.wikipedia.org/wiki/Z-buffering) and [Binary Space Partitioning (BSP) Tree](https://en.wikipedia.org/wiki/Binary_space_partitioning) with [Painter's algorithm](https://en.wikipedia.org/wiki/Painter%27s_algorithm)
- Tree .svg generator for BSP Tree
- Load a simple custom .obj file. See `SampleObjects/`
- Rotation and Translation
- Object selection and deletion

## Build Instructions

```
git submodule update --init --recursive
mkdir build/
cd build/
cmake ..
cmake --build . --target all
```

## License

Licensed under [GNU GPL-v3](LICENSE)
