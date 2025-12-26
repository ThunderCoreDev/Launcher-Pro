#!/bin/bash
# Script de construcción para ThunderCore Launcher Pro

set -e

echo "=========================================="
echo "  CONSTRUYENDO THUNDERCORE LAUNCHER PRO    "
echo "=========================================="

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Configuración
BUILD_TYPE="Release"
BUILD_DIR="build"
INSTALL_DIR="/opt/thundercore-launcher"
THREADS=$(nproc)

# Función para mostrar mensajes
print_status() {
    echo -e "${BLUE}[$(date +%H:%M:%S)]${NC} $1"
}

print_success() {
    echo -e "${GREEN}✓${NC} $1"
}

print_error() {
    echo -e "${RED}✗${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}!${NC} $1"
}

# Verificar dependencias
check_dependencies() {
    print_status "Verificando dependencias..."
    
    local missing=()
    
    # Qt
    if ! command -v qmake6 &> /dev/null && ! command -v qmake-qt6 &> /dev/null; then
        missing+=("qt6-base-dev")
    fi
    
    # CMake
    if ! command -v cmake &> /dev/null; then
        missing+=("cmake")
    fi
    
    # GCC/G++
    if ! command -v g++ &> /dev/null; then
        missing+=("g++")
    fi
    
    # MySQL
    if ! mysql_config --version &> /dev/null; then
        missing+=("libmysqlclient-dev")
    fi
    
    if [ ${#missing[@]} -ne 0 ]; then
        print_warning "Dependencias faltantes: ${missing[*]}"
        read -p "¿Instalar automáticamente? (y/n): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            sudo apt update
            sudo apt install -y "${missing[@]}"
        else
            print_error "Dependencias necesarias no instaladas"
            exit 1
        fi
    fi
    
    print_success "Dependencias verificadas"
}

# Limpiar construcción anterior
clean_build() {
    print_status "Limpiando construcción anterior..."
    
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
        print_success "Directorio de construcción limpiado"
    fi
    
    mkdir -p "$BUILD_DIR"
}

# Configurar CMake
configure_cmake() {
    print_status "Configurando CMake..."
    
    cd "$BUILD_DIR"
    
    local qt_path=$(which qmake6 2>/dev/null || which qmake-qt6 2>/dev/null || which qmake 2>/dev/null)
    if [ -z "$qt_path" ]; then
        print_error "Qt no encontrado"
        exit 1
    fi
    
    cmake .. \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
        -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR" \
        -DCMAKE_CXX_COMPILER=g++ \
        -DCMAKE_C_COMPILER=gcc \
        -DQt6_DIR=$(dirname "$qt_path")/../lib/cmake/Qt6 \
        -DMySQL_INCLUDE_DIR=$(mysql_config --include | sed 's/-I//') \
        -DMySQL_LIBRARY=$(mysql_config --libs | awk '{print $1}' | sed 's/-l//') \
        -Wno-dev
    
    if [ $? -eq 0 ]; then
        print_success "CMake configurado correctamente"
    else
        print_error "Error en configuración de CMake"
        exit 1
    fi
    
    cd ..
}

# Compilar
compile() {
    print_status "Compilando..."
    
    cd "$BUILD_DIR"
    
    make -j"$THREADS"
    
    if [ $? -eq 0 ]; then
        print_success "Compilación completada"
    else
        print_error "Error en la compilación"
        exit 1
    fi
    
    cd ..
}

# Ejecutar pruebas
run_tests() {
    if [ -f "$BUILD_DIR/bin/ThunderCore-Launcher-Pro" ]; then
        print_status "Probando ejecutable..."
        
        # Verificar que el ejecutable se puede iniciar
        timeout 5 "$BUILD_DIR/bin/ThunderCore-Launcher-Pro" --version 2>&1 | grep -q "ThunderCore"
        
        if [ $? -eq 0 ]; then
            print_success "Prueba de ejecución exitosa"
        else
            print_warning "El ejecutable no mostró la versión esperada"
        fi
    fi
}

# Instalar
install_app() {
    print_status "Instalando..."
    
    cd "$BUILD_DIR"
    
    sudo make install
    
    if [ $? -eq 0 ]; then
        print_success "Instalación completada en $INSTALL_DIR"
    else
        print_warning "No se pudo instalar globalmente (instalando localmente)"
        mkdir -p ~/.local/bin
        cp bin/ThunderNet-Launcher-Pro ~/.local/bin/
        print_success "Instalado en ~/.local/bin/"
    fi
    
    cd ..
}

# Crear acceso directo
create_desktop_file() {
    print_status "Creando acceso directo..."
    
    local desktop_file="$HOME/.local/share/applications/thundercore-launcher.desktop"
    
    cat > "$desktop_file" << EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=ThunderCore Launcher Pro
Comment=Advanced WoW Server Launcher
Exec=$INSTALL_DIR/bin/ThunderCore-Launcher-Pro
Icon=$INSTALL_DIR/resources/images/icon.png
Terminal=false
Categories=Game;
StartupWMClass=ThunderCore-Launcher-Pro
Keywords=wow;launcher;gaming;thundernet
EOF
    
    chmod +x "$desktop_file"
    print_success "Acceso directo creado"
}

# Función principal
main() {
    print_status "Iniciando construcción de ThunderNet Launcher Pro"
    
    check_dependencies
    clean_build
    configure_cmake
    compile
    run_tests
    
    read -p "¿Instalar la aplicación? (y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        install_app
        create_desktop_file
    fi
    
    print_success "¡Construcción completada exitosamente!"
    echo ""
    echo "Para ejecutar:"
    echo "  $BUILD_DIR/bin/ThunderCore-Launcher-Pro"
    echo ""
    echo "O si instalaste:"
    echo "  ThunderCore-Launcher-Pro"
    echo ""
    echo "¡Disfruta de ThunderCore Launcher Pro!"
}

# Ejecutar
main