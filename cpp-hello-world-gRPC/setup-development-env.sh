#!/bin/bash

# A. Clone the gRPC repository (on this case v1.71.0)
git clone --recurse-submodules \
      -b v1.71.0 \
      --depth 1 \
      --shallow-submodules https://github.com/grpc/grpc

# B. Build and install gRPC
cd grpc
mkdir -p cmake/build
pushd cmake/build
cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_CXX_STANDARD=17 \
      -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
      ../..
make -j$(nproc)
sudo make install
popd