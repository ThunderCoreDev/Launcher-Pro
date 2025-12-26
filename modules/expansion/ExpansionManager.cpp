#include "ExpansionManager.h"
#include "core/Logger.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

ExpansionManager& ExpansionManager::instance() {
    static ExpansionManager instance;
    return instance;
}

ExpansionManager::ExpansionManager() 
    : m_currentExpansion(Expansion::WOTLK)
    , m_currentEmulator(Emulator::TRINITYCORE) {
    loadConfigurations();
}

void ExpansionManager::loadConfigurations() {
    // Configuración para WotLK
    ExpansionConfig wotlkConfig;
    wotlkConfig.name = "Wrath of the Lich King";
    wotlkConfig.version = "3.3.5a";
    wotlkConfig.realmlistFormat = "set realmlist %1";
    wotlkConfig.executable = "WoW.exe";
    wotlkConfig.authAlgorithm = "SRP6";
    wotlkConfig.maxLevel = 80;
    
    // Tablas para TrinityCore WotLK
    wotlkConfig.databaseTables["account"] = "account";
    wotlkConfig.databaseTables["characters"] = "characters";
    wotlkConfig.databaseTables["world"] = "world";
    
    configurations[{Expansion::WOTLK, Emulator::TRINITYCORE}] = wotlkConfig;
    
    // Configuración para MoP
    ExpansionConfig mopConfig;
    mopConfig.name = "Mists of Pandaria";
    mopConfig.version = "5.4.8";
    mopConfig.realmlistFormat = "set realmlist %1";
    mopConfig.executable = "Wow-64.exe";
    mopConfig.authAlgorithm = "SRP6_v2";
    mopConfig.maxLevel = 90;
    
    mopConfig.databaseTables["account"] = "battlenet_accounts";
    mopConfig.databaseTables["characters"] = "characters";
    mopConfig.databaseTables["world"] = "world";
    
    configurations[{Expansion::MOP, Emulator::TRINITYCORE}] = mopConfig;
    
    LOG_INFO("Expansion configurations loaded");
}

Expansion ExpansionManager::getCurrentExpansion() const {
    return m_currentExpansion;
}

Emulator ExpansionManager::getCurrentEmulator() const {
    return m_currentEmulator;
}

ExpansionConfig ExpansionManager::getConfig(Expansion exp, Emulator emu) const {
    auto key = qMakePair(exp, emu);
    if (configurations.contains(key)) {
        return configurations[key];
    }
    
    // Retornar configuración por defecto (WotLK)
    return configurations[{Expansion::WOTLK, Emulator::TRINITYCORE}];
}

void ExpansionManager::setExpansion(Expansion exp) {
    if (m_currentExpansion != exp) {
        m_currentExpansion = exp;
        LOG_INFO("Expansion changed to: " + getExpansionName(exp));
        emit expansionChanged(exp);
    }
}

void ExpansionManager::setEmulator(Emulator emu) {
    if (m_currentEmulator != emu) {
        m_currentEmulator = emu;
        LOG_INFO("Emulator changed to: " + getEmulatorName(emu));
        emit emulatorChanged(emu);
    }
}

QStringList ExpansionManager::getSupportedExpansions() const {
    return {
        "Classic (1.12.1)",
        "The Burning Crusade (2.4.3)", 
        "Wrath of the Lich King (3.3.5a)",
        "Cataclysm (4.3.4)",
        "Mists of Pandaria (5.4.8)",
        "Warlords of Draenor (6.2.4)",
        "Legion (7.3.5)",
        "Battle for Azeroth (8.3.7)",
        "Shadowlands (9.2.7)",
        "Dragonflight (10.2.0)"
    };
}

QStringList ExpansionManager::getSupportedEmulators() const {
    return {
        "TrinityCore",
        "AzerothCore", 
        "CMaNGOS",
        "SkyFire",
        "MangOS",
        "OregonCore"
    };
}

bool ExpansionManager::validateClientPath(const QString& path) const {
    if (path.isEmpty()) return false;
    
    ExpansionConfig config = getConfig(m_currentExpansion, m_currentEmulator);
    QString exePath = path + "/" + config.executable;
    
    return QFile::exists(exePath);
}

QString ExpansionManager::getExpansionName(Expansion exp) const {
    switch (exp) {
        case Expansion::CLASSIC: return "Classic";
        case Expansion::TBC: return "The Burning Crusade";
        case Expansion::WOTLK: return "Wrath of the Lich King";
        case Expansion::CATA: return "Cataclysm";
        case Expansion::MOP: return "Mists of Pandaria";
        case Expansion::WOD: return "Warlords of Draenor";
        case Expansion::LEGION: return "Legion";
        case Expansion::BFA: return "Battle for Azeroth";
        case Expansion::SHADOWLANDS: return "Shadowlands";
        case Expansion::DRAGONFLIGHT: return "Dragonflight";
        default: return "Unknown";
    }
}

QString ExpansionManager::getEmulatorName(Emulator emu) const {
    switch (emu) {
        case Emulator::TRINITYCORE: return "TrinityCore";
        case Emulator::AZEROTHCORE: return "AzerothCore";
        case Emulator::CMANGOS: return "CMaNGOS";
        case Emulator::SKYFIRE: return "SkyFire";
        case Emulator::MANGOS: return "MangOS";
        case Emulator::OREGONCORE: return "OregonCore";
        default: return "Unknown";
    }
}