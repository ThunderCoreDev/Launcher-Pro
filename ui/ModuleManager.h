#pragma once
#include <QObject>
#include <QMap>
#include <QPluginLoader>
#include <QJsonObject>
#include "core/Singleton.h"

class ModuleInterface {
public:
    virtual ~ModuleInterface() = default;
    
    virtual QString getName() const = 0;
    virtual QString getVersion() const = 0;
    virtual QString getDescription() const = 0;
    virtual QJsonObject getConfig() const = 0;
    virtual QWidget* createWidget(QWidget* parent = nullptr) = 0;
    
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    virtual void update() = 0;
    
    virtual QStringList getDependencies() const = 0;
    virtual bool isCompatible(const QString& coreVersion) const = 0;
};

class ModuleManager : public QObject, public Singleton<ModuleManager> {
    Q_OBJECT
    
public:
    ~ModuleManager();
    
    void loadModules();
    void unloadModules();
    
    bool loadModule(const QString& modulePath);
    bool unloadModule(const QString& moduleName);
    bool isModuleLoaded(const QString& moduleName) const;
    
    ModuleInterface* getModule(const QString& moduleName) const;
    QStringList getLoadedModules() const;
    QStringList getAvailableModules() const;
    
    // Gestión de dependencias
    bool checkDependencies(const QString& moduleName) const;
    QStringList getMissingDependencies(const QString& moduleName) const;
    
    // Configuración de módulos
    void setModuleConfig(const QString& moduleName, const QJsonObject& config);
    QJsonObject getModuleConfig(const QString& moduleName) const;
    
    // Eventos de módulos
    void broadcastEvent(const QString& eventName, const QVariant& data = QVariant());
    
signals:
    void moduleLoaded(const QString& moduleName);
    void moduleUnloaded(const QString& moduleName);
    void moduleError(const QString& moduleName, const QString& error);
    void eventReceived(const QString& eventName, const QVariant& data);
    
private:
    ModuleManager();
    
    void scanModulesDirectory();
    void loadModuleMetadata(const QString& path);
    bool resolveDependencies(const QString& moduleName);
    void saveModuleConfig(const QString& moduleName);
    
    struct ModuleInfo {
        QString path;
        QString name;
        QString version;
        QString description;
        QJsonObject config;
        QPluginLoader* loader;
        ModuleInterface* instance;
        bool enabled;
        QStringList dependencies;
    };
    
    QMap<QString, ModuleInfo> m_modules;
    QString m_modulesDir;
    
    Q_DISABLE_COPY(ModuleManager)
};

Q_DECLARE_INTERFACE(ModuleInterface, "com.thundercore.ModuleInterface/1.0")