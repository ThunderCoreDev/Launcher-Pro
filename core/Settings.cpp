#include "Settings.h"
#include "Logger.h"
#include <QStandardPaths>
#include <QDir>
#include <QJsonDocument>

Settings::Settings() 
    : m_settings(nullptr) {
}

Settings::~Settings() {
    save();
    delete m_settings;
}

void Settings::load(const QString& filename) {
    if (m_settings) {
        save();
        delete m_settings;
    }
    
    if (filename.isEmpty()) {
        QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        QDir().mkpath(configDir);
        m_filename = configDir + "/thundercore.ini";
    } else {
        m_filename = filename;
    }
    
    m_settings = new QSettings(m_filename, QSettings::IniFormat);
    
    // Establecer valores por defecto si no existen
    if (m_settings->allKeys().isEmpty()) {
        resetToDefaults();
    }
    
    LOG_INFO("Settings loaded from: " + m_filename);
}

void Settings::save() {
    if (m_settings) {
        m_settings->sync();
        LOG_DEBUG("Settings saved");
    }
}

void Settings::resetToDefaults() {
    if (!m_settings) return;
    
    // General
    setString("Application/Name", "ThunderCore Launcher Pro");
    setString("Application/Version", "1.0.0");
    setString("Application/Organization", "ThunderCore WoW");
    
    // Window
    setInt("Window/Width", 1200);
    setInt("Window/Height", 800);
    setBool("Window/Maximized", false);
    setString("Window/Theme", "BattleNet");
    setString("Window/Language", "es_ES");
    setInt("Window/Opacity", 100);
    setBool("Window/EnableAnimations", true);
    setBool("Window/EnableEffects", true);
    
    // Game
    setString("Game/DefaultPath", "");
    setString("Game/Realmlist", "logon.thundernet.com");
    setString("Game/Expansion", "WOTLK");
    setString("Game/Emulator", "TrinityCore");
    setBool("Game/AutoPatch", true);
    setBool("Game/AutoUpdateAddons", true);
    setBool("Game/ClearCacheOnLaunch", false);
    
    // Database
    setString("Database/LauncherHost", "127.0.0.1");
    setInt("Database/LauncherPort", 3306);
    setString("Database/LauncherUser", "launcher_user");
    setString("Database/LauncherPassword", "ThunderNet2024!");
    
    setString("Database/AuthHost", "127.0.0.1");
    setInt("Database/AuthPort", 3306);
    setString("Database/AuthUser", "ThunderCore");
    setString("Database/AuthPassword", "A990302**//");
    
    // Network
    setInt("Network/UpdateCheckInterval", 3600);
    setInt("Network/NewsRefreshInterval", 300);
    setInt("Network/StatsRefreshInterval", 60);
    setInt("Network/Timeout", 30);
    setInt("Network/RetryAttempts", 3);
    setBool("Network/UseProxy", false);
    
    // Performance
    setBool("Performance/EnableHardwareAcceleration", true);
    setString("Performance/TextureQuality", "High");
    setString("Performance/ShadowQuality", "Medium");
    setString("Performance/ParticleDensity", "Normal");
    setInt("Performance/CacheSize", 512);
    setInt("Performance/WorkerThreads", 4);
    
    save();
    LOG_INFO("Default settings restored");
}

// Getters
QString Settings::getString(const QString& key, const QString& defaultValue) const {
    if (!m_settings) return defaultValue;
    return m_settings->value(key, defaultValue).toString();
}

int Settings::getInt(const QString& key, int defaultValue) const {
    if (!m_settings) return defaultValue;
    return m_settings->value(key, defaultValue).toInt();
}

bool Settings::getBool(const QString& key, bool defaultValue) const {
    if (!m_settings) return defaultValue;
    return m_settings->value(key, defaultValue).toBool();
}

double Settings::getDouble(const QString& key, double defaultValue) const {
    if (!m_settings) return defaultValue;
    return m_settings->value(key, defaultValue).toDouble();
}

QJsonObject Settings::getJson(const QString& key) const {
    QString jsonStr = getString(key, "{}");
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    return doc.object();
}

QStringList Settings::getStringList(const QString& key) const {
    if (!m_settings) return QStringList();
    return m_settings->value(key).toStringList();
}

// Setters
void Settings::setValue(const QString& key, const QVariant& value) {
    if (!m_settings) return;
    m_settings->setValue(key, value);
}

Settings::GameConfig Settings::getGameConfig() const {
    GameConfig config;
    config.path = getString("Game/DefaultPath");
    config.realmlist = getString("Game/Realmlist");
    config.expansion = getString("Game/Expansion");
    config.emulator = getString("Game/Emulator");
    config.autoPatch = getBool("Game/AutoPatch");
    config.autoUpdateAddons = getBool("Game/AutoUpdateAddons");
    config.graphicsQuality = getInt("Game/GraphicsQuality", 3);
    return config;
}

void Settings::setGameConfig(const GameConfig& config) {
    setString("Game/DefaultPath", config.path);
    setString("Game/Realmlist", config.realmlist);
    setString("Game/Expansion", config.expansion);
    setString("Game/Emulator", config.emulator);
    setBool("Game/AutoPatch", config.autoPatch);
    setBool("Game/AutoUpdateAddons", config.autoUpdateAddons);
    setInt("Game/GraphicsQuality", config.graphicsQuality);
    save();
}