#ifndef USERMODEL_H
#define USERMODEL_H

#include <QObject>
#include <QDateTime>

class User : public QObject {
    Q_OBJECT
    
    Q_PROPERTY(quint32 id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(int level READ level WRITE setLevel NOTIFY levelChanged)
    Q_PROPERTY(quint64 experience READ experience WRITE setExperience NOTIFY experienceChanged)
    Q_PROPERTY(QDateTime lastLogin READ lastLogin WRITE setLastLogin NOTIFY lastLoginChanged)
    
public:
    explicit User(QObject* parent = nullptr);
    User(quint32 id, const QString& username, QObject* parent = nullptr);
    
    // Getters
    quint32 id() const { return m_id; }
    QString username() const { return m_username; }
    int level() const { return m_level; }
    quint64 experience() const { return m_experience; }
    QDateTime lastLogin() const { return m_lastLogin; }
    
    // Setters
    void setId(quint32 id);
    void setUsername(const QString& username);
    void setLevel(int level);
    void setExperience(quint64 experience);
    void setLastLogin(const QDateTime& lastLogin);
    
    // Métodos de utilidad
    quint64 experienceForNextLevel() const;
    float experienceProgress() const;
    void addExperience(quint64 amount);
    
signals:
    void idChanged(quint32 id);
    void usernameChanged(const QString& username);
    void levelChanged(int level);
    void experienceChanged(quint64 experience);
    void lastLoginChanged(const QDateTime& lastLogin);
    void levelUp(int newLevel);
    
private:
    quint32 m_id;
    QString m_username;
    int m_level;
    quint64 m_experience;
    QDateTime m_lastLogin;
};

#endif // USERMODEL_H