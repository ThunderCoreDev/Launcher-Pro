#pragma once
#include <QObject>
#include <QMap>

enum class Expansion {
    CLASSIC      = 0,
    TBC          = 1,
    WOTLK        = 2,
    CATA         = 3,
    MOP          = 4,
    WOD          = 5,
    LEGION       = 6,
    BFA          = 7,
    SHADOWLANDS  = 8,
    DRAGONFLIGHT = 9
};

enum class Emulator {
    TRINITYCORE  = 0,
    AZEROTHCORE  = 1,
    CMANGOS      = 2,
    SKYFIRE      = 3,
    MANGOS       = 4,
    OREGONCORE   = 5
};

struct ExpansionConfig {
    QString name;
    QString version;
    QString realmlistFormat;
    QString executable;
    QString patchUrl;
    QMap<QString, QString> databaseTables;
    QString authAlgorithm;
    int maxLevel;
};

class ExpansionManager : public QObject {
    Q_OBJECT
public:
    static ExpansionManager& instance();
    
    Expansion getCurrentExpansion() const;
    Emulator getCurrentEmulator() const;
    ExpansionConfig getConfig(Expansion exp, Emulator emu) const;
    
    void setExpansion(Expansion exp);
    void setEmulator(Emulator emu);
    
    QStringList getSupportedExpansions() const;
    QStringList getSupportedEmulators() const;
    
    bool validateClientPath(const QString& path) const;
    QString getExpectedHash(Expansion exp) const;
    
signals:
    void expansionChanged(Expansion newExpansion);
    void emulatorChanged(Emulator newEmulator);
    
private:
    ExpansionManager();
    void loadConfigurations();
    
    Expansion currentExpansion;
    Emulator currentEmulator;
    QMap<QPair<Expansion, Emulator>, ExpansionConfig> configurations;
};