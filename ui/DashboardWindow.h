#pragma once
#include <QMainWindow>
#include <QStackedWidget>
#include <QTabWidget>
#include <QDockWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include "modules/ModuleManager.h"

class DashboardWindow : public QMainWindow {
    Q_OBJECT
    
public:
    DashboardWindow(const QString& username, quint32 accountId, QWidget* parent = nullptr);
    ~DashboardWindow();
    
    void initializeDashboard();
    
protected:
    void closeEvent(QCloseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void changeEvent(QEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    
private slots:
    void onTabChanged(int index);
    void onModuleLoaded(const QString& moduleName);
    void onNotificationClicked(const QString& id, const QVariant& data);
    void onFriendRequest(quint32 accountId, const QString& username);
    void onPrivateMessage(quint32 fromId, const QString& message);
    void onServerEvent(const QString& eventName, const QDateTime& startTime);
    void onDailyRewardAvailable();
    void onAchievementUnlocked(const QString& achievementCode);
    
    // Acciones de menú
    void showProfile();
    void showFriends();
    void showMessages();
    void showAchievements();
    void showStore();
    void showEvents();
    void showCalendar();
    void showTalentCalculator();
    void showGearPlanner();
    void showAddonManager();
    void showSettings();
    void showAdminPanel();
    void launchGame();
    void logout();
    void exit();
    
    // Actualizaciones
    void updateServerStats();
    void refreshNews();
    void updateOnlineFriends();
    void checkForUpdates();
    void updateDailyRewards();
    
    // Sistema de notificaciones
    void showNotification(const QString& title, const QString& message, 
                         const QIcon& icon = QIcon());
    void showTrayNotification(const QString& title, const QString& message);
    
private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void setupDockWidgets();
    void setupSystemTray();
    void setupShortcuts();
    void loadUserData();
    void saveWindowState();
    void restoreWindowState();
    void applyTheme();
    void updateWelcomeMessage();
    
    // Crear widgets de dock
    QDockWidget* createServerStatsDock();
    QDockWidget* createOnlineFriendsDock();
    QDockWidget* createNewsFeedDock();
    QDockWidget* createQuickActionsDock();
    QDockWidget* createEventCalendarDock();
    QDockWidget* createNotificationsDock();
    
    // Widgets principales
    QStackedWidget* m_centralStack;
    QTabWidget* m_mainTabs;
    
    // Pestañas
    QWidget* m_homeTab;
    QWidget* m_charactersTab;
    QWidget* m_socialTab;
    QWidget* m_eventsTab;
    QWidget* m_storeTab;
    QWidget* m_toolsTab;
    QWidget* m_adminTab;
    
    // Widgets de dock
    QDockWidget* m_serverStatsDock;
    QDockWidget* m_onlineFriendsDock;
    QDockWidget* m_newsFeedDock;
    QDockWidget* m_quickActionsDock;
    QDockWidget* m_eventCalendarDock;
    QDockWidget* m_notificationsDock;
    
    // Módulos
    ModuleManager* m_moduleManager;
    
    // Tray icon
    QSystemTrayIcon* m_trayIcon;
    QMenu* m_trayMenu;
    
    // Estado del usuario
    QString m_username;
    quint32 m_accountId;
    int m_launcherLevel;
    int m_experience;
    QString m_avatarPath;
    
    // Temporizadores
    QTimer* m_statsTimer;
    QTimer* m_newsTimer;
    QTimer* m_friendsTimer;
    QTimer* m_updatesTimer;
    QTimer* m_rewardsTimer;
    
    // Estado de la ventana
    bool m_isCompactMode;
    bool m_isMaximized;
    QByteArray m_windowGeometry;
    
    Q_DISABLE_COPY(DashboardWindow)
};