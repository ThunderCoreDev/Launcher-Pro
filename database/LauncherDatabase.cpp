#include "LauncherDatabase.h"
#include "core/Logger.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonArray>

LauncherDatabase::LauncherDatabase(QObject* parent) 
    : QObject(parent) {
}

LauncherDatabase& LauncherDatabase::instance() {
    static LauncherDatabase instance;
    return instance;
}

bool LauncherDatabase::initialize(const QString& host, int port, 
                                 const QString& user, const QString& password) {
    m_database = QSqlDatabase::addDatabase("QMYSQL", "launcher");
    m_database.setHostName(host);
    m_database.setPort(port);
    m_database.setUserName(user);
    m_database.setPassword(password);
    m_database.setDatabaseName("launcher");
    
    if (!m_database.open()) {
        LOG_ERROR("Failed to open launcher database: " + m_database.lastError().text());
        return false;
    }
    
    LOG_INFO("Launcher database connected successfully");
    return true;
}

bool LauncherDatabase::createUser(AccountId accountId, const QString& username) {
    QSqlQuery query(m_database);
    query.prepare(
        "INSERT INTO launcher_users (account_id, username, level, experience, "
        "daily_login_streak, total_login_days, created_at) "
        "VALUES (:account_id, :username, 1, 0, 0, 1, NOW()) "
        "ON DUPLICATE KEY UPDATE "
        "username = VALUES(username), "
        "last_login = NOW()"
    );
    
    query.bindValue(":account_id", accountId);
    query.bindValue(":username", username);
    
    if (!query.exec()) {
        LOG_ERROR("Failed to create launcher user: " + query.lastError().text());
        return false;
    }
    
    return true;
}

bool LauncherDatabase::updateDailyLogin(AccountId accountId) {
    QSqlQuery query(m_database);
    query.prepare("CALL RegisterDailyLogin(:account_id)");
    query.bindValue(":account_id", accountId);
    
    if (!query.exec()) {
        LOG_ERROR("Failed to update daily login: " + query.lastError().text());
        return false;
    }
    
    return true;
}

QJsonObject LauncherDatabase::getUserStats(AccountId accountId) {
    QSqlQuery query(m_database);
    query.prepare(
        "SELECT level, experience, daily_login_streak, total_login_days, "
        "last_login_date FROM launcher_users WHERE account_id = :account_id"
    );
    query.bindValue(":account_id", accountId);
    
    QJsonObject stats;
    if (query.exec() && query.next()) {
        stats["level"] = query.value("level").toInt();
        stats["experience"] = query.value("experience").toInt();
        stats["daily_streak"] = query.value("daily_login_streak").toInt();
        stats["total_days"] = query.value("total_login_days").toInt();
        stats["last_login"] = query.value("last_login_date").toString();
    }
    
    return stats;
}

bool LauncherDatabase::sendMessage(AccountId senderId, AccountId receiverId, 
                                  const QString& subject, const QString& message) {
    QSqlQuery query(m_database);
    query.prepare("CALL SendPrivateMessage(:sender_id, :receiver_id, :subject, :message)");
    
    query.bindValue(":sender_id", senderId);
    query.bindValue(":receiver_id", receiverId);
    query.bindValue(":subject", subject);
    query.bindValue(":message", message);
    
    if (!query.exec()) {
        LOG_ERROR("Failed to send message: " + query.lastError().text());
        return false;
    }
    
    return true;
}

