#!/bin/bash
echo "=== ThunderCore Launcher Pro Build Script ==="

# Crear estructura de directorios
mkdir -p build
mkdir -p resources/images/icons
mkdir -p resources/styles
mkdir -p config

# Crear archivos de recursos mínimos si no existen
if [ ! -f resources/images/logo.png ]; then
    echo "Creating placeholder logo..."
    convert -size 256x256 xc:#00aeff -fill white -pointsize 50 -gravity center \
            -draw "text 0,0 'TN'" resources/images/logo.png 2>/dev/null || \
    echo "Note: Install ImageMagick for better placeholder generation"
fi

if [ ! -f resources/styles/theme_battlenet.qss ]; then
    echo "Creating default theme..."
    cat > resources/styles/theme_battlenet.qss << 'EOF'
QMainWindow { background: #0c1a2d; }
QPushButton { background: #00aeff; color: white; }
QLabel { color: #aaccff; }
EOF
fi

if [ ! -f config/default.ini ]; then
    echo "Creating default config..."
    cat > config/default.ini << 'EOF'
[Application]
Name=ThunderCoreLauncher Pro
Version=1.0.0

[Database]
Host=127.0.0.1
Port=3306
User=ThunderNet
Password=A990302**//
EOF
fi

# Entrar en directorio de construcción
cd build

# Configurar CMake
echo "Configuring CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Compilar
echo "Compiling..."
make -j$(nproc)

# Verificar que el ejecutable existe
if [ -f bin/ThunderCore-Launcher-Pro ]; then
    echo "=== BUILD SUCCESSFUL ==="
    echo "Executable: build/bin/ThunderCore-Launcher-Pro"
else
    echo "=== BUILD FAILED ==="
    echo "Check CMake output for errors"
    exit 1
fi
