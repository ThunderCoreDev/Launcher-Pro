#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QVector>
#include <QMap>
#include "core/Singleton.h"
#include "core/Global.h"

struct CharacterInfo {
    quint64 guid;
    QString name;
    QString race;
    QString className;
    int level;
    int zone;
    int map;
    float posX;
    float posY;
    float posZ;
    bool online;
    quint32 totalPlayTime;
};

struct ServerStats {
    int onlinePlayers;
    int allianceOnline;
    int hordeOnline;
    int uptimeHours;
    float averageLatency;
    QDateTime lastRestart;
    QString realmName;
};

class DatabaseManager : public QObject, public Singleton<DatabaseManager> {
    Q_OBJECT
    
public:
    ~DatabaseManager();
    
    bool initialize();
    void disconnect();
    
    // Autenticación
    bool authenticateUser(const QString& username, const QString& password);
    
    // Personajes
    QVector<CharacterInfo> getUserCharacters(AccountId accountId);
    bool unstuckCharacter(CharacterGuid characterGuid, const QString& characterName);
    
    // Estadísticas del servidor
    ServerStats getServerStats();
    
    // Tienda
    bool processDonation(AccountId accountId, const QString& itemCode, int quantity);
    
    // Eventos
    QVector<QString> getActiveEvents();
    
    // Tickets
    bool createSupportTicket(AccountId accountId, const QString& subject, const QString& message);
    QVector<QPair<QString, QString>> getSupportTickets(AccountId accountId);
    
private:
    DatabaseManager();
    friend class Singleton<DatabaseManager>;
    
    bool connectToDatabase(const QString& connectionName, 
                          const QString& host, int port,
                          const QString& user, const QString& password,
                          const QString& databaseName);
    
    bool createTables();
    bool registerLauncherUser(AccountId accountId, const QString& username);
    
    QMap<QString, QSqlDatabase> m_connections;
    
    Q_DISABLE_COPY(DatabaseManager)
};

#endif // DATABASEMANAGER_H