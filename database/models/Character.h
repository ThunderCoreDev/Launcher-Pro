#ifndef CHARACTERMODEL_H
#define CHARACTERMODEL_H

#include <QObject>
#include <QDateTime>

class Character : public QObject {
    Q_OBJECT
    
    Q_PROPERTY(quint64 guid READ guid WRITE setGuid NOTIFY guidChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString race READ race WRITE setRace NOTIFY raceChanged)
    Q_PROPERTY(QString className READ className WRITE setClassName NOTIFY classNameChanged)
    Q_PROPERTY(int level READ level WRITE setLevel NOTIFY levelChanged)
    Q_PROPERTY(bool online READ online WRITE setOnline NOTIFY onlineChanged)
    Q_PROPERTY(quint32 playTime READ playTime WRITE setPlayTime NOTIFY playTimeChanged)
    Q_PROPERTY(QDateTime lastLogin READ lastLogin WRITE setLastLogin NOTIFY lastLoginChanged)
    
public:
    explicit Character(QObject* parent = nullptr);
    Character(quint64 guid, const QString& name, QObject* parent = nullptr);
    
    // Getters
    quint64 guid() const { return m_guid; }
    QString name() const { return m_name; }
    QString race() const { return m_race; }
    QString className() const { return m_className; }
    int level() const { return m_level; }
    bool online() const { return m_online; }
    quint32 playTime() const { return m_playTime; }
    QDateTime lastLogin() const { return m_lastLogin; }
    
    // Setters
    void setGuid(quint64 guid);
    void setName(const QString& name);
    void setRace(const QString& race);
    void setClassName(const QString& className);
    void setLevel(int level);
    void setOnline(bool online);
    void setPlayTime(quint32 playTime);
    void setLastLogin(const QDateTime& lastLogin);
    
    // Métodos de utilidad
    QString faction() const;
    QString playTimeFormatted() const;
    bool isStuck() const;
    
signals:
    void guidChanged(quint64 guid);
    void nameChanged(const QString& name);
    void raceChanged(const QString& race);
    void classNameChanged(const QString& className);
    void levelChanged(int level);
    void onlineChanged(bool online);
    void playTimeChanged(quint32 playTime);
    void lastLoginChanged(const QDateTime& lastLogin);
    
private:
    quint64 m_guid;
    QString m_name;
    QString m_race;
    QString m_className;
    int m_level;
    bool m_online;
    quint32 m_playTime;
    QDateTime m_lastLogin;
};

#endif // CHARACTERMODEL_H