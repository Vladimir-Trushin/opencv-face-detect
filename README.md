# Real-Time Face Detection

A C++ application that detects faces in a live webcam stream using OpenCV's
Haar cascade classifier. Detected faces are outlined in real time and the
current frame rate is displayed on screen.

---

## Features

- Live capture from the default system camera
- Haar cascade face detection with histogram equalisation for better
  performance under uneven lighting
- On-screen FPS counter
- Explicit error handling for missing arguments, unreadable cascade files
  and unavailable camera devices

---

## Requirements

| Component | Minimum version |
|---|---|
| OpenCV | 4.x |
| CMake | 3.22 |
| C++ compiler | C++17 (MSVC 19.3x, GCC 9, or Clang 10) |
| Ninja | 1.10 (optional, recommended) |

The Haar cascade XML files ship with the standard OpenCV distribution —
no `opencv_contrib` modules and no additional downloads are required.
They are located under `<opencv-install>/etc/haarcascades/`.

---

## Setup

### Windows

Download the prebuilt Windows package from https://opencv.org/releases/ and
extract it to `C:\tools\opencv`.

Tell CMake where to find it:

```powershell
[Environment]::SetEnvironmentVariable("OpenCV_DIR","C:\tools\opencv\build","User")
```

Add the runtime libraries to `PATH`, otherwise the executable will fail to
start with a missing `opencv_world*.dll` error:

```powershell
$old = [Environment]::GetEnvironmentVariable("Path","User")
[Environment]::SetEnvironmentVariable("Path", "$old;C:\tools\opencv\build\x64\vc16\bin", "User")
```

Restart the terminal so the new variables take effect.

### Linux

```bash
sudo apt install libopencv-dev cmake ninja-build
```

Cascade files are installed to `/usr/share/opencv4/haarcascades/`.

---

## Building

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

On Windows the configure step must be run from a **Developer PowerShell for
VS 2022**, since the Ninja generator resolves `cl.exe` through `PATH`.

If Ninja is unavailable, the default generator works as well — note that the
build type then moves to the build step and the binary lands in a
configuration subdirectory:

```bash
cmake -S . -B build
cmake --build build --config Release
```

---

## Usage

The program takes the path to a cascade XML file as its only argument.

**Windows**

```powershell
.\build\face_detect.exe C:\tools\opencv\build\etc\haarcascades\haarcascade_frontalface_default.xml
```

**Linux**

```bash
./build/face_detect /usr/share/opencv4/haarcascades/haarcascade_frontalface_default.xml
```

Press **ESC** to quit.

Other cascades from the same directory can be substituted directly —
`haarcascade_eye.xml` and `haarcascade_profileface.xml` work without any
code changes.

---

## How it works

Each frame goes through four stages:

1. **Grayscale conversion.** Haar features operate on intensity alone, so
   colour information is discarded up front to cut the work per frame.
2. **Histogram equalisation.** Redistributing intensity values improves
   detection when the subject is backlit or unevenly lit.
3. **Multi-scale detection.** `detectMultiScale` slides a detection window
   across the image at progressively larger scales. `scaleFactor = 1.1`
   shrinks the image by 10% per pass; `minNeighbors = 5` requires five
   overlapping detections before a region is accepted, which suppresses
   most false positives.
4. **Annotation.** Bounding boxes and the frame rate are drawn onto the
   original colour frame for display.

---

## Limitations

Haar cascades were introduced by Viola and Jones in 2001 and carry the
constraints of that era:

- Reliable only on **frontal** faces — detection degrades sharply beyond
  roughly 20° of head rotation
- Sensitive to strong shadows and low light despite equalisation
- Prone to false positives on high-contrast rectangular patterns
- Returns bounding boxes only, with no confidence score

The trade-off is speed: detection runs comfortably in real time on a CPU,
with no GPU, no model download and no runtime dependencies beyond OpenCV
itself.

---

## Possible improvements

- Replace the cascade with OpenCV's DNN module and the ResNet-10 SSD face
  detector for higher accuracy and rotation tolerance
- Add temporal tracking so boxes persist through brief detection gaps
- Expose `scaleFactor`, `minNeighbors` and the camera index as
  command-line options
- Downscale frames before detection and rescale the results, trading a
  little accuracy for a substantial speed gain

---

## License

MIT for project code. OpenCV and the bundled cascade files are distributed
under the Apache License 2.0.
