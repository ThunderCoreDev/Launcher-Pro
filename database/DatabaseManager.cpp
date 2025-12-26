#include "DatabaseManager.h"
#include "Logger.h"
#include "Settings.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QThread>
#include <QJsonDocument>

DatabaseManager::DatabaseManager(QObject* parent) 
    : QObject(parent) {
}

DatabaseManager::~DatabaseManager() {
    disconnect();
}

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::initialize() {
    LOG_INFO("Initializing DatabaseManager...");
    
    // Configuración desde Settings
    Settings& settings = Settings::instance();
    
    // Conexión a base de datos del launcher
    if (!connectToDatabase("launcher", 
                          settings.getString("Database/LauncherHost"),
                          settings.getInt("Database/LauncherPort"),
                          settings.getString("Database/LauncherUser"),
                          settings.getString("Database/LauncherPassword"),
                          "launcher")) {
        LOG_ERROR("Failed to connect to launcher database");
        return false;
    }
    
    // Conexión a base de datos de autenticación
    if (!connectToDatabase("auth",
                          settings.getString("Database/AuthHost"),
                          settings.getInt("Database/AuthPort"),
                          settings.getString("Database/AuthUser"),
                          settings.getString("Database/AuthPassword"),
                          "auth")) {
        LOG_WARNING("Failed to connect to auth database (some features disabled)");
    }
    
    // Crear tablas si no existen
    if (!createTables()) {
        LOG_ERROR("Failed to create database tables");
        return false;
    }
    
    LOG_INFO("DatabaseManager initialized successfully");
    return true;
}

bool DatabaseManager::connectToDatabase(const QString& connectionName, 
                                       const QString& host, int port,
                                       const QString& user, const QString& password,
                                       const QString& databaseName) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", connectionName);
    db.setHostName(host);
    db.setPort(port);
    db.setUserName(user);
    db.setPassword(password);
    db.setDatabaseName(databaseName);
    
    if (!db.open()) {
        LOG_ERROR("Failed to connect to database " + databaseName + 
                 ": " + db.lastError().text());
        return false;
    }
    
    m_connections[connectionName] = db;
    LOG_INFO("Connected to database: " + databaseName + 
            " on " + host + ":" + QString::number(port));
    
    return true;
}

bool DatabaseManager::authenticateUser(const QString& username, const QString& password) {
    QSqlDatabase authDb = m_connections.value("auth");
    if (!authDb.isValid()) {
        LOG_ERROR("Auth database not connected");
        return false;
    }
    
    QSqlQuery query(authDb);
    query.prepare(
        "SELECT id, username, salt, verifier, email, last_login, last_ip, "
        "locked, online, expansion, mutetime FROM account WHERE username = :username"
    );
    query.bindValue(":username", username);
    
    if (!query.exec()) {
        LOG_ERROR("Auth query failed: " + query.lastError().text());
        return false;
    }
    
    if (!query.next()) {
        LOG_WARNING("User not found: " + username);
        return false;
    }
    
    // Para WoW TrinityCore/AzerothCore (SHA1)
    QString salt = query.value("salt").toString();
    QString storedVerifier = query.value("verifier").toString();
    
    // Calcular verifier: SHA1(SHA1(username:password) + salt)
    QCryptographicHash hash1(QCryptographicHash::Sha1);
    hash1.addData((username + ":" + password).toUpper().toUtf8());
    QString firstHash = hash1.result().toHex();
    
    QCryptographicHash hash2(QCryptographicHash::Sha1);
    hash2.addData((firstHash + salt).toUtf8());
    QString calculatedVerifier = hash2.result().toHex();
    
    bool authenticated = (storedVerifier.toUpper() == calculatedVerifier.toUpper());
    
    if (authenticated) {
        quint32 accountId = query.value("id").toUInt();
        
        // Registrar en la base de datos del launcher
        registerLauncherUser(accountId, username);
        
        // Actualizar último login
        QSqlQuery updateQuery(authDb);
        updateQuery.prepare(
            "UPDATE account SET last_login = NOW(), online = 1 WHERE id = :id"
        );
        updateQuery.bindValue(":id", accountId);
        updateQuery.exec();
        
        LOG_INFO("User authenticated: " + username + " (ID: " + QString::number(accountId) + ")");
    } else {
        LOG_WARNING("Authentication failed for user: " + username);
    }
    
    return authenticated;
}

