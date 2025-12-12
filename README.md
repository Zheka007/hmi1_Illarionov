# XSmall-HMI Editor

Visual objects editor for HMI applications with variable database binding.

## Features

- Create visual objects:
  - Rectangle
  - Line
  - Polyline
  - Text
  - Button
  - Input Field
  - History Graph
  - Image
- Bind object properties to variables
- Tool palette
- Variable database with change subscription

## Build

### Automatic Build (Recommended)

The project will automatically download and build SFML 3.0.2 and Google Test:

```bash
# Create build directory
mkdir build
cd build

# Configure and build
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..
cmake --build .

# Or for Debug mode
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .# hmi1_Illarionov
