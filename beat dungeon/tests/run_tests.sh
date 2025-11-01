#!/bin/bash
# Script to build and run tests on Unix-like systems

echo "Building Beat Dungeon Tests..."

# Create build directory
mkdir -p build
cd build

# Configure CMake
cmake ..

# Build
cmake --build .

# Run tests
echo "Running tests..."
./BeatDungeonTests

# Run with CTest for better output
echo ""
echo "Running tests with CTest..."
ctest --output-on-failure

