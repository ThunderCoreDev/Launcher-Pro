cmake_minimum_required(VERSION 3.16)
project(ThunderCore-Launcher-Pro VERSION 1.0.0 LANGUAGES CXX)

# Configurar Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Buscar Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Sql)

# Configurar compilador
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra)
endif()

# Archivos fuente
set(SOURCE_FILES
    src/main.cpp
    core/Application.cpp
    core/Settings.cpp
    core/Logger.cpp
    ui/MainWindow.cpp
    ui/LoginWindow.cpp
    ui/DashboardWindow.cpp
    database/DatabaseManager.cpp
    database/LauncherDatabase.cpp
    network/ApiClient.cpp
    network/WebSocketClient.cpp
    utils/FileUtils.cpp
)

# Archivos de cabecera
set(HEADER_FILES
    core/Application.h
    core/Settings.h
    core/Logger.h
    core/Singleton.h
    core/Global.h
    ui/MainWindow.h
    ui/LoginWindow.h
    ui/DashboardWindow.h
    database/DatabaseManager.h
    database/LauncherDatabase.h
    network/ApiClient.h
    network/WebSocketClient.h
    utils/FileUtils.h
)

# Archivos de recursos
set(RESOURCE_FILES
    resources/resources.qrc
)

# Crear ejecutable
add_executable(ThunderCore-Launcher-Pro 
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${RESOURCE_FILES}
)

# Enlazar bibliotecas
target_link_libraries(ThunderCore-Launcher-Pro
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Sql
)

# Configuración específica de Windows
if(WIN32)
    target_link_libraries(ThunderCore-Launcher-Pro ws2_32)
endif()

# Directorio de salida
set_target_properties(ThunderCore-Launcher-Pro PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    OUTPUT_NAME "ThunderCore-Launcher-Pro"
)

message(STATUS "Project configured successfully")
