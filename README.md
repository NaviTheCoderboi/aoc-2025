# Advent of Code — C++ Solutions

This repository contains my Advent of Code solutions implemented in C++.

- Each day lives under `src/dayNN/` and typically contains a `main.cpp` and an `input.txt` with the puzzle input.
- Common utilities are kept in `src/common/` and shared via the `aoc_common` target.

Quick overview:

- Purpose: provide small, focused solutions for each Advent of Code puzzle written in modern C++.
- Language: C++ (CMake as the build system).
- Structure: `src/dayNN/` for each day; top-level `CMakeLists.txt` configures targets.

## Build and run

Create a build directory and build the project with CMake:

```bash
cmake -S . -B build
cmake --build build
```

After building, executables for each day are under the build tree (e.g. `build/src/day01/day01`).

Run a day's executable from the build directory. Many day CMake files copy the day's `input.txt` into the build dir so the program can open `input.txt` directly:

```bash
./build/src/day01/day01
```

## Notes and tips

- Inputs: keep `input.txt` beside the corresponding `main.cpp` in `src/dayNN/`.
- If you want the build artifacts in a custom folder (for example `out/`), edit the top-level `CMakeLists.txt` to set `CMAKE_RUNTIME_OUTPUT_DIRECTORY` or adjust target properties.
- To test a single day quickly, you can run the corresponding executable in the build tree; CMake targets often use `configure_file()` to copy inputs into the build directory automatically.
