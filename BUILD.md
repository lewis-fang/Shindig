# Building and Running Shindig-FNN on Linux

Instructions verified on this machine against Qt 5.15.18 and GCC 15.2.0.

## Prerequisites

- CMake: `/usr/bin/cmake`
- C++ compiler: `/usr/bin/c++` (GCC 15.2.0)
- GNU Make: `/usr/bin/gmake`
- Qt 5.15.18 development packages, with CMake configs at:
  - Qt5Widgets: `/usr/lib/x86_64-linux-gnu/cmake/Qt5Widgets`
  - Qt5Charts: `/usr/lib/x86_64-linux-gnu/cmake/Qt5Charts`

Notes:

- Qt 5.12 or newer is required (`find_package(Qt5 5.12 ...)` in `CMakeLists.txt`).
- An x86_64 host is required. The code uses x86 SIMD intrinsics (SSE/AVX/AVX2) and will not build on other architectures.

## Configure

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

## Build

```bash
cmake --build build -j$(nproc)
```

The resulting binary is `build/FNN`.

## Run

Launch from the data-correct working directory, using the absolute path to the binary:

```bash
cd x64/Release
/abs/path/to/build/FNN
```

The app hardcodes CWD-relative paths: `./cifar-10-binary/cifar-10-batches-bin/data_batch_1.bin` for data import (`FNN/FNN.cpp`) and `./outModel.onet` for model save/load (`CNNModel.cpp`). Launching from `x64/Release/` makes the bundled data resolve. On this machine the verified absolute path is `/home/lewis/project/3-shindig/1-version 1.0.0/Shindig-FNN5-B1.5.8/build/FNN`.

## Working-directory warning

Both data import and model save/load are relative to the launch directory. Launch the binary from `x64/Release/`. From any other directory the CIFAR data will not be found, and models will be saved to and loaded from that other directory instead.

## `-march=native` portability

The binary is compiled for the CPU it was built on. `CMakeLists.txt` enables `FNN_ENABLE_NATIVE_SIMD=ON` by default, which adds `-O3 -march=native`. The resulting binary will not port across machines. Rebuild on any other machine before running it there.

## GUI only, no headless mode

This is a Qt GUI application (`QApplication`). There is no CLI or headless interface, so a display is required to run it.

## Data is gitignored

The CIFAR-10 data lives under `x64/` and that directory is gitignored, so a fresh clone will NOT include it. For the default import path to resolve, the data must be placed at `x64/Release/cifar-10-binary/cifar-10-batches-bin/` (including `data_batch_1.bin`).

## Verified results (this machine)

- Configure: exit 0, Qt 5.15.18 found (Qt5Widgets and Qt5Charts).
- Build: exit 0, 9 translation units compiled, `build/FNN` produced.
- Binary: ELF 64-bit x86-64, dynamically linked against `libQt5Widgets.so.5` and `libQt5Charts.so.5`, zero missing libraries.
- GUI liveness smoke test: PASS (process still alive when killed at 10 s, exit 124).
