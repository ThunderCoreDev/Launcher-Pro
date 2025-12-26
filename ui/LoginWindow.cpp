#include "LoginWindow.h"
#include "core/Logger.h"
#include "core/Settings.h"
#include "database/DatabaseManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QMovie>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

LoginWindow::LoginWindow(QWidget* parent) 
    : QWidget(parent)
    , m_isLoggingIn(false)
    , m_dbManager(nullptr)
    , m_networkManager(nullptr) {
    
    setupUI();
    setupAnimations();
    setupConnections();
    
    LOG_INFO("LoginWindow created");
}

LoginWindow::~LoginWindow() {
    delete m_dbManager;
    delete m_networkManager;
}

void LoginWindow::setupUI() {
    // Layout principal
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(50, 50, 50, 50);
    mainLayout->setSpacing(20);
    
    // Logo
    QLabel* logoLabel = new QLabel(this);
    logoLabel->setPixmap(QPixmap(":/images/logo.png").scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(logoLabel);
    
    // Título
    QLabel* titleLabel = new QLabel("THUNDERCORE LAUNCHER PRO", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "font-size: 28px; font-weight: bold; color: #00aeff; "
        "text-shadow: 2px 2px 4px rgba(0,0,0,0.5);"
    );
    mainLayout->addWidget(titleLabel);
    
    // Subtítulo
    QLabel* subtitleLabel = new QLabel("Conéctate a la aventura", this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("font-size: 14px; color: #88aaff;");
    mainLayout->addWidget(subtitleLabel);
    
    // Formulario de login
    QFrame* loginFrame = new QFrame(this);
    loginFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    loginFrame->setStyleSheet(
        "QFrame { background: rgba(20, 30, 50, 0.8); "
        "border: 2px solid #2a3f6f; border-radius: 10px; }"
    );
    
    QFormLayout* formLayout = new QFormLayout(loginFrame);
    formLayout->setSpacing(15);
    formLayout->setContentsMargins(30, 30, 30, 30);
    
    // Campo de usuario
    QLabel* userLabel = new QLabel("Usuario:", this);
    userLabel->setStyleSheet("color: #aaccff; font-weight: bold;");
    
    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setPlaceholderText("Ingresa tu nombre de usuario");
    m_usernameEdit->setMinimumHeight(40);
    m_usernameEdit->setStyleSheet(
        "QLineEdit { background: rgba(15, 25, 45, 0.9); "
        "border: 1px solid #2a3f6f; border-radius: 5px; "
        "padding: 10px; color: white; font-size: 14px; }"
        "QLineEdit:focus { border: 1px solid #00aeff; }"
    );
    
    // Campo de contraseña
    QLabel* passLabel = new QLabel("Contraseña:", this);
    passLabel->setStyleSheet("color: #aaccff; font-weight: bold;");
    
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText("Ingresa tu contraseña");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setMinimumHeight(40);
    m_passwordEdit->setStyleSheet(
        "QLineEdit { background: rgba(15, 25, 45, 0.9); "
        "border: 1px solid #2a3f6f; border-radius: 5px; "
        "padding: 10px; color: white; font-size: 14px; }"
        "QLineEdit:focus { border: 1px solid #00aeff; }"
    );
    
    // Checkbox "Recordar"
    m_rememberCheck = new QCheckBox("Recordar usuario", this);
    m_rememberCheck->setStyleSheet("color: #88aaff;");
    
    formLayout->addRow(userLabel, m_usernameEdit);
    formLayout->addRow(passLabel, m_passwordEdit);
    formLayout->addRow("", m_rememberCheck);
    
    mainLayout->addWidget(loginFrame);
    
    // Botones
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);
    
    m_loginButton = new QPushButton("INICIAR SESIÓN", this);
    m_loginButton->setMinimumHeight(45);
    m_loginButton->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #00aeff, stop:1 #0088cc); color: white; font-weight: bold; "
        "border-radius: 5px; font-size: 14px; border: none; }"
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #00ccff, stop:1 #0099ff); }"
        "QPushButton:pressed { background: #006699; }"
        "QPushButton:disabled { background: #555555; color: #999999; }"
    );
    
    QPushButton* registerButton = new QPushButton("REGISTRARSE", this);
    registerButton->setMinimumHeight(45);
    registerButton->setStyleSheet(
        "QPushButton { background: transparent; color: #00aeff; "
        "border: 2px solid #00aeff; border-radius: 5px; font-size: 14px; }"
        "QPushButton:hover { background: rgba(0, 174, 255, 0.1); }"
    );
    
    buttonLayout->addWidget(m_loginButton);
    buttonLayout->addWidget(registerButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Enlaces inferiores
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(20);
    
    QPushButton* forgotButton = new QPushButton("¿Olvidaste tu contraseña?", this);
    forgotButton->setStyleSheet(
        "QPushButton { background: transparent; color: #88aaff; "
        "border: none; text-decoration: underline; }"
        "QPushButton:hover { color: #00aeff; }"
    );
    
    QPushButton* settingsButton = new QPushButton("Ajustes", this);
    settingsButton->setStyleSheet(
        "QPushButton { background: transparent; color: #88aaff; "
        "border: none; }"
        "QPushButton:hover { color: #00aeff; }"
    );
    
    bottomLayout->addWidget(forgotButton);
    bottomLayout->addStretch();
    bottomLayout->addWidget(settingsButton);
    
    mainLayout->addLayout(bottomLayout);
    
    // Estado del servidor
    m_statusLabel = new QLabel(this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("color: #88aaff; font-size: 12px;");
    mainLayout->addWidget(m_statusLabel);
    
    // Cargar usuario recordado
    loadRememberedUser();
    
    // Inicializar gestores
    m_dbManager = new DatabaseManager();
    m_networkManager = new QNetworkAccessManager(this);
    
    // Iniciar actualizaciones
    startUpdates();
}

void LoginWindow::setupAnimations() {
    // Efecto de opacidad
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(m_opacityEffect);
    
    // Animación de fade in
    m_fadeAnimation = new QPropertyAnimation(m_opacityEffect, "opacity");
    m_fadeAnimation->setDuration(1000);
    m_fadeAnimation->setStartValue(0.0);
    m_fadeAnimation->setEndValue(1.0);
    m_fadeAnimation->setEasingCurve(QEasingCurve::InOutQuad);
    
    // Grupo de animaciones
    m_loginAnimGroup = new QParallelAnimationGroup(this);
    m_loginAnimGroup->addAnimation(m_fadeAnimation);
    
    // Temporizador para animaciones de fondo
    m_animationTimer = new QTimer(this);
    m_animationTimer->setInterval(5000);
    connect(m_animationTimer, &QTimer::timeout, this, &LoginWindow::animateBackground);
}

void LoginWindow::setupConnections() {
    // Botones
    connect(m_loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    
    // Campos de texto (Enter para login)
    connect(m_usernameEdit, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
    
    // Animación inicial
    QTimer::singleShot(500, m_fadeAnimation, [this]() {
        m_fadeAnimation->start();
    });
    
    m_animationTimer->start();
}

void LoginWindow::onLoginClicked() {
    if (m_isLoggingIn) return;
    
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();
    
    if (username.isEmpty() || password.isEmpty()) {
        showError("Por favor, ingresa usuario y contraseña");
        return;
    }
    
    m_isLoggingIn = true;
    m_loginButton->setEnabled(false);
    m_loginButton->setText("CONECTANDO...");
    
    LOG_INFO("Attempting login for user: " + username);
    
    // Autenticar en un hilo separado
    QtConcurrent::run([this, username, password]() {
        bool success = m_dbManager->authenticateUser(username, password);
        
        QMetaObject::invokeMethod(this, [this, success, username]() {
            onLoginResult(success, username);
        });
    });
}

void LoginWindow::onLoginResult(bool success, const QString& username) {
    m_isLoggingIn = false;
    m_loginButton->setEnabled(true);
    m_loginButton->setText("INICIAR SESIÓN");
    
    if (success) {
        // Guardar usuario si está marcado "recordar"
        if (m_rememberCheck->isChecked()) {
            Settings::instance().setString("Auth/LastUsername", username);
            Settings::instance().save();
        }
        
        // Obtener account ID (en una implementación real)
        quint32 accountId = 1; // Esto debería venir de la autenticación
        
        showSuccess("¡Bienvenido, " + username + "!");
        
        // Emitir señal de éxito
        emit loginSuccess(username, accountId);
        
    } else {
        showError("Usuario o contraseña incorrectos");
        m_passwordEdit->clear();
        m_passwordEdit->setFocus();
    }
}

void LoginWindow::loadRememberedUser() {
    QString lastUser = Settings::instance().getString("Auth/LastUsername");
    if (!lastUser.isEmpty()) {
        m_usernameEdit->setText(lastUser);
        m_rememberCheck->setChecked(true);
        m_passwordEdit->setFocus();
    }
}

void LoginWindow::showError(const QString& message) {
    m_statusLabel->setText("<span style='color: #ff5555;'>✗ " + message + "</span>");
    
    // Animación de error
    QPropertyAnimation* shake = new QPropertyAnimation(this, "pos");
    shake->setDuration(100);
    shake->setKeyValueAt(0, pos());
    shake->setKeyValueAt(0.2, pos() + QPoint(5, 0));
    shake->setKeyValueAt(0.4, pos() + QPoint(-5, 0));
    shake->setKeyValueAt(0.6, pos() + QPoint(5, 0));
    shake->setKeyValueAt(0.8, pos() + QPoint(-5, 0));
    shake->setKeyValueAt(1, pos());
    shake->start(QAbstractAnimation::DeleteWhenStopped);
}

void LoginWindow::showSuccess(const QString& message) {
    m_statusLabel->setText("<span style='color: #00ff88;'>✓ " + message + "</span>");
}

void LoginWindow::animateBackground() {
    // Animación sutil de elementos
    QPropertyAnimation* pulse = new QPropertyAnimation(m_loginButton, "geometry");
    pulse->setDuration(300);
    pulse->setKeyValueAt(0, m_loginButton->geometry());
    pulse->setKeyValueAt(0.5, m_loginButton->geometry().adjusted(-2, -2, 2, 2));
    pulse->setKeyValueAt(1, m_loginButton->geometry());
    pulse->start(QAbstractAnimation::DeleteWhenStopped);
}

void LoginWindow::startUpdates() {
    // Temporizador para actualizar estado del servidor
    QTimer* serverTimer = new QTimer(this);
    connect(serverTimer, &QTimer::timeout, this, [this]() {
        updateServerStatus();
    });
    serverTimer->start(30000); // Cada 30 segundos
    
    // Actualizar inmediatamente
    updateServerStatus();
}

void LoginWindow::updateServerStatus() {
    // En una implementación real, esto consultaría al servidor
    ServerStats stats = m_dbManager->getServerStats();
    
    QString status = QString("Servidor: <span style='color: #00ff88;'>●</span> Online | "
                            "Jugadores: %1 (%2 Alianza, %3 Horda) | "
                            "Uptime: %4h")
                    .arg(stats.onlinePlayers)
                    .arg(stats.allianceOnline)
                    .arg(stats.hordeOnline)
                    .arg(stats.uptimeHours);
    
    m_statusLabel->setText(status);
}