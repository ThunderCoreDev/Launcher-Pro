#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H

#include <QObject>
#include <QMap>
#include <QPluginLoader>
#include "ModuleInterface.h"

class ModuleManager : public QObject {
    Q_OBJECT
    
public:
    explicit ModuleManager(QObject* parent = nullptr);
    ~ModuleManager();
    
    void loadAllModules();
    void unloadAllModules();
    
    bool loadModule(const QString& modulePath);
    bool unloadModule(const QString& moduleName);
    bool isModuleLoaded(const QString& moduleName) const;
    
    ModuleInterface* getModule(const QString& moduleName) const;
    QStringList getLoadedModules() const;
    
    void setModuleConfig(const QString& moduleName, const QJsonObject& config);
    QJsonObject getModuleConfig(const QString& moduleName) const;
    
signals:
    void moduleLoaded(const QString& moduleName);
    void moduleUnloaded(const QString& moduleName);
    void moduleError(const QString& moduleName, const QString& error);
    
private:
    struct ModuleInfo {
        QString path;
        QPluginLoader* loader;
        ModuleInterface* instance;
        bool enabled;
    };
    
    QMap<QString, ModuleInfo> m_modules;
    QString m_modulesDir;
};

#endif // MODULEMANAGER_H