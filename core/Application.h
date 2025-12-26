#pragma once
#include <QObject>
#include <QTimer>
#include <QThreadPool>
#include <memory>
#include "Singleton.h"

class Application : public QObject, public Singleton<Application> {
    Q_OBJECT
    friend class Singleton<Application>;
    
public:
    ~Application();
    
    void initialize();
    void shutdown();
    
    bool isInitialized() const { return m_initialized; }
    QThreadPool& threadPool() { return m_threadPool; }
    
    // Gestión de módulos
    void loadModule(const QString& moduleName);
    void unloadModule(const QString& moduleName);
    bool isModuleLoaded(const QString& moduleName) const;
    
    // Utilitarios
    QString getVersion() const;
    QString getBuildDate() const;
    QString getPlatform() const;
    
    // Estado de la aplicación
    enum class State {
        Starting,
        Running,
        ShuttingDown,
        Error
    };
    
    State getState() const { return m_state; }
    
signals:
    void moduleLoaded(const QString& moduleName);
    void moduleUnloaded(const QString& moduleName);
    void stateChanged(State newState);
    void shutdownRequested();
    void fatalError(const QString& error);
    
public slots:
    void restart();
    void emergencyShutdown();
    
private:
    Application();
    
    void loadCoreModules();
    void saveState();
    void loadState();
    void cleanup();
    
    State m_state;
    bool m_initialized;
    QThreadPool m_threadPool;
    QTimer m_heartbeatTimer;
    QMap<QString, QObject*> m_modules;
    QStringList m_loadedModules;
    
    Q_DISABLE_COPY(Application)
};