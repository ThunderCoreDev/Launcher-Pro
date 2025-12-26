# ThunderCore Launcher Pro ⚡

- El Launcher más Avanzado para Servidores WoW Privados
Potencia, Elegancia y Características Profesionales en un Solo Lugar

![Qt 6.5 ](https://img.shields.io/badge/Qt-6.5%2B-41CD52?logo=qt.svg)
![C++ 17 ](https://img.shields.io/badge/C++-17%2B-00599C?logo=c%2B%2B.svg)
![MySQL 8.0 ](https://img.shields.io/badge/MySQL-8.0%2B-4479A1?logo=mysql.svg)
![GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)
![Windows](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-informational.svg)

</div>

## 📖 Descripción General

ThunderCore Launcher Pro es una solución de software empresarial diseñada específicamente para servidores privados de World of Warcraft. Combina la potencia de C++/Qt con una arquitectura modular extensible para ofrecer una experiencia de usuario profesional comparable a Battle.net, pero con características específicas para comunidades de WoW.

## ✨ Características Principales

· ✅ Multi-expansión (Clásico hasta Dragonflight)
· ✅ Multi-emulador (TrinityCore, AzerothCore, CMaNGOS, etc.)
· ✅ Interfaz estilo Battle.Net con animaciones fluidas
· ✅ 150+ módulos y características integradas
· ✅ Base de datos independiente para funciones sociales
· ✅ Sistema de actualizaciones automáticas
· ✅ Completamente personalizable con temas CSS

## 🏗️ Arquitectura del Proyecto

```
ThunderCore-Launcher-Pro/
├── core/              # Núcleo del sistema
├── modules/           # Módulos intercambiables
├── database/          # Gestión de datos
├── ui/               # Interfaz gráfica
├── network/          # Comunicación
├── utils/            # Utilidades
├── resources/        # Recursos multimedia
├── scripts/          # Scripts SQL y automatización
└── config/           # Configuraciones
```

## 🎮 Módulos y Características Implementadas

🔐 Autenticación y Seguridad

· Sistema de login con animaciones estilo Battle.Net
· Autenticación SRP6 para todas las expansiones
· 2FA (Autenticación de Dos Factores)
· Historial de sesiones y detección de intrusiones
· Recuperación de cuenta segura

📊 Estadísticas en Tiempo Real

· Jugadores online por facción (Alianza/Horda)
· Uptime del servidor y latencia
· Top PvP (Arenas/BGs) del mes
· Top PvE (Raids/Mazmorras)
· Estado de instancias (raids lock)
· Gráficos de población por hora/día
· Heatmap de actividad del servidor

👥 Sistema Social Completo

· Mensajería privada entre usuarios
· Sistema de amigos con estados online
· Chat global del servidor
· Buscador de guilds que reclutan
· Sala de espera para dungeons/raids
· Perfiles públicos personalizables
· Comparativa con amigos

🎯 Gestión de Personajes

· Destrabar personajes atascados
· Calculadora de talentos interactiva
· Planificador de equipo (BiS lists)
· Comparador de estadísticas
· Generador de macros avanzadas
· Historial de personajes y progreso
· Colección de mounts/mascotas

🏪 Tienda y Economía

· Tienda de donaciones con carrito
· Sistema de cupones/descuentos
· Wishlist de items deseados
· Paquetes especiales (bundles)
· Historial de compras detallado
· Integración con múltiples pasarelas

📅 Eventos y Torneos

· Calendario de eventos integrado
· Countdown para eventos programados
· Inscripción automática a torneos
· Sistema de premios y recompensas
· Notificaciones push para eventos
· Stream integrado de eventos

🛠️ Herramientas Avanzadas

· Auto-parcheador inteligente
· Descarga automática del cliente
· Verificador de integridad de archivos
· Selector de addons integrado
· Limpiador de cache WDB automático
· Optimizador de configuración WoW
· Diagnosticador de problemas

🏆 Sistema de Progresión

· Login diario con recompensas acumulativas
· Niveles del launcher (XP por actividades)
· Insignias/títulos por logros
· Misiones del launcher diarias/semanales
· Cofres sorpresa aleatorios
· Programa de referidos (trae amigos)

🎨 Personalización Visual

· Temas personalizables (oscuro, claro, colores)
· Fondos animados personalizados
· Sonidos personalizables
· Widgets arrastrables
· Efectos de partículas
· Modo compacto/expandido
· Ajustes de transparencia

👑 Panel de Administración

· Gestor de tickets de soporte
· Sistema de anuncios push masivos
· Ban manager visual
· Monitor de actividad sospechosa
· Log viewer en tiempo real
· Herramienta de restauración de personajes
· Estadísticas detalladas del servidor

## 📋 Pre-requisitos de Instalación

🖥️ Para Windows

```powershell
# Requerimientos mínimos
- Windows 10/11 (64-bit)
- 4GB RAM mínimo (8GB recomendado)
- 2GB espacio en disco
- Visual C++ Redistributable 2019+
- MySQL Server 8.0+
- Qt 6.5.0+ (incluido en el instalador)
```

🐧 Para Linux (Ubuntu/Debian)

```bash
# Dependencias del sistema
sudo apt update
sudo apt install -y \
    build-essential \
    cmake \
    qt6-base-dev \
    qt6-multimedia-dev \
    qt6-webengine-dev \
    qt6-charts-dev \
    libmysqlclient-dev \
    git \
    wget \
    unzip
```

🍎 Para macOS

```bash
# Requerimientos
- macOS 11.0+
- Homebrew instalado
- Xcode Command Line Tools

# Instalar dependencias
brew install qt@6
brew install mysql-client
brew install cmake
```

## 🗄️ Requerimientos de Base de Datos

```sql
-- MySQL 8.0+ o MariaDB 10.5+
-- Permisos necesarios:
- CREATE, ALTER, DROP en bases de datos
- INSERT, UPDATE, DELETE, SELECT en tablas
- CREATE USER, GRANT OPTION (para configuración)
-- Espacio mínimo: 500MB
```

## 🚀 Instalación Rápida

Método 1: Script Automático (Recomendado)

```bash
# Descargar el script de instalación
wget https://raw.githubusercontent.com/ThunderCoreDev/Launcher-Pro/main/scripts/install.sh

# Hacerlo ejecutable
chmod +x install.sh

# Ejecutar instalación
sudo ./install.sh

```

Método 2: Instalación Manual

```bash
# 1. Clonar el repositorio
git clone --recursive https://github.com/ThunderCoreDev/Launcher-Pro.git
cd Launcher-Pro

# 2. Configurar base de datos
mysql -u root -p < scripts/database/setup.sql

# 3. Compilar
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# 4. Instalar
sudo make install

# 5. Configurar
cp ../config/default.ini ~/.config/ThunderCore/config.ini
```

Método 3: Docker (Para producción)

```yaml
# docker-compose.yml
version: '3.8'
services:
  launcher-db:
    image: mysql:8.0
    environment:
      MYSQL_ROOT_PASSWORD: ThunderCore2026!
      MYSQL_DATABASE: launcher
      MYSQL_USER: launcher_user
      MYSQL_PASSWORD: ThunderCore2026!
    volumes:
      - launcher_data:/var/lib/mysql
  
  launcher-app:
    build: .
    ports:
      - "8080:80"
    depends_on:
      - launcher-db
    volumes:
      - ./config:/app/config
      - ./resources:/app/resources

volumes:
  launcher_data:
```

## ⚙️ Configuración Inicial

1. Configurar Conexiones de Base de Datos

```ini
# config/database.ini
[Launcher]
Host=127.0.0.1
Port=3306
User=launcher_user
Password=ThunderCore2026!
Database=launcher

[Auth]
Host=127.0.0.1
Port=3306
User=auth_user
Password=auth_password
Database=auth

[Characters]
Host=127.0.0.1
Port=3306
User=chars_user
Password=chars_password
Database=characters

[World]
Host=127.0.0.1
Port=3306
User=world_user
Password=world_password
Database=world
```

2. Configurar Expansión y Emulador

```bash
# El launcher detecta automáticamente, pero puedes forzar:
./ThunderCore-Launcher-Pro --expansion WOTLK --emulator TrinityCore
```

3. Configurar Rutas del Juego

```bash
# En el launcher, ve a:
Ajustes → Juego → Ruta del Cliente

# O edita manualmente:
~/.config/ThunderCore/config.ini
[Game]
DefaultPath=/ruta/a/tu/WoW
Realmlist=logon.tuserver.com
```

## 📚 Documentación Adicional

🔧 Comandos Avanzados

```bash
# Modo debug
./ThunderCore-Launcher-Pro --debug --log-level=verbose

# Resetear configuración
./ThunderCore-Launcher-Pro --reset-config

# Verificar instalación
./ThunderCore-Launcher-Pro --verify

# Actualizar manualmente
./ThunderCore-Launcher-Pro --update

# Generar informe de diagnóstico
./ThunderCore-Launcher-Pro --diagnose
```

## 🎨 Personalización

Crear un Tema Personalizado

```css
/* resources/themes/mi-tema.qss */
QMainWindow {
    background: #1a1a2e;
    border: 2px solid #00ff88;
}

QPushButton {
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
        stop:0 #00ff88, stop:1 #00cc66);
    border-radius: 10px;
}
```

Configurar Widgets Personalizados

```json
{
  "dashboard": {
    "widgets": ["stats", "news", "events", "friends"],
    "layout": "grid",
    "columns": 3,
    "enableDragDrop": true
  },
  "notifications": {
    "position": "top-right",
    "duration": 5000,
    "sound": "custom_notification.wav"
  }
}
```

## 🐛 Solución de Problemas

Problemas Comunes

Problema Solución
Error de conexión a MySQL Verificar credenciales y puerto 3306
Launcher no se inicia Instalar VC++ Redistributable
Texto distorsionado Actualizar drivers de gráficos
Sin sonido Verificar Qt Multimedia
Lento al cargar Aumentar memoria en config.ini

Logs y Diagnóstico

```bash
# Ver logs en tiempo real
tail -f ~/.local/share/ThunderNet/logs/launcher.log

# Limpiar cache
rm -rf ~/.cache/ThunderNet

# Reinstalar dependencias
scripts/repair.sh --reinstall-deps
```

## 🤝 Contribuir al Proyecto

¡Nos encantan las contribuciones! Sigue estos pasos:

1. Fork el repositorio
2. Crea una rama (git checkout -b feature/AmazingFeature)
3. Commit tus cambios (git commit -m 'Add AmazingFeature')
4. Push a la rama (git push origin feature/AmazingFeature)
5. Abre un Pull Request

## Guía para Desarrolladores

```bash
# Configurar entorno de desarrollo
git clone https://github.com/ThunderCoreDev/Launcher-Pro.git
cd Launcher-Pro
./scripts/setup-dev.sh

# Ejecutar tests
./scripts/run-tests.sh

# Formatear código
./scripts/format-code.sh

# Construir documentación
./scripts/build-docs.sh
```

## 📄 Licencia

Este proyecto está licenciado bajo la GNU General Public License v3.0 - ver el archivo LICENSE para más detalles.

```
ThunderCore Launcher Pro
Copyright (C) 2026 ThunderNet WoW

Este programa es software libre: puedes redistribuirlo y/o modificar
bajo los términos de la GNU General Public License publicada por
la Free Software Foundation, ya sea la versión 3 de la Licencia, o
(a su elección) cualquier versión posterior.
```

## 👥 Equipo y Créditos

Desarrolladores Principales

· Lead Developer: [ThunderCoreDev]

## Tecnologías Utilizadas

· Qt 6: Framework para la interfaz gráfica
· MySQL 8: Base de datos principal
· CMake: Sistema de construcción
· JSON for Modern C++: Manipulación JSON
· spdlog: Sistema de logging
· FontAwesome: Iconos

## Agradecimientos Especiales

· Comunidad de TrinityCore y AzerothCore
· Todos los testers beta que ayudaron a mejorar
· La increíble comunidad de ThunderCore WoW

## 📞 Soporte y Contacto

· Email: devthundercore@gmail.com
· Issues de GitHub: Reportar Bug

## Estado del Servicio

![](https://img.shields.io/badge/Estado-Activo-green.svg)
![](https://img.shields.io/badge/Actualizaciones-Periodicas-rose.svg)

## 🌟 Características Próximas

En Desarrollo

· App móvil companion
· Integración con Discord Rich Presence
· Sistema de clans cross-server
· Marketplace de items entre jugadores
· AI Assistant para nuevos jugadores

Planeado

· Replays de arenas/BGs
· Simulador de raids 3D
· Sistema de torneos automático
· Machine Learning para detección de trampas

---

<div align="center">

¡Únete a la Revolución de los Launchers para WoW!

```
¿Te gusta el proyecto? ¡Dale una estrella! ⭐
```

</div>
