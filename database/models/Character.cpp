#include "Character.h"
#include <QMap>

Character::Character(QObject* parent) 
    : QObject(parent)
    , m_guid(0)
    , m_level(1)
    , m_online(false)
    , m_playTime(0) {
}

Character::Character(quint64 guid, const QString& name, QObject* parent)
    : QObject(parent)
    , m_guid(guid)
    , m_name(name)
    , m_level(1)
    , m_online(false)
    , m_playTime(0)
    , m_lastLogin(QDateTime::currentDateTime()) {
}

void Character::setGuid(quint64 guid) {
    if (m_guid != guid) {
        m_guid = guid;
        emit guidChanged(guid);
    }
}

void Character::setName(const QString& name) {
    if (m_name != name) {
        m_name = name;
        emit nameChanged(name);
    }
}

void Character::setRace(const QString& race) {
    if (m_race != race) {
        m_race = race;
        emit raceChanged(race);
    }
}

void Character::setClassName(const QString& className) {
    if (m_className != className) {
        m_className = className;
        emit classNameChanged(className);
    }
}

void Character::setLevel(int level) {
    if (m_level != level) {
        m_level = level;
        emit levelChanged(level);
    }
}

void Character::setOnline(bool online) {
    if (m_online != online) {
        m_online = online;
        emit onlineChanged(online);
    }
}

void Character::setPlayTime(quint32 playTime) {
    if (m_playTime != playTime) {
        m_playTime = playTime;
        emit playTimeChanged(playTime);
    }
}

void Character::setLastLogin(const QDateTime& lastLogin) {
    if (m_lastLogin != lastLogin) {
        m_lastLogin = lastLogin;
        emit lastLoginChanged(lastLogin);
    }
}

QString Character::faction() const {
    static QMap<QString, QString> raceToFaction = {
        {"Human", "Alliance"},
        {"Dwarf", "Alliance"},
        {"Night Elf", "Alliance"},
        {"Gnome", "Alliance"},
        {"Draenei", "Alliance"},
        {"Worgen", "Alliance"},
        {"Pandaren (Alliance)", "Alliance"},
        
        {"Orc", "Horde"},
        {"Undead", "Horde"},
        {"Tauren", "Horde"},
        {"Troll", "Horde"},
        {"Blood Elf", "Horde"},
        {"Goblin", "Horde"},
        {"Pandaren (Horde)", "Horde"}
    };
    
    return raceToFaction.value(m_race, "Neutral");
}

QString Character::playTimeFormatted() const {
    quint32 seconds = m_playTime;
    quint32 days = seconds / 86400;
    seconds %= 86400;
    quint32 hours = seconds / 3600;
    seconds %= 3600;
    quint32 minutes = seconds / 60;
    
    if (days > 0) {
        return QString("%1d %2h %3m").arg(days).arg(hours).arg(minutes);
    } else if (hours > 0) {
        return QString("%1h %2m").arg(hours).arg(minutes);
    } else {
        return QString("%1m").arg(minutes);
    }
}

bool Character::isStuck() const {
    // Un personaje se considera "atascado" si no ha iniciado sesión en más de 7 días
    // y está por debajo del nivel 10
    if (m_level < 10 && m_lastLogin.addDays(7) < QDateTime::currentDateTime()) {
        return true;
    }
    
    return false;
}