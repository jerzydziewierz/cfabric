#!/bin/bash

# try to delete the old build directory
rm -rf build || true

# Exit immediately if a command exits with a non-zero status.
set -e


# Navigate to the project root directory
# The script assumes it's in the project root
cd "$(dirname "$0")"

echo "Building demo3..."

# Create build directory if it doesn't exist
mkdir -p build

# Navigate to build directory
cd build

echo "Configuring with CMake..."
# Configure the project with CMake
cmake ..

# Check if Makefile exists
if [ ! -f "Makefile" ]; then
    echo "Error: Makefile not generated. CMake configuration might have failed."
    exit 1
fi

echo "Building the project..."
# Build the project using cmake --build
cmake --build .

echo "Running demo3..."
# Run demo3
./demo3

# Return to the original directory
cd ..

echo "Demo3 execution completed."
