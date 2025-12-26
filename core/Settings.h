#pragma once
#include <QSettings>
#include <QMap>
#include <QVariant>
#include <QJsonObject>
#include "Singleton.h"

class Settings : public Singleton<Settings> {
public:
    ~Settings();
    
    void load(const QString& filename);
    void save();
    void resetToDefaults();
    
    // Getters
    QString getString(const QString& key, const QString& defaultValue = "") const;
    int getInt(const QString& key, int defaultValue = 0) const;
    bool getBool(const QString& key, bool defaultValue = false) const;
    double getDouble(const QString& key, double defaultValue = 0.0) const;
    QJsonObject getJson(const QString& key) const;
    QStringList getStringList(const QString& key) const;
    
    // Setters
    void setValue(const QString& key, const QVariant& value);
    void setString(const QString& key, const QString& value);
    void setInt(const QString& key, int value);
    void setBool(const QString& key, bool value);
    void setDouble(const QString& key, double value);
    void setJson(const QString& key, const QJsonObject& json);
    void setStringList(const QString& key, const QStringList& list);
    
    // Grupos
    void beginGroup(const QString& prefix);
    void endGroup();
    
    // Configuraciones específicas
    struct GameConfig {
        QString path;
        QString realmlist;
        QString expansion;
        QString emulator;
        bool autoPatch;
        bool autoUpdateAddons;
        int graphicsQuality;
    };
    
    struct SocialConfig {
        bool enableFriends;
        bool enableChat;
        bool enableNotifications;
        bool showOnlineStatus;
        int messageHistoryDays;
    };
    
    struct NetworkConfig {
        int updateCheckInterval;
        int newsRefreshInterval;
        int statsRefreshInterval;
        int timeout;
        int retryAttempts;
        bool useProxy;
    };
    
    GameConfig getGameConfig() const;
    SocialConfig getSocialConfig() const;
    NetworkConfig getNetworkConfig() const;
    
    void setGameConfig(const GameConfig& config);
    void setSocialConfig(const SocialConfig& config);
    void setNetworkConfig(const NetworkConfig& config);
    
private:
    Settings();
    
    QSettings* m_settings;
    QString m_filename;
    
    Q_DISABLE_COPY(Settings)
};