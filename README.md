# Face Detection with OpenCV

## Description
Simple face detection application using OpenCV's Haar Cascade classifier.

## Requirements
- OpenCV 4.x (with contrib modules for haarcascade files)
- CMake 3.22+
- C++17 compatible compiler

## Build Instructions
1. Create build directory: `mkdir build && cd build`
2. Configure: `cmake ..`
3. Build: `cmake --build .`

## Usage Example
```bash
./face_detect ../haarcascade_frontalface_default.xml
```

Note: Download haarcascade files from OpenCV's GitHub repository: