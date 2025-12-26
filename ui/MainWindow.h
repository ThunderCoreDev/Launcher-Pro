#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "ui/LoginWindow.h"
#include "ui/DashboardWindow.h"

QT_BEGIN_NAMESPACE
class QMenuBar;
class QStatusBar;
class QSystemTrayIcon;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    
protected:
    void closeEvent(QCloseEvent* event) override;
    
private slots:
    void onLoginSuccess(const QString& username, quint32 accountId);
    void onRegisterRequested();
    void onSettingsRequested();
    void onLogout();
    
    void showAboutDialog();
    void showDocumentation();
    
private:
    void setupUI();
    void setupMenuBar();
    void setupConnections();
    
    QStackedWidget* m_stackedWidget;
    LoginWindow* m_loginWindow;
    DashboardWindow* m_dashboardWindow;
    
    Q_DISABLE_COPY(MainWindow)
};

#endif // MAINWINDOW_H