#pragma once
#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include <QChartView>
#include <QtCharts>

class AdminPanel : public QWidget {
    Q_OBJECT
public:
    explicit AdminPanel(DatabaseManager* dbManager, QWidget* parent = nullptr);
    
private slots:
    void refreshAll();
    void sendGlobalAnnouncement();
    void processSelectedTickets();
    void banSelectedAccounts();
    void restoreSelectedCharacters();
    void createMassReward();
    void runDatabaseMaintenance();
    void exportStatistics();
    void backupDatabase();
    void showAuditLog();
    
private:
    void setupUI();
    void loadStatistics();
    void loadPendingTickets();
    void loadRecentBans();
    void loadServerPerformance();
    void createCharts();
    void updateDashboard();
    
    DatabaseManager* dbManager;
    
    // Pestañas principales
    QTabWidget* mainTabs;
    
    // Dashboard
    QWidget* dashboardTab;
    QChartView* onlineChart;
    QChartView* factionChart;
    QChartView* activityChart;
    
    // Gestión de usuarios
    QTableWidget* usersTable;
    QTableWidget* ticketsTable;
    QTableWidget* bansTable;
    
    // Herramientas
    QWidget* toolsTab;
    QTextEdit* announcementEditor;
    QTextEdit* massRewardEditor;
    QSpinBox* rewardAmount;
    QComboBox* rewardType;
    
    // Estadísticas
    struct DailyStats {
        QDate date;
        int logins;
        int newAccounts;
        int onlinePeak;
        int ticketsCreated;
        int ticketsResolved;
    };
    
    QList<DailyStats> loadDailyStats(int days = 30);
    
    // Temporizador de actualización
    QTimer* refreshTimer;
};