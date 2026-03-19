#!/usr/bin/env sh

set -eux

if [ ! -d "build" ]; then
  mkdir "build"
fi

cd "build"
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . -j `nproc`
cd ..