QJsonArray LauncherDatabase::getUserMessages(AccountId userId, int limit) {
    QSqlQuery query(m_database);
    query.prepare(
        "SELECT pm.id, pm.sender_id, lu_s.username as sender_name, "
        "pm.receiver_id, lu_r.username as receiver_name, "
        "pm.subject, pm.message, pm.is_read, pm.created_at "
        "FROM private_messages pm "
        "JOIN launcher_users lu_s ON pm.sender_id = lu_s.account_id "
        "JOIN launcher_users lu_r ON pm.receiver_id = lu_r.account_id "
        "WHERE (pm.sender_id = :user_id OR pm.receiver_id = :user_id) "
        "AND pm.deleted_by_sender = FALSE AND pm.deleted_by_receiver = FALSE "
        "ORDER BY pm.created_at DESC LIMIT :limit"
    );
    
    query.bindValue(":user_id", userId);
    query.bindValue(":limit", limit);
    
    QJsonArray messages;
    if (query.exec()) {
        while (query.next()) {
            QJsonObject msg;
            msg["id"] = query.value("id").toInt();
            msg["sender_id"] = query.value("sender_id").toInt();
            msg["sender_name"] = query.value("sender_name").toString();
            msg["receiver_id"] = query.value("receiver_id").toInt();
            msg["receiver_name"] = query.value("receiver_name").toString();
            msg["subject"] = query.value("subject").toString();
            msg["message"] = query.value("message").toString();
            msg["is_read"] = query.value("is_read").toBool();
            msg["created_at"] = query.value("created_at").toString();
            
            messages.append(msg);
        }
    }
    
    return messages;
}

bool LauncherDatabase::addFriend(AccountId userId, AccountId friendId, const QString& message) {
    QSqlQuery query(m_database);
    query.prepare(
        "INSERT INTO friendships (user_id, friend_id, status, request_message) "
        "VALUES (:user_id, :friend_id, 'pending', :message) "
        "ON DUPLICATE KEY UPDATE status = 'pending'"
    );
    
    query.bindValue(":user_id", userId);
    query.bindValue(":friend_id", friendId);
    query.bindValue(":message", message);
    
    if (!query.exec()) {
        LOG_ERROR("Failed to add friend: " + query.lastError().text());
        return false;
    }
    
    // Crear notificación
    createNotification(friendId, "friend_request", 
                      "Solicitud de amistad", 
                      QString("El usuario %1 te ha enviado una solicitud de amistad")
                      .arg(getUsername(userId)));
    
    return true;
}

QJsonArray LauncherDatabase::getUserFriends(AccountId userId) {
    QSqlQuery query(m_database);
    query.prepare(
        "SELECT f.friend_id, lu.username, f.status, lu.last_login_date, "
        "(SELECT COUNT(*) FROM characters c WHERE c.account = f.friend_id AND c.online = 1) as online "
        "FROM friendships f "
        "JOIN launcher_users lu ON f.friend_id = lu.account_id "
        "WHERE f.user_id = :user_id AND f.status = 'accepted'"
    );
    
    query.bindValue(":user_id", userId);
    
    QJsonArray friends;
    if (query.exec()) {
        while (query.next()) {
            QJsonObject friendObj;
            friendObj["account_id"] = query.value("friend_id").toInt();
            friendObj["username"] = query.value("username").toString();
            friendObj["status"] = query.value("status").toString();
            friendObj["last_login"] = query.value("last_login_date").toString();
            friendObj["online"] = query.value("online").toInt() > 0;
            
            friends.append(friendObj);
        }
    }
    
    return friends;
}

bool LauncherDatabase::createNotification(AccountId userId, const QString& type, 
                                         const QString& title, const QString& content) {
    QSqlQuery query(m_database);
    query.prepare(
        "INSERT INTO notifications (user_id, type, title, content, created_at) "
        "VALUES (:user_id, :type, :title, :content, NOW())"
    );
    
    query.bindValue(":user_id", userId);
    query.bindValue(":type", type);
    query.bindValue(":title", title);
    query.bindValue(":content", content);
    
    return query.exec();
}

QString LauncherDatabase::getUsername(AccountId accountId) {
    QSqlQuery query(m_database);
    query.prepare("SELECT username FROM launcher_users WHERE account_id = :account_id");
    query.bindValue(":account_id", accountId);
    
    if (query.exec() && query.next()) {
        return query.value("username").toString();
    }
    
    return "Unknown User";
}

void LauncherDatabase::close() {
    if (m_database.isOpen()) {
        m_database.close();
        LOG_INFO("Launcher database closed");
    }
}