#!/bin/bash

# get exepath
getExePathFromCMakeLists() {
  local content
  local exeName=""

  content=$(cat "./CMakeLists.txt")

  while IFS= read -r line; do
    if [[ $line == add_executable* ]]; then
      local index=$(echo $line | grep -b -o '(' | cut -d: -f1)
      exeName=$(echo $line | cut -d' ' -f2)
      break
    fi
  done <<<"$content"

  local exePath="./build/$exeName"
  echo "$exePath"
}

# get currentDirectory
currentDirectory=$(pwd)
cmakeListsPath="$currentDirectory/CMakeLists.txt"

if [ ! -f "$cmakeListsPath" ]; then
  echo "No CMakeLists.txt in current directory, please check."
  exit 1
fi

echo "Start generating and compiling..."

# set the path of build directory
buildFolderPath="./build"

if [ ! -d "$buildFolderPath" ]; then
  mkdir "$buildFolderPath"
  echo "build folder created."
fi

# use cmake to generate project
cmake -G "Unix Makefiles" -S . -B ./build/
if [ $? -eq 0 ]; then
  cmake --build ./build/
  if [ $? -eq 0 ]; then
    exePath=$(getExePathFromCMakeLists)
    echo "start running as follows..."
    echo "=================================================="
    ./build/cpp-dsa
  else
    echo "Build failed."
    exit 1
  fi
else
  echo "CMake generation failed."
  exit 1
fi
