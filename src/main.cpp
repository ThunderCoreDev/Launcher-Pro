#include <QApplication>
#include <QStyleFactory>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QStandardPaths>
#include "core/Application.h"
#include "core/Logger.h"
#include "core/Settings.h"
#include "ui/MainWindow.h"
#include "database/DatabaseManager.h"
#include "database/LauncherDatabase.h"

int main(int argc, char *argv[]) {
    // Configurar atributos de la aplicación
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL, false);
    
    // Crear aplicación
    QApplication app(argc, argv);
    app.setApplicationName("ThunderCore Launcher Pro");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("ThunderCore WoW");
    app.setOrganizationDomain("thundercore.com");
    
    // Configurar estilo
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    
    // Configurar directorios
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString logDir = dataDir + "/logs";
    
    QDir().mkpath(configDir);
    QDir().mkpath(dataDir);
    QDir().mkpath(logDir);
    
    // Inicializar logger
    Logger::instance().initialize(logDir + "/thundercore.log");
    Logger::instance().setConsoleOutput(true);
    Logger::instance().setFileOutput(true);
    
    LOG_INFO("==========================================");
    LOG_INFO(" ThunderCore Launcher Pro v1.0.0");
    LOG_INFO("==========================================");
    LOG_INFO("Starting application...");
    LOG_INFO("Config directory: " + configDir);
    LOG_INFO("Data directory: " + dataDir);
    LOG_INFO("Log directory: " + logDir);
    
    // Cargar configuración
    Settings& settings = Settings::instance();
    settings.load(configDir + "/thundercore.ini");
    
    // Cargar estilo QSS
    QString theme = settings.getString("Window/Theme", "BattleNet");
    QString themeFile = ":/styles/theme_" + theme.toLower() + ".qss";
    
    QFile styleFile(themeFile);
    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        app.setStyleSheet(styleSheet);
        LOG_INFO("Theme loaded: " + theme);
    } else {
        LOG_WARNING("Could not load theme file: " + themeFile);
    }
    
    // Inicializar base de datos
    LOG_INFO("Initializing database connections...");
    
    // Base de datos del launcher
    if (!LauncherDatabase::instance().initialize(
        settings.getString("Database/LauncherHost", "127.0.0.1"),
        settings.getInt("Database/LauncherPort", 3306),
        settings.getString("Database/LauncherUser", "launcher_user"),
        settings.getString("Database/LauncherPassword", "ThunderCoreNet2024!")
    )) {
        LOG_ERROR("Failed to initialize launcher database");
        QMessageBox::critical(nullptr, "Database Error",
            "No se pudo conectar a la base de datos del launcher.\n"
            "Verifica que MySQL esté ejecutándose y que las credenciales sean correctas.");
        return 1;
    }
    
    // Base de datos del juego
    if (!DatabaseManager::instance().initialize()) {
        LOG_WARNING("Game database connection failed - some features will be disabled");
        QMessageBox::warning(nullptr, "Database Warning",
            "No se pudo conectar a la base de datos del juego.\n"
            "Algunas características estarán deshabilitadas.");
    }
    
    try {
        // Crear ventana principal
        MainWindow mainWindow;
        mainWindow.show();
        
        LOG_INFO("Main window displayed");
        
        // Ejecutar aplicación
        int result = app.exec();
        
        LOG_INFO("Application shutting down...");
        
        // Limpiar
        LauncherDatabase::instance().close();
        DatabaseManager::instance().disconnect();
        
        LOG_INFO("Application shutdown complete");
        return result;
        
    } catch (const std::exception& e) {
        LOG_CRITICAL("Unhandled exception: " + QString(e.what()));
        QMessageBox::critical(nullptr, "Critical Error",
            QString("A critical error occurred:\n%1\n\n"
                   "The application will now close.").arg(e.what()));
        return 1;
    } catch (...) {
        LOG_CRITICAL("Unknown exception occurred");
        QMessageBox::critical(nullptr, "Critical Error",
            "An unknown critical error occurred.\n"
            "The application will now close.");
        return 1;
    }
}