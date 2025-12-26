#pragma once
#include <QWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QNetworkAccessManager>
#include "database/DatabaseManager.h"

class LoginWindow : public QWidget {
    Q_OBJECT
    
public:
    LoginWindow(QWidget* parent = nullptr);
    ~LoginWindow();
    
    void startAnimations();
    void setFocusOnUsername();
    
signals:
    void loginSuccess(const QString& username, quint32 accountId);
    void registerRequested();
    void recoveryRequested();
    void settingsRequested();
    
private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onForgotPasswordClicked();
    void onSettingsClicked();
    void onExitClicked();
    
    void onLoginResult(bool success, const QString& message, quint32 accountId = 0);
    void onNewsLoaded(const QString& news);
    void onServerStatusLoaded(bool online, int players, const QString& message);
    
    void animateBackground();
    void updateServerTime();
    
private:
    void setupUI();
    void setupAnimations();
    void setupConnections();
    void loadNews();
    void checkServerStatus();
    bool validateInput();
    void showError(const QString& message);
    void showSuccess(const QString& message);
    void updateRealmlistFile();
    
    // Widgets
    QLabel* m_backgroundLabel;
    QLabel* m_logoLabel;
    QLabel* m_titleLabel;
    QLabel* m_versionLabel;
    QLineEdit* m_usernameEdit;
    QLineEdit* m_passwordEdit;
    QCheckBox* m_rememberCheck;
    QPushButton* m_loginButton;
    QPushButton* m_registerButton;
    QPushButton* m_forgotButton;
    QPushButton* m_settingsButton;
    QPushButton* m_exitButton;
    QLabel* m_statusLabel;
    QLabel* m_newsLabel;
    QLabel* m_statsLabel;
    
    // Animaciones
    QPropertyAnimation* m_fadeAnimation;
    QPropertyAnimation* m_slideAnimation;
    QGraphicsOpacityEffect* m_opacityEffect;
    QParallelAnimationGroup* m_loginAnimGroup;
    
    // Gestores
    DatabaseManager* m_dbManager;
    QNetworkAccessManager* m_networkManager;
    
    // Estado
    bool m_isLoggingIn;
    QString m_lastUsername;
    QString m_gamePath;
    QString m_realmlist;
    
    // Temporizadores
    QTimer* m_animationTimer;
    QTimer* m_newsTimer;
    QTimer* m_statsTimer;
    
    Q_DISABLE_COPY(LoginWindow)
};