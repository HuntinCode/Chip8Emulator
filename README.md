# CHIP-8 Emulator

A CHIP-8 emulator written in C++ using SDL3 for window creation, rendering, and input handling.

## Overview

This project is an implementation of the CHIP-8 virtual machine, following the guide written by **Tobias V. Langhoff**.

The goal of this project was to explore emulator development, understand how a virtual machine operates, and gain practical experience working with low-level concepts such as memory, registers, opcodes, and instruction execution.

Currently, the emulator supports CHIP-8 only. Other systems and extensions are outside the scope of this implementation.

## Features

- CHIP-8 instruction set implementation.
- Virtual machine memory and register management.
- Graphics rendering through SDL3.
- Keyboard input handling.
- ROM execution.

## Dependencies

- **C++** – Core implementation.
- **CMake** – Build system configuration.
- **SDL3** – Window management, graphics rendering, and input handling.

## Building

### Prerequisites

Before building, make sure you have the following installed:

- A C++ compiler supporting C++17 or later.
- CMake.
- SDL3 development libraries.

### Build Instructions

Clone the repository:

```bash
git clone <repository-url>
cd <repository-directory>
```

Create a build directory:

```bash
cmake -S . -B build
```

Build the project:

```bash
cmake --build build
```

The resulting executable will be generated in the build directory (or its configuration-specific subdirectory, depending on your build environment).

**Note:** SDL3 must be installed and discoverable by CMake for configuration to succeed.

## Current Limitations

- **No audio support:** The emulator currently has no sound implementation. There are no plans to implement audio support.
- **Hardcoded ROM path:** The ROM file path must currently be specified directly in the source code. A more convenient way to select and load ROMs is planned for a future update.
- **CHIP-8 only:** Only the original CHIP-8 system is supported. Other variants and extensions are not currently implemented.

## Credits

- **[Tobias V. Langhoff](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)** – CHIP-8 emulator development guide, which served as the primary reference for this project.
- **[SDL](https://www.libsdl.org/)** – Cross-platform multimedia library used for rendering and input handling.

## License

See the repository for license information.
