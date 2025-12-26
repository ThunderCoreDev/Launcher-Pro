#ifndef LAUNCHERDATABASE_H
#define LAUNCHERDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QJsonObject>
#include <QJsonArray>
#include "core/Singleton.h"
#include "core/Global.h"

class LauncherDatabase : public QObject, public Singleton<LauncherDatabase> {
    Q_OBJECT
    
public:
    bool initialize(const QString& host, int port, 
                   const QString& user, const QString& password);
    
    // Gestión de usuarios
    bool createUser(AccountId accountId, const QString& username);
    bool updateDailyLogin(AccountId accountId);
    QJsonObject getUserStats(AccountId accountId);
    
    // Mensajería
    bool sendMessage(AccountId senderId, AccountId receiverId, 
                    const QString& subject, const QString& message);
    QJsonArray getUserMessages(AccountId userId, int limit = 50);
    
    // Sistema de amigos
    bool addFriend(AccountId userId, AccountId friendId, const QString& message = "");
    QJsonArray getUserFriends(AccountId userId);
    
    // Notificaciones
    bool createNotification(AccountId userId, const QString& type, 
                           const QString& title, const QString& content);
    
    // Utilidades
    QString getUsername(AccountId accountId);
    
    void close();
    
private:
    LauncherDatabase() = default;
    friend class Singleton<LauncherDatabase>;
    
    QSqlDatabase m_database;
};

#endif // LAUNCHERDATABASE_H