bool DatabaseManager::registerLauncherUser(quint32 accountId, const QString& username) {
    QSqlDatabase launcherDb = m_connections.value("launcher");
    if (!launcherDb.isValid()) {
        LOG_ERROR("Launcher database not connected");
        return false;
    }
    
    QSqlQuery query(launcherDb);
    query.prepare(
        "INSERT INTO launcher_users (account_id, username, level, experience, "
        "daily_login_streak, last_login_date, total_login_days, created_at) "
        "VALUES (:account_id, :username, 1, 0, 0, CURDATE(), 1, NOW()) "
        "ON DUPLICATE KEY UPDATE "
        "username = VALUES(username), "
        "last_login_date = VALUES(last_login_date), "
        "total_login_days = total_login_days + 1"
    );
    
    query.bindValue(":account_id", accountId);
    query.bindValue(":username", username);
    
    if (!query.exec()) {
        LOG_ERROR("Failed to register launcher user: " + query.lastError().text());
        return false;
    }
    
    // Registrar login diario
    QSqlQuery dailyQuery(launcherDb);
    dailyQuery.prepare("CALL RegisterDailyLogin(:account_id, LAST_INSERT_ID())");
    dailyQuery.bindValue(":account_id", accountId);
    dailyQuery.exec();
    
    return true;
}

QVector<CharacterInfo> DatabaseManager::getUserCharacters(quint32 accountId) {
    QVector<CharacterInfo> characters;
    
    QSqlDatabase charsDb = m_connections.value("characters");
    if (!charsDb.isValid()) {
        LOG_ERROR("Characters database not connected");
        return characters;
    }
    
    QSqlQuery query(charsDb);
    query.prepare(
        "SELECT guid, name, race, class, level, gender, "
        "zone, map, position_x, position_y, position_z, "
        "taximask, online, totaltime, totalKills, "
        "todayKills, yesterdayKills, totalHonorPoints, "
        "todayHonorPoints, yesterdayHonorPoints, "
        "totalKills, todayKills, yesterdayKills, "
        "chosenTitle, knownCurrencies, watchedFaction, "
        "drunk, health, power1, power2, power3, "
        "power4, power5, power6, power7, "
        "latency, talentGroupsCount, activeTalentGroup, "
        "exploredZones, equipmentCache, ammoId, "
        "knownTitles, actionBars, grantableLevels, "
        "deleteInfos_Account, deleteInfos_Name, "
        "deleteDate FROM characters WHERE account = :accountId"
    );
    
    query.bindValue(":accountId", accountId);
    
    if (!query.exec()) {
        LOG_ERROR("Failed to get user characters: " + query.lastError().text());
        return characters;
    }
    
    while (query.next()) {
        CharacterInfo charInfo;
        charInfo.guid = query.value("guid").toULongLong();
        charInfo.name = query.value("name").toString();
        charInfo.race = query.value("race").toString();
        charInfo.className = query.value("class").toString();
        charInfo.level = query.value("level").toInt();
        charInfo.zone = query.value("zone").toInt();
        charInfo.map = query.value("map").toInt();
        charInfo.posX = query.value("position_x").toFloat();
        charInfo.posY = query.value("position_y").toFloat();
        charInfo.posZ = query.value("position_z").toFloat();
        charInfo.online = query.value("online").toBool();
        charInfo.totalPlayTime = query.value("totaltime").toUInt();
        
        characters.append(charInfo);
    }
    
    LOG_DEBUG("Retrieved " + QString::number(characters.size()) + " characters for account " + 
              QString::number(accountId));
    
    return characters;
}

bool DatabaseManager::unstuckCharacter(quint64 characterGuid, const QString& characterName) {
    QSqlDatabase charsDb = m_connections.value("characters");
    if (!charsDb.isValid()) {
        LOG_ERROR("Characters database not connected");
        return false;
    }
    
    // Obtener homebind del personaje
    QSqlQuery homebindQuery(charsDb);
    homebindQuery.prepare(
        "SELECT mapId, zoneId, posX, posY, posZ FROM character_homebind WHERE guid = :guid"
    );
    homebindQuery.bindValue(":guid", characterGuid);
    
    float posX, posY, posZ;
    int mapId, zoneId;
    
    if (homebindQuery.exec() && homebindQuery.next()) {
        mapId = homebindQuery.value("mapId").toInt();
        zoneId = homebindQuery.value("zoneId").toInt();
        posX = homebindQuery.value("posX").toFloat();
        posY = homebindQuery.value("posY").toFloat();
        posZ = homebindQuery.value("posZ").toFloat();
    } else {
        // Valores por defecto (Stormwind para Alianza, Orgrimmar para Horda)
        // Necesitarías determinar la facción del personaje
        mapId = 0; // Eastern Kingdoms
        zoneId = 1519; // Stormwind City
        posX = -8949.95f;
        posY = -132.493f;
        posZ = 83.5312f;
    }
    
    // Actualizar posición del personaje
    QSqlQuery updateQuery(charsDb);
    updateQuery.prepare(
        "UPDATE characters SET "
        "position_x = :posX, position_y = :posY, position_z = :posZ, "
        "map = :mapId, zone = :zoneId, "
        "taximask = '0 0' WHERE guid = :guid"
    );
    
    updateQuery.bindValue(":posX", posX);
    updateQuery.bindValue(":posY", posY);
    updateQuery.bindValue(":posZ", posZ);
    updateQuery.bindValue(":mapId", mapId);
    updateQuery.bindValue(":zoneId", zoneId);
    updateQuery.bindValue(":guid", characterGuid);
    
    if (!updateQuery.exec()) {
        LOG_ERROR("Failed to unstuck character: " + updateQuery.lastError().text());
        return false;
    }
    
    LOG_INFO("Character unstuck: " + characterName + " (GUID: " + 
             QString::number(characterGuid) + ") teleported to home location");
    
    return true;
}

ServerStats DatabaseManager::getServerStats() {
    ServerStats stats;
    
    QSqlDatabase charsDb = m_connections.value("characters");
    if (!charsDb.isValid()) {
        return stats;
    }
    
    // Jugadores online
    QSqlQuery onlineQuery(charsDb);
    onlineQuery.prepare("SELECT COUNT(*) as online FROM characters WHERE online = 1");
    if (onlineQuery.exec() && onlineQuery.next()) {
        stats.onlinePlayers = onlineQuery.value("online").toInt();
    }
    
    // Distribución por facción
    QSqlQuery factionQuery(charsDb);
    factionQuery.prepare(
        "SELECT "
        "SUM(CASE WHEN race IN (1,3,4,7,11,22,25,29,30,32,34,37) THEN 1 ELSE 0 END) as alliance, "
        "SUM(CASE WHEN race IN (2,5,6,8,9,10,26,27,28,31,35,36) THEN 1 ELSE 0 END) as horde "
        "FROM characters WHERE online = 1"
    );
    
    if (factionQuery.exec() && factionQuery.next()) {
        stats.allianceOnline = factionQuery.value("alliance").toInt();
        stats.hordeOnline = factionQuery.value("horde").toInt();
    }
    
    // Uptime del servidor (desde auth database)
    QSqlDatabase authDb = m_connections.value("auth");
    if (authDb.isValid()) {
        QSqlQuery uptimeQuery(authDb);
        uptimeQuery.prepare("SELECT MAX(uptime) as max_uptime FROM uptime");
        if (uptimeQuery.exec() && uptimeQuery.next()) {
            stats.uptimeHours = uptimeQuery.value("max_uptime").toInt() / 3600;
        }
    }
    
    return stats;
}

bool DatabaseManager::createTables() {
    QSqlDatabase launcherDb = m_connections.value("launcher");
    if (!launcherDb.isValid()) {
        return false;
    }
    
    QSqlQuery query(launcherDb);
    
    // Tabla de usuarios del launcher
    QString createUsersTable = 
        "CREATE TABLE IF NOT EXISTS launcher_users ("
        "id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,"
        "account_id INT UNSIGNED NOT NULL UNIQUE,"
        "username VARCHAR(255) NOT NULL,"
        "avatar_url VARCHAR(500),"
        "level INT UNSIGNED DEFAULT 1,"
        "experience BIGINT UNSIGNED DEFAULT 0,"
        "daily_login_streak INT UNSIGNED DEFAULT 0,"
        "last_login_date DATE,"
        "total_login_days INT UNSIGNED DEFAULT 0,"
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
        "updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP"
        ")";
    
    if (!query.exec(createUsersTable)) {
        LOG_ERROR("Failed to create users table: " + query.lastError().text());
        return false;
    }
    
    LOG_INFO("Database tables created successfully");
    return true;
}

void DatabaseManager::disconnect() {
    for (auto& connection : m_connections) {
        connection.close();
    }
    m_connections.clear();
    QSqlDatabase::removeDatabase("launcher");
    QSqlDatabase::removeDatabase("auth");
    QSqlDatabase::removeDatabase("characters");
    
    LOG_INFO("Database connections closed");
}