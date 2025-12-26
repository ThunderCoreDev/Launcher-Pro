#!/bin/bash
# Script de instalación automática para ThunderNet Launcher Pro

echo "========================================="
echo "  INSTALACIÓN THUNDERCORE LAUNCHER PRO    "
echo "========================================="

# Variables
LAUNCHER_NAME="ThunderCore-Launcher-Pro"
INSTALL_DIR="/opt/$LAUNCHER_NAME"
DESKTOP_FILE="$HOME/.local/share/applications/thundercore-launcher.desktop"
APP_ICON="$INSTALL_DIR/resources/icon.png"

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Verificar dependencias
check_dependencies() {
    echo -e "${BLUE}[1/8]${NC} Verificando dependencias..."
    
    local missing_deps=()
    
    # Qt
    if ! command -v qmake &> /dev/null; then
        missing_deps+=("qt6-base-dev")
    fi
    
    # MySQL
    if ! command -v mysql &> /dev/null; then
        missing_deps+=("mysql-client")
    fi
    
    # GCC
    if ! command -v g++ &> /dev/null; then
        missing_deps+=("g++")
    fi
    
    # Git
    if ! command -v git &> /dev/null; then
        missing_deps+=("git")
    fi
    
    if [ ${#missing_deps[@]} -ne 0 ]; then
        echo -e "${YELLOW}Dependencias faltantes:${NC}"
        printf '%s\n' "${missing_deps[@]}"
        
        read -p "¿Instalar automáticamente? (y/n): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            sudo apt update
            sudo apt install -y "${missing_deps[@]}"
        else
            echo -e "${RED}Instalación abortada.${NC}"
            exit 1
        fi
    fi
}

# Clonar repositorio
clone_repository() {
    echo -e "${BLUE}[2/8]${NC} Descargando código fuente..."
    
    if [ -d "$INSTALL_DIR" ]; then
        echo -e "${YELLOW}El directorio ya existe. Actualizando...${NC}"
        cd "$INSTALL_DIR"
        git pull origin master
    else
        git clone https://github.com/thundernet-wow/launcher-pro.git "$INSTALL_DIR"
    fi
    
    if [ $? -ne 0 ]; then
        echo -e "${RED}Error al descargar el código fuente.${NC}"
        exit 1
    fi
}

# Configurar base de datos
setup_database() {
    echo -e "${BLUE}[3/8]${NC} Configurando base de datos..."
    
    read -sp "Contraseña root de MySQL: " mysql_root_pass
    echo
    
    # Crear base de datos del launcher
    mysql -u root -p"$mysql_root_pass" <<EOF
CREATE DATABASE IF NOT EXISTS launcher CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
CREATE USER IF NOT EXISTS 'launcher_user'@'localhost' IDENTIFIED BY 'ThunderCore2024!';
GRANT ALL PRIVILEGES ON launcher.* TO 'launcher_user'@'localhost';
FLUSH PRIVILEGES;
EOF
    
    # Ejecutar scripts SQL
    cd "$INSTALL_DIR/scripts"
    mysql -u launcher_user -p'ThunderNet2024!' launcher < launcher_database.sql
    
    # Configurar conexión
    cat > "$INSTALL_DIR/config/database.ini" <<EOF
[Launcher]
Host=127.0.0.1
Port=3306
User=launcher_user
Password=ThunderNet2024!
Database=launcher

[Auth]
Host=127.0.0.1
Port=3306
User=ThunderNet
Password=A990302**//

[Characters]
Host=127.0.0.1
Port=3306
User=ThunderNet
Password=A990302**//

[World]
Host=127.0.0.1
Port=3306
User=ThunderNet
Password=A990302**//
EOF
}

# Compilar launcher
compile_launcher() {
    echo -e "${BLUE}[4/8]${NC} Compilando launcher..."
    
    cd "$INSTALL_DIR"
    mkdir -p build
    cd build
    
    qmake ..
    make -j$(nproc)
    
    if [ $? -ne 0 ]; then
        echo -e "${RED}Error en la compilación.${NC}"
        exit 1
    fi
}

# Instalar recursos
install_resources() {
    echo -e "${BLUE}[5/8]${NC} Instalando recursos..."
    
    # Descargar recursos adicionales
    wget -O "$INSTALL_DIR/resources/backgrounds.zip" \
        https://cdn.thundercore.com/launcher/backgrounds.zip
    unzip -o "$INSTALL_DIR/resources/backgrounds.zip" -d "$INSTALL_DIR/resources/"
    
    # Descargar iconos
    wget -O "$INSTALL_DIR/resources/icons.zip" \
        https://cdn.thundercore.com/launcher/icons.zip
    unzip -o "$INSTALL_DIR/resources/icons.zip" -d "$INSTALL_DIR/resources/"
    
    # Descargar sonidos
    wget -O "$INSTALL_DIR/resources/sounds.zip" \
        https://cdn.thundercore.com/launcher/sounds.zip
    unzip -o "$INSTALL_DIR/resources/sounds.zip" -d "$INSTALL_DIR/resources/"
}

# Crear acceso directo
create_desktop_file() {
    echo -e "${BLUE}[6/8]${NC} Creando acceso directo..."
    
    cat > "$DESKTOP_FILE" <<EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=ThunderCore Launcher Pro
Comment=Advanced WoW Server Launcher
Exec=$INSTALL_DIR/build/ThunderCore-Launcher-Pro
Icon=$APP_ICON
Terminal=false
Categories=Game;
StartupWMClass=ThunderCore-Launcher-Pro
Keywords=wow;launcher;gaming;thundernet
EOF
    
    chmod +x "$DESKTOP_FILE"
}

# Configurar auto-arranque
setup_autostart() {
    echo -e "${BLUE}[7/8]${NC} Configurando auto-arranque..."
    
    read -p "¿Iniciar automáticamente con el sistema? (y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        AUTOSTART_DIR="$HOME/.config/autostart"
        mkdir -p "$AUTOSTART_DIR"
        cp "$DESKTOP_FILE" "$AUTOSTART_DIR/"
    fi
}

# Configurar servicio de actualizaciones
setup_update_service() {
    echo -e "${BLUE}[8/8]${NC} Configurando servicio de actualizaciones..."
    
    # Crear script de actualización
    cat > "$INSTALL_DIR/update_launcher.sh" <<'EOF'
#!/bin/bash
cd /opt/ThunderCore-Launcher-Pro
git pull origin master
cd build
make -j$(nproc)
echo "Launcher actualizado: $(date)" >> /var/log/thundernet-updates.log
EOF
    
    chmod +x "$INSTALL_DIR/update_launcher.sh"
    
    # Agregar a cron (actualización diaria a las 3 AM)
    (crontab -l 2>/dev/null; echo "0 3 * * * $INSTALL_DIR/update_launcher.sh") | crontab -
}

# Función principal
main() {
    echo -e "${GREEN}Iniciando instalación de ThunderCore Launcher Pro${NC}"
    
    check_dependencies
    clone_repository
    setup_database
    compile_launcher
    install_resources
    create_desktop_file
    setup_autostart
    setup_update_service
    
    echo -e "${GREEN}=========================================${NC}"
    echo -e "${GREEN}  INSTALACIÓN COMPLETADA EXITOSAMENTE    ${NC}"
    echo -e "${GREEN}=========================================${NC}"
    echo ""
    echo -e "${YELLOW}Información importante:${NC}"
    echo "• Launcher instalado en: $INSTALL_DIR"
    echo "• Acceso directo creado en el menú de aplicaciones"
    echo "• Base de datos configurada: launcher"
    echo "• Usuario BD: launcher_user"
    echo "• Para iniciar: ThunderCore Launcher Pro"
    echo ""
    echo -e "${BLUE}Configuración inicial requerida:${NC}"
    echo "1. Abre el launcher"
    echo "2. Configura la ruta de tu cliente WoW"
    echo "3. Selecciona expansión y emulador"
    echo "4. ¡Listo para jugar!"
    
    # Preguntar si iniciar ahora
    read -p "¿Iniciar el launcher ahora? (y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        "$INSTALL_DIR/build/ThunderCore-Launcher-Pro" &
    fi
}

# Ejecutar instalación
main