#include "Application.h"
#include "Logger.h"
#include "Settings.h"
#include <QDir>
#include <QPluginLoader>
#include <QJsonDocument>

Application::Application() 
    : m_state(State::Starting)
    , m_initialized(false) {
    
    m_threadPool.setMaxThreadCount(QThread::idealThreadCount());
    LOG_INFO("Application created");
}

Application::~Application() {
    shutdown();
}

void Application::initialize() {
    if (m_initialized) return;
    
    LOG_INFO("Initializing Application...");
    m_state = State::Starting;
    emit stateChanged(m_state);
    
    try {
        // Cargar configuración
        Settings::instance().load();
        
        // Cargar módulos del núcleo
        loadCoreModules();
        
        // Iniciar heartbeat
        m_heartbeatTimer.setInterval(60000); // 1 minuto
        connect(&m_heartbeatTimer, &QTimer::timeout, this, []() {
            LOG_DEBUG("Heartbeat");
        });
        m_heartbeatTimer.start();
        
        m_initialized = true;
        m_state = State::Running;
        emit stateChanged(m_state);
        
        LOG_INFO("Application initialized successfully");
        
    } catch (const std::exception& e) {
        LOG_CRITICAL("Failed to initialize application: " + QString(e.what()));
        m_state = State::Error;
        emit stateChanged(m_state);
        emit fatalError(e.what());
    }
}

void Application::shutdown() {
    if (!m_initialized) return;
    
    LOG_INFO("Shutting down Application...");
    m_state = State::ShuttingDown;
    emit stateChanged(m_state);
    
    m_heartbeatTimer.stop();
    
    // Descargar módulos
    for (const auto& moduleName : m_loadedModules) {
        unloadModule(moduleName);
    }
    
    // Limpiar thread pool
    m_threadPool.waitForDone(5000);
    
    m_initialized = false;
    LOG_INFO("Application shutdown complete");
}

void Application::loadModule(const QString& moduleName) {
    if (m_modules.contains(moduleName)) {
        LOG_WARNING("Module already loaded: " + moduleName);
        return;
    }
    
    QString modulePath = QDir::currentPath() + "/modules/" + moduleName + ".dll";
    if (!QFile::exists(modulePath)) {
        modulePath = QDir::currentPath() + "/modules/lib" + moduleName + ".so";
    }
    
    QPluginLoader loader(modulePath);
    if (!loader.load()) {
        LOG_ERROR("Failed to load module " + moduleName + ": " + loader.errorString());
        return;
    }
    
    QObject* plugin = loader.instance();
    if (!plugin) {
        LOG_ERROR("Failed to get plugin instance: " + moduleName);
        loader.unload();
        return;
    }
    
    ModuleInterface* module = qobject_cast<ModuleInterface*>(plugin);
    if (!module) {
        LOG_ERROR("Invalid module interface: " + moduleName);
        loader.unload();
        return;
    }
    
    if (!module->initialize()) {
        LOG_ERROR("Module initialization failed: " + moduleName);
        loader.unload();
        return;
    }
    
    m_modules[moduleName] = plugin;
    m_loadedModules.append(moduleName);
    
    LOG_INFO("Module loaded: " + moduleName + " v" + module->getVersion());
    emit moduleLoaded(moduleName);
}

void Application::loadCoreModules() {
    QStringList coreModules = {
        "DatabaseModule",
        "NetworkModule",
        "CacheModule"
    };
    
    for (const QString& module : coreModules) {
        loadModule(module);
    }
}

QString Application::getVersion() const {
    return "1.0.0";
}

QString Application::getBuildDate() const {
    return __DATE__ " " __TIME__;
}

QString Application::getPlatform() const {
#ifdef Q_OS_WIN
    return "Windows";
#elif defined(Q_OS_LINUX)
    return "Linux";
#elif defined(Q_OS_MAC)
    return "macOS";
#else
    return "Unknown";
#endif
}