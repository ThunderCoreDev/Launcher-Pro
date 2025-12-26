#pragma once
#include "ModuleInterface.h"
#include <QWidget>
#include <QTabWidget>
#include <QWebSocketServer>
#include <QtCharts>

class AdminModule : public QObject, public ModuleInterface {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)
    Q_PLUGIN_METADATA(IID "com.thundercore.ModuleInterface.AdminModule")
    
public:
    AdminModule();
    ~AdminModule();
    
    QString getName() const override { return "AdminModule"; }
    QString getVersion() const override { return "1.0.0"; }
    QString getDescription() const override { return "Panel de administración del servidor"; }
    QJsonObject getConfig() const override;
    QWidget* createWidget(QWidget* parent = nullptr) override;
    
    bool initialize() override;
    void shutdown() override;
    void update() override;
    
    QStringList getDependencies() const override { 
        return {"DatabaseModule", "StatsModule", "SocialModule"}; 
    }
    bool isCompatible(const QString& coreVersion) const override;
    
    // Funcionalidades de administración
    struct AdminStats {
        struct AccountStats {
            int total;
            int activeToday;
            int activeWeek;
            int banned;
            int premium;
        };
        
        struct CharacterStats {
            int total;
            int online;
            int maxLevel;
            int deleted;
            QMap<QString, int> byClass;
            QMap<QString, int> byRace;
        };
        
        struct EconomyStats {
            qint64 totalGold;
            qint64 averageGold;
            qint64 auctionHouseVolume;
            int auctionsActive;
            int itemsTradedToday;
        };
        
        struct PerformanceStats {
            float cpuUsage;
            qint64 memoryUsage;
            int queriesPerSecond;
            int activeConnections;
            float averageLatency;
        };
        
        AccountStats accounts;
        CharacterStats characters;
        EconomyStats economy;
        PerformanceStats performance;
        QDateTime lastUpdate;
    };
    
    // Funciones principales
    AdminStats getAdminStats() const;
    void sendGlobalAnnouncement(const QString& title, const QString& message, 
                               const QString& type = "info");
    void processSupportTickets();
    void manageBans();
    void manageAccounts();
    void manageCharacters();
    void runMaintenanceTasks();
    void backupDatabases();
    void viewLogs();
    void manageServerSettings();
    
    // Control de acceso
    enum class Permission {
        ViewDashboard,
        ManageAnnouncements,
        ManageTickets,
        ManageBans,
        ManageAccounts,
        ManageCharacters,
        RunMaintenance,
        ViewLogs,
        ManageSettings,
        FullAccess
    };
    
    bool hasPermission(quint32 accountId, Permission permission) const;
    
signals:
    void statsUpdated(const AdminStats& stats);
    void announcementSent(const QString& title, const QString& message);
    void ticketProcessed(quint64 ticketId, const QString& action);
    void banUpdated(quint32 accountId, const QString& action, const QString& reason);
    void accountModified(quint32 accountId, const QString& modification);
    void characterModified(quint64 characterGuid, const QString& modification);
    void maintenanceComplete(const QString& task, bool success);
    void backupComplete(const QString& filename, bool success);
    void logEntryAdded(const QString& entry);
    
private slots:
    void onRefreshClicked();
    void onSendAnnouncementClicked();
    void onProcessTicketsClicked();
    void onManageBansClicked();
    void onManageAccountsClicked();
    void onManageCharactersClicked();
    void onRunMaintenanceClicked();
    void onBackupClicked();
    void onViewLogsClicked();
    void onSettingsClicked();
    
    void onWebSocketConnected();
    void onWebSocketDisconnected();
    void onWebSocketMessageReceived(const QByteArray& message);
    
    void updateDashboard();
    void checkAlerts();
    
private:
    void setupUI();
    void setupWebSocketServer();
    void loadAdminStats();
    void loadPendingTickets();
    void loadRecentBans();
    void loadActiveAlerts();
    void updateCharts();
    void saveAuditLog(const QString& action, const QJsonObject& details);
    
    // Widgets principales
    QTabWidget* m_adminTabs;
    
    // Dashboard
    QWidget* m_dashboardTab;
    QChartView* m_activityChart;
    QChartView* m_economyChart;
    QChartView* m_performanceChart;
    QTableWidget* m_alertsTable;
    
    // Anuncios
    QWidget* m_announcementsTab;
    QTextEdit* m_announcementEditor;
    QComboBox* m_announcementType;
    QPushButton* m_sendAnnouncementButton;
    
    // Tickets
    QWidget* m_ticketsTab;
    QTableWidget* m_ticketsTable;
    QTextEdit* m_ticketResponse;
    QPushButton* m_processTicketButton;
    
    // Bans
    QWidget* m_bansTab;
    QTableWidget* m_bansTable;
    QLineEdit* m_banAccountEdit;
    QTextEdit* m_banReasonEdit;
    QPushButton* m_banButton;
    QPushButton* m_unbanButton;
    
    // Cuentas
    QWidget* m_accountsTab;
    QTableWidget* m_accountsTable;
    QLineEdit* m_searchAccountEdit;
    QPushButton* m_searchAccountButton;
    
    // Personajes
    QWidget* m_charactersTab;
    QTableWidget* m_charactersTable;
    QLineEdit* m_searchCharacterEdit;
    QPushButton* m_searchCharacterButton;
    
    // Mantenimiento
    QWidget* m_maintenanceTab;
    QTreeWidget* m_maintenanceTasks;
    QPushButton* m_runTaskButton;
    QProgressBar* m_taskProgress;
    
    // Logs
    QWidget* m_logsTab;
    QTextEdit* m_logsViewer;
    QComboBox* m_logLevelCombo;
    QPushButton* m_refreshLogsButton;
    QPushButton* m_clearLogsButton;
    
    // Configuración
    QWidget* m_settingsTab;
    QTreeWidget* m_settingsTree;
    QPushButton* m_saveSettingsButton;
    QPushButton* m_resetSettingsButton;
    
    // Datos
    AdminStats m_currentStats;
    QWebSocketServer* m_webSocketServer;
    QList<QWebSocket*> m_connectedClients;
    
    quint32 m_currentAdminId;
    QList<Permission> m_adminPermissions;
    
    // Temporizadores
    QTimer* m_updateTimer;
    QTimer* m_alertTimer;
    
    Q_DISABLE_COPY(AdminModule)
};