#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
set -e

# Navigate to the project root directory
# The script assumes it's run from the project root
cd "$(dirname "$0")"

# Create build directory if it doesn't exist
mkdir -p build

# Navigate to build directory
cd build

# Configure the project with CMake
cmake ..

# Build the project
make

# Run demo3
./demo3

# Return to the original directory
cd ..

echo "Demo3 execution completed."
