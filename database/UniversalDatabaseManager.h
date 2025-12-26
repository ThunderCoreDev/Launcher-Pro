#pragma once
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QThreadPool>
#include "ExpansionSelector.h"

class DatabaseManager : public QObject {
    Q_OBJECT
public:
    DatabaseManager(QObject* parent = nullptr);
    ~DatabaseManager();
    
    bool initialize();
    
    // Autenticación universal
    bool authenticate(const QString& username, const QString& password);
    
    // Estadísticas del servidor
    struct ServerStats {
        int onlinePlayers;
        int allianceOnline;
        int hordeOnline;
        int uptimeHours;
        float averageLatency;
        QDateTime lastRestart;
        QString realmName;
        int maxPlayersToday;
        int uniqueAccounts;
        int activeGuilds;
    };
    
    ServerStats getServerStats();
    
    // Personajes
    struct CharacterInfo {
        quint64 guid;
        QString name;
        QString race;
        QString className;
        int level;
        int zone;
        int map;
        bool online;
        float posX, posY, posZ;
        quint32 totalPlayTime;
        quint32 money;
        QString guildName;
        int honorPoints;
        int arenaPoints;
        QDateTime lastLogin;
    };
    
    QList<CharacterInfo> getUserCharacters(quint32 accountId);
    bool unstuckCharacter(quint64 characterGuid);
    CharacterInfo getCharacterDetails(quint64 guid);
    
    // PvP Rankings
    struct PvPRanking {
        QString characterName;
        QString className;
        int rating2v2;
        int rating3v3;
        int rating5v5;
        int honorKills;
        int arenaWins;
        int rank;
    };
    
    QList<PvPRanking> getPvPRankings(int limit = 100);
    
    // PvE Rankings
    struct PvERanking {
        QString characterName;
        QString guildName;
        int raidProgress;
        int dungeonScore;
        QString bestRaid;
        QDateTime lastKill;
    };
    
    QList<PvERanking> getPvERankings();
    
    // Eventos
    struct GameEvent {
        QString name;
        QString description;
        QDateTime startTime;
        QDateTime endTime;
        bool active;
        QString type;
    };
    
    QList<GameEvent> getActiveEvents();
    
    // AutoBroadcast (Noticias)
    struct NewsItem {
        QString message;
        QDateTime timestamp;
        QString type;
        int priority;
    };
    
    QList<NewsItem> getAutoBroadcasts(int limit = 20);
    
    // Instancias bloqueadas
    struct InstanceLock {
        QString instanceName;
        QString difficulty;
        QDateTime resetTime;
        bool extended;
        quint32 remainingEncounters;
    };
    
    QList<InstanceLock> getCharacterInstanceLocks(quint64 characterGuid);
    
private:
    QSqlDatabase authDb;
    QSqlDatabase charactersDb;
    QSqlDatabase worldDb;
    QSqlDatabase launcherDb;
    QThreadPool queryPool;
    
    bool connectToDatabase(const QString& type, const QString& dbName);
    QString getTableName(const QString& baseName) const;
    ExpansionConfig currentConfig;
};