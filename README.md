# Collatz Sequence Calculator written on C++ QtWidgets

## What it does

The application calculates, within a user-specified integer interval, which starting integer produces the longest Collatz (3n + 1) sequence. It supports multithreaded calculation so you can test different numbers of worker threads to measure parallel speedup.

## Typical workflow:
- Choose a lower and upper bound for the interval (positive integers).

- Choose the number of threads to use.

- Start the calculation.

The app displays the number that produced the longest Collatz chain, the length of that chain, and timing information.

## Features

- Graphical user interface built with Qt Widgets.

- Multithreaded computation (user-configurable number of threads).

- Displays results: starting number with longest sequence, sequence length, and elapsed time.

- Simple, platform-independent build using CMake.

Tech stack

- C++ (modern C++)

- Qt (Qt Widgets) — UI

## Prerequisites

- C++ toolchain (GCC / Clang / MSVC) supporting C++20 onwards (project uses modern C++; adjust if necessary).

CMake (recommended >= 3.19).

Qt development libraries (Qt6.5+). The project uses Qt Widgets — ensure the QtWidgets component is available.

## Build (CMake + Qt)

Below are general, cross-platform instructions that work on Linux/macOS and Windows.

1. Clone the repository
```
git clone https://github.com/Nexu66/Collatz-Sequence-Calculator.git
cd Collatz-Sequence-Calculator
```
2. Create a build directory and configure with CMake
```Unix
mkdir build
cd build
cmake ..
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_PREFIX_PATH="/path/to/Qt/lib/cmake"   # adjust to your Qt installation
```
3. Build
```Windows
cmake --build . --config Release
```
```Unix
cmake --build .
```
4. Run 
```
./src/app/main
```

## Testing (GoogleTest)

This project uses GoogleTest for unit testing. Tests are placed under the tests/ directory and are integrated in the CMake configuration.

## Project structure
/
├── build/
│   ├── MyKit-Debug/         
│   └── MyKit-Release/       
├── CMakeLists.txt
├── src/
│   ├── app/
│   │   ├── CMakeLists.txt
│   │   └── main.cpp
│   ├── controller/
│   │   ├── CMakeLists.txt
│   │   ├── controller.cpp
│   │   └── controller.hpp
│   ├── core/
│   │   ├── CMakeLists.txt
│   │   ├── core.cpp
│   │   ├── core.hpp
│   │   ├── coreImpl/
│   │   │   ├── CMakeLists.txt
│   │   │   ├── coreImpl.cpp
│   │   │   └── coreImpl.hpp
│   │   └── model/
│   │       ├── CMakeLists.txt
│   │       └── model.hpp
│   ├── dialog/
│   │   ├── CMakeLists.txt
│   │   ├── dialog.cpp/.hpp
│   │   └── dialog.ui
│   └── timer/
│       ├── CMakeLists.txt
│       └── Timer.hpp
├── tests/                    # GoogleTest unit tests
└── (other build and metadata files)
