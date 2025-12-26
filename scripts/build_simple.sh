#!/bin/bash

echo "Building ThunderCore Launcher Pro..."

# Crear directorio de construcción
mkdir -p build
cd build

# Configurar con CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Compilar
make -j$(nproc)

# Copiar recursos
cp -r ../config/ bin/
cp -r ../resources/ bin/

echo "Build complete! Executable is in: build/bin/"