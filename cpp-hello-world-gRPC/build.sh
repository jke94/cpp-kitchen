#!/bin/bash

# Clonar el repositorio
git clone --recurse-submodules -b v1.62.0 https://github.com/grpc/grpc
cd grpc

# Crear carpeta de build
mkdir -p cmake/build
cd cmake/build

# Configurar y compilar
cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX=/usr/local \
      ../..

make -j$(nproc)
sudo make install