cmake_minimum_required(VERSION 3.16)
project(ThunderCore-Launcher-Pro VERSION 1.0.0 LANGUAGES CXX)

# Configuración básica
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configuración de Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Buscar Qt6
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Network
    Sql
    Multimedia
    WebSockets
    Concurrent
)

# Buscar MySQL
find_package(MySQL REQUIRED)

# Opciones de construcción
option(BUILD_WITH_DEBUG "Build with debug symbols" OFF)
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_DOCS "Build documentation" OFF)
option(INSTALL_SYSTEM_WIDE "Install system-wide" ON)

# Configuración del compilador
if(MSVC)
    add_compile_options(/W4 /permissive- /MP)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter)
endif()

# Directorios de inclusión
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/database
    ${CMAKE_CURRENT_SOURCE_DIR}/network
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${MySQL_INCLUDE_DIRS}
)

# Archivos fuente principales
set(MAIN_SOURCES
    src/main.cpp
)

# Archivos del núcleo
set(CORE_SOURCES
    core/Application.cpp
    core/Settings.cpp
    core/Logger.cpp
)

set(CORE_HEADERS
    core/Application.h
    core/Settings.h
    core/Logger.h
    core/Singleton.h
)

# Archivos de UI
set(UI_SOURCES
    ui/MainWindow.cpp
    ui/LoginWindow.cpp
    ui/DashboardWindow.cpp
)

set(UI_HEADERS
    ui/MainWindow.h
    ui/LoginWindow.h
    ui/DashboardWindow.h
)

# Archivos de base de datos
set(DATABASE_SOURCES
    database/DatabaseManager.cpp
)

set(DATABASE_HEADERS
    database/DatabaseManager.h
)

# Archivos de recursos
set(RESOURCE_FILES
    resources/resources.qrc
)

# Combinar todas las fuentes
set(ALL_SOURCES
    ${MAIN_SOURCES}
    ${CORE_SOURCES}
    ${UI_SOURCES}
    ${DATABASE_SOURCES}
    ${RESOURCE_FILES}
)

set(ALL_HEADERS
    ${CORE_HEADERS}
    ${UI_HEADERS}
    ${DATABASE_HEADERS}
)


# Crear ejecutable
add_executable(ThunderCore-Launcher-Pro
    ${ALL_SOURCES}
    ${ALL_HEADERS}
    ${ALL_RESOURCES}
)

# Enlazar bibliotecas
target_link_libraries(ThunderCore-Launcher-Pro
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Sql
    Qt6::Multimedia
    Qt6::WebEngineWidgets
    Qt6::WebChannel
    Qt6::WebSockets
    Qt6::Charts
    Qt6::Concurrent
    Qt6::PrintSupport
    ${MySQL_LIBRARIES}
    Threads::Threads
)

# Configuración específica de Windows
if(WIN32)
    target_link_libraries(ThunderCore-Launcher-Pro
        ws2_32
        crypt32
        winmm
    )
    
    # Configurar icono
    if(EXISTS "${CMAKE_SOURCE_DIR}/resources/images/icon.ico")
        set_target_properties(ThunderCore-Launcher-Pro PROPERTIES
            WIN32_EXECUTABLE TRUE
            RC_ICONS "${CMAKE_SOURCE_DIR}/resources/images/icon.ico"
        )
    endif()
endif()

# Configuración específica de macOS
if(APPLE)
    set_target_properties(ThunderCore-Launcher-Pro PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.thundercore.launcher"
        MACOSX_BUNDLE_BUNDLE_NAME "ThunderCore Launcher Pro"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/cmake/macos/Info.plist"
    )
    
    # Añadir frameworks necesarios para macOS
    target_link_libraries(ThunderCore-Launcher-Pro
        "-framework CoreFoundation"
        "-framework Security"
        "-framework SystemConfiguration"
    )
endif()

# Configurar salida
set_target_properties(ThunderCore-Launcher-Pro PROPERTIES
    OUTPUT_NAME "ThunderCore-Launcher-Pro"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Copiar archivos de configuración post-build
add_custom_command(TARGET ThunderCore-Launcher-Pro POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/config"
    "$<TARGET_FILE_DIR:ThunderCore-Launcher-Pro>/../config"
    COMMENT "Copying configuration files..."
)
# Copiar recursos
add_custom_command(TARGET ThunderCore-Launcher-Pro POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/resources"
    "$<TARGET_FILE_DIR:ThunderCore-Launcher-Pro>/../resources"
    COMMENT "Copying resource files..."
)

# Instalación
install(TARGETS ThunderCore-Launcher-Pro
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION Applications
)

install(DIRECTORY config/ DESTINATION config)
install(DIRECTORY resources/ DESTINATION resources)

# Mensaje de éxito
message(STATUS "==========================================")
message(STATUS " ThunderCore Launcher Pro configured")
message(STATUS "==========================================")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Qt version: ${Qt6_VERSION}")
message(STATUS "==========================================")