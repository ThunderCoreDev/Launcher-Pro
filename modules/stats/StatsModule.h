#pragma once
#include "ModuleInterface.h"
#include <QWidget>
#include <QtCharts>

class StatsModule : public QObject, public ModuleInterface {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)
    Q_PLUGIN_METADATA(IID "com.thundercore.ModuleInterface.StatsModule")
    
public:
    StatsModule();
    ~StatsModule();
    
    QString getName() const override { return "StatsModule"; }
    QString getVersion() const override { return "1.0.0"; }
    QString getDescription() const override { return "Estadísticas del servidor en tiempo real"; }
    QJsonObject getConfig() const override;
    QWidget* createWidget(QWidget* parent = nullptr) override;
    
    bool initialize() override;
    void shutdown() override;
    void update() override;
    
    QStringList getDependencies() const override { return {"DatabaseModule"}; }
    bool isCompatible(const QString& coreVersion) const override;
    
    // Estructuras de datos
    struct ServerStats {
        int totalPlayers;
        int alliancePlayers;
        int hordePlayers;
        int onlinePlayers;
        int maxOnlineToday;
        int uptimeHours;
        float averageLatency;
        QDateTime lastRestart;
        QString realmName;
        QString realmType;
        int activeGuilds;
        int totalAccounts;
    };
    
    struct PlayerDistribution {
        QMap<QString, int> classDistribution;
        QMap<QString, int> raceDistribution;
        QMap<int, int> levelDistribution;
        QMap<QString, int> zoneDistribution;
    };
    
    struct PvPRankings {
        struct ArenaTeam {
            QString name;
            int rating;
            int seasonWins;
            int seasonGames;
            QString captain;
            QString faction;
        };
        
        QList<ArenaTeam> arena2v2;
        QList<ArenaTeam> arena3v3;
        QList<ArenaTeam> arena5v5;
        QMap<QString, int> bgStatistics;
    };
    
    struct PvERankings {
        struct RaidProgress {
            QString raidName;
            QString difficulty;
            int bossesTotal;
            int bossesKilled;
            QDateTime firstKill;
            QDateTime lastKill;
            QString topGuild;
        };
        
        QList<RaidProgress> raidProgress;
        QMap<QString, int> dungeonCompletions;
        QMap<QString, int> achievementPoints;
    };
    
    ServerStats getServerStats() const;
    PlayerDistribution getPlayerDistribution() const;
    PvPRankings getPvPRankings() const;
    PvERankings getPvERankings() const;
    
    // Gráficos
    QChart* createOnlinePlayersChart();
    QChart* createFactionDistributionChart();
    QChart* createClassDistributionChart();
    QChart* createUptimeChart();
    QChart* createActivityHeatmap();
    
signals:
    void statsUpdated(const ServerStats& stats);
    void playerDistributionUpdated(const PlayerDistribution& distribution);
    void pvpRankingsUpdated(const PvPRankings& rankings);
    void pveRankingsUpdated(const PvERankings& rankings);
    void serverStatusChanged(bool online);
    
private slots:
    void refreshStats();
    void exportStats();
    void showDetailedView();
    void onChartTypeChanged(int index);
    void onTimeRangeChanged(int days);
    
private:
    void setupUI();
    void loadServerStats();
    void loadPlayerDistribution();
    void loadPvPRankings();
    void loadPvERankings();
    void updateCharts();
    void saveStatsToDatabase();
    void loadHistoricalData();
    
    // Widgets
    QTabWidget* m_statsTabs;
    QChartView* m_onlineChartView;
    QChartView* m_factionChartView;
    QChartView* m_classChartView;
    QChartView* m_uptimeChartView;
    QTableWidget* m_statsTable;
    QTableWidget* m_pvpTable;
    QTableWidget* m_pveTable;
    
    // Controles
    QComboBox* m_chartTypeCombo;
    QComboBox* m_timeRangeCombo;
    QPushButton* m_refreshButton;
    QPushButton* m_exportButton;
    QPushButton* m_detailsButton;
    
    // Datos
    ServerStats m_currentStats;
    PlayerDistribution m_playerDistribution;
    PvPRankings m_pvpRankings;
    PvERankings m_pveRankings;
    
    QTimer* m_updateTimer;
    QDateTime m_lastUpdate;
    
    Q_DISABLE_COPY(StatsModule)
};