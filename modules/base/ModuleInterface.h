#ifndef MODULEINTERFACE_H
#define MODULEINTERFACE_H

#include <QObject>
#include <QWidget>
#include <QJsonObject>

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

Q_DECLARE_INTERFACE(ModuleInterface, "com.thundercore.ModuleInterface/1.0")

#endif // MODULEINTERFACE_H