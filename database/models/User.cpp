#include "User.h"
#include <QDebug>

User::User(QObject* parent) 
    : QObject(parent)
    , m_id(0)
    , m_level(1)
    , m_experience(0) {
}

User::User(quint32 id, const QString& username, QObject* parent)
    : QObject(parent)
    , m_id(id)
    , m_username(username)
    , m_level(1)
    , m_experience(0)
    , m_lastLogin(QDateTime::currentDateTime()) {
}

void User::setId(quint32 id) {
    if (m_id != id) {
        m_id = id;
        emit idChanged(id);
    }
}

void User::setUsername(const QString& username) {
    if (m_username != username) {
        m_username = username;
        emit usernameChanged(username);
    }
}

void User::setLevel(int level) {
    if (m_level != level) {
        m_level = level;
        emit levelChanged(level);
    }
}

void User::setExperience(quint64 experience) {
    if (m_experience != experience) {
        m_experience = experience;
        emit experienceChanged(experience);
        
        // Verificar si subió de nivel
        while (m_experience >= experienceForNextLevel() && m_level < 100) {
            m_level++;
            emit levelUp(m_level);
        }
    }
}

void User::setLastLogin(const QDateTime& lastLogin) {
    if (m_lastLogin != lastLogin) {
        m_lastLogin = lastLogin;
        emit lastLoginChanged(lastLogin);
    }
}

quint64 User::experienceForNextLevel() const {
    // Fórmula: 100 * nivel^2
    return 100ULL * m_level * m_level;
}

float User::experienceProgress() const {
    if (m_level >= 100) return 1.0f;
    
    quint64 currentLevelExp = experienceForNextLevel();
    if (m_level > 1) {
        quint64 prevLevelExp = 100ULL * (m_level - 1) * (m_level - 1);
        currentLevelExp -= prevLevelExp;
    }
    
    quint64 currentExp = m_experience;
    if (m_level > 1) {
        quint64 prevLevelExp = 100ULL * (m_level - 1) * (m_level - 1);
        currentExp -= prevLevelExp;
    }
    
    return static_cast<float>(currentExp) / currentLevelExp;
}

void User::addExperience(quint64 amount) {
    setExperience(m_experience + amount);
}