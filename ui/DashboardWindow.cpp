#include "DashboardWindow.h"
#include "core/Logger.h"
#include "core/Settings.h"
#include "database/DatabaseManager.h"
#include "database/LauncherDatabase.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QToolBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QListWidget>
#include <QTreeWidget>
#include <QTableWidget>
#include <QTextBrowser>
#include <QPushButton>
#include <QLabel>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QTimer>

DashboardWindow::DashboardWindow(const QString& username, AccountId accountId, QWidget* parent)
    : QMainWindow(parent)
    , m_username(username)
    , m_accountId(accountId)
    , m_trayIcon(nullptr)
    , m_isCompactMode(false) {
    
    setWindowTitle("ThunderNet Launcher Pro - Dashboard");
    setMinimumSize(1280, 720);
    
    setupUI();
    setupMenuBar();
    setupToolBar();
    setupStatusBar();
    setupDockWidgets();
    setupSystemTray();
    setupConnections();
    
    loadUserData();
    startTimers();
    
    LOG_INFO("Dashboard created for user: " + username);
}

DashboardWindow::~DashboardWindow() {
    LOG_INFO("Dashboard destroyed");
}

void DashboardWindow::setupUI() {
    // Widget central con pestañas
    m_mainTabs = new QTabWidget(this);
    m_mainTabs->setTabPosition(QTabWidget::North);
    m_mainTabs->setMovable(true);
    m_mainTabs->setTabsClosable(false);
    m_mainTabs->setStyleSheet(
        "QTabWidget::pane { border: 1px solid #2a3f6f; background: rgba(10, 20, 40, 0.9); }"
        "QTabBar::tab { background: #2a3f6f; color: #aaccff; padding: 8px 16px; }"
        "QTabBar::tab:selected { background: #00aeff; color: white; }"
        "QTabBar::tab:hover { background: rgba(0, 174, 255, 0.3); }"
    );
    
    setCentralWidget(m_mainTabs);
    
    // Crear pestañas
    createHomeTab();
    createCharactersTab();
    createSocialTab();
    createEventsTab();
    createStoreTab();
    createToolsTab();
    
    // Widget de bienvenida
    QWidget* welcomeWidget = new QWidget();
    QVBoxLayout* welcomeLayout = new QVBoxLayout(welcomeWidget);
    
    QLabel* welcomeLabel = new QLabel(QString("¡Bienvenido, <b>%1</b>!").arg(m_username));
    welcomeLabel->setStyleSheet("font-size: 24px; color: #00aeff; padding: 20px;");
    welcomeLabel->setAlignment(Qt::AlignCenter);
    
    QLabel* motdLabel = new QLabel("El servidor está funcionando perfectamente. ¡Disfruta de tu aventura!");
    motdLabel->setStyleSheet("font-size: 14px; color: #88aaff; padding: 10px;");
    motdLabel->setAlignment(Qt::AlignCenter);
    
    welcomeLayout->addStretch();
    welcomeLayout->addWidget(welcomeLabel);
    welcomeLayout->addWidget(motdLabel);
    welcomeLayout->addStretch();
    
    m_mainTabs->insertTab(0, welcomeWidget, "Inicio");
}

void DashboardWindow::createHomeTab() {
    QWidget* homeTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(homeTab);
    
    // Estadísticas rápidas
    QGroupBox* statsBox = new QGroupBox("Estadísticas Rápidas");
    statsBox->setStyleSheet(
        "QGroupBox { border: 2px solid #2a3f6f; border-radius: 5px; "
        "margin-top: 10px; font-weight: bold; color: #00aeff; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px 0 5px; }"
    );
    
    QGridLayout* statsLayout = new QGridLayout();
    
    QLabel* onlineLabel = new QLabel("Jugadores Online:");
    QLabel* onlineValue = new QLabel("0");
    onlineValue->setStyleSheet("color: #00ff88; font-weight: bold;");
    
    QLabel* allianceLabel = new QLabel("Alianza:");
    QLabel* allianceValue = new QLabel("0");
    allianceValue->setStyleSheet("color: #0070dd; font-weight: bold;");
    
    QLabel* hordeLabel = new QLabel("Horda:");
    QLabel* hordeValue = new QLabel("0");
    hordeValue->setStyleSheet("color: #c41f3b; font-weight: bold;");
    
    QLabel* uptimeLabel = new QLabel("Uptime:");
    QLabel* uptimeValue = new QLabel("0h");
    uptimeValue->setStyleSheet("color: #ffcc00; font-weight: bold;");
    
    statsLayout->addWidget(onlineLabel, 0, 0);
    statsLayout->addWidget(onlineValue, 0, 1);
    statsLayout->addWidget(allianceLabel, 1, 0);
    statsLayout->addWidget(allianceValue, 1, 1);
    statsLayout->addWidget(hordeLabel, 2, 0);
    statsLayout->addWidget(hordeValue, 2, 1);
    statsLayout->addWidget(uptimeLabel, 3, 0);
    statsLayout->addWidget(uptimeValue, 3, 1);
    
    statsBox->setLayout(statsLayout);
    
    // Noticias
    QGroupBox* newsBox = new QGroupBox("Últimas Noticias");
    newsBox->setStyleSheet(statsBox->styleSheet());
    
    QTextBrowser* newsBrowser = new QTextBrowser();
    newsBrowser->setStyleSheet(
        "QTextBrowser { background: rgba(15, 25, 45, 0.8); "
        "border: 1px solid #2a3f6f; border-radius: 3px; "
        "color: #cce0ff; padding: 10px; }"
    );
    
    QVBoxLayout* newsLayout = new QVBoxLayout();
    newsLayout->addWidget(newsBrowser);
    newsBox->setLayout(newsLayout);
    
    // Botón de jugar
    QPushButton* playButton = new QPushButton("🎮 JUGAR AHORA");
    playButton->setMinimumHeight(60);
    playButton->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #00cc66, stop:1 #00aa55); color: white; font-weight: bold; "
        "font-size: 18px; border-radius: 5px; border: 2px solid #008844; }"
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #00ff88, stop:1 #00cc66); border-color: #00ff88; }"
        "QPushButton:pressed { background: #008844; }"
    );
    
    connect(playButton, &QPushButton::clicked, this, &DashboardWindow::launchGame);
    
    layout->addWidget(statsBox);
    layout->addWidget(newsBox);
    layout->addWidget(playButton);
    
    m_mainTabs->addTab(homeTab, "🏠 Inicio");
}

void DashboardWindow::createCharactersTab() {
    QWidget* charactersTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(charactersTab);
    
    QLabel* titleLabel = new QLabel("Tus Personajes");
    titleLabel->setStyleSheet("font-size: 18px; color: #00aeff; font-weight: bold;");
    
    m_charactersTable = new QTableWidget();
    m_charactersTable->setColumnCount(7);
    m_charactersTable->setHorizontalHeaderLabels({
        "Nombre", "Clase", "Nivel", "Zona", "Online", "Acciones", ""
    });
    m_charactersTable->setStyleSheet(
        "QTableWidget { background: rgba(20, 30, 50, 0.8); "
        "border: 1px solid #2a3f6f; color: #cce0ff; }"
        "QHeaderView::section { background: #2a3f6f; color: #00aeff; "
        "padding: 5px; border: 1px solid #1a2f57; }"
    );
    
    QPushButton* refreshButton = new QPushButton("🔄 Actualizar");
    QPushButton* unstuckButton = new QPushButton("📍 Destrabar Seleccionado");
    unstuckButton->setStyleSheet(
        "QPushButton { background: #ff6b00; color: white; font-weight: bold; }"
        "QPushButton:hover { background: #ff8c00; }"
        "QPushButton:disabled { background: #555555; }"
    );
    
    connect(refreshButton, &QPushButton::clicked, this, &DashboardWindow::refreshCharacters);
    connect(unstuckButton, &QPushButton::clicked, this, &DashboardWindow::unstuckSelectedCharacter);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(refreshButton);
    buttonLayout->addWidget(unstuckButton);
    buttonLayout->addStretch();
    
    layout->addWidget(titleLabel);
    layout->addWidget(m_charactersTable);
    layout->addLayout(buttonLayout);
    
    m_mainTabs->addTab(charactersTab, "👤 Personajes");
}

void DashboardWindow::createSocialTab() {
    QWidget* socialTab = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(socialTab);
    
    // Lista de amigos
    QGroupBox* friendsBox = new QGroupBox("Amigos");
    friendsBox->setStyleSheet(
        "QGroupBox { border: 2px solid #2a3f6f; border-radius: 5px; "
        "margin-top: 10px; font-weight: bold; color: #00aeff; }"
    );
    
    m_friendsList = new QListWidget();
    m_friendsList->setStyleSheet(
        "QListWidget { background: rgba(15, 25, 45, 0.8); "
        "border: 1px solid #2a3f6f; color: #cce0ff; }"
        "QListWidget::item { padding: 8px; border-bottom: 1px solid #2a3f6f; }"
        "QListWidget::item:selected { background: rgba(0, 174, 255, 0.3); }"
    );
    
    QPushButton* addFriendButton = new QPushButton("➕ Añadir Amigo");
    QPushButton* chatButton = new QPushButton("💬 Chatear");
    
    QVBoxLayout* friendsLayout = new QVBoxLayout();
    friendsLayout->addWidget(m_friendsList);
    friendsLayout->addWidget(addFriendButton);
    friendsLayout->addWidget(chatButton);
    friendsBox->setLayout(friendsLayout);
    
    // Mensajes
    QGroupBox* messagesBox = new QGroupBox("Mensajes");
    messagesBox->setStyleSheet(friendsBox->styleSheet());
    
    m_messagesList = new QListWidget();
    m_messagesList->setStyleSheet(m_friendsList->styleSheet());
    
    QTextEdit* messageEdit = new QTextEdit();
    messageEdit->setPlaceholderText("Escribe tu mensaje aquí...");
    messageEdit->setMaximumHeight(100);
    
    QPushButton* sendButton = new QPushButton("📤 Enviar");
    
    QVBoxLayout* messagesLayout = new QVBoxLayout();
    messagesLayout->addWidget(m_messagesList);
    messagesLayout->addWidget(messageEdit);
    messagesLayout->addWidget(sendButton);
    messagesBox->setLayout(messagesLayout);
    
    layout->addWidget(friendsBox, 1);
    layout->addWidget(messagesBox, 2);
    
    m_mainTabs->addTab(socialTab, "👥 Social");
}

void DashboardWindow::setupMenuBar() {
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    
    // Menú Archivo
    QMenu* fileMenu = menuBar->addMenu("&Archivo");
    QAction* launchAction = fileMenu->addAction("🎮 &Lanzar Juego");
    launchAction->setShortcut(QKeySequence("Ctrl+L"));
    connect(launchAction, &QAction::triggered, this, &DashboardWindow::launchGame);
    
    fileMenu->addSeparator();
    
    QAction* settingsAction = fileMenu->addAction("⚙️ &Ajustes");
    settingsAction->setShortcut(QKeySequence::Preferences);
    connect(settingsAction, &QAction::triggered, this, &DashboardWindow::showSettings);
    
    QAction* logoutAction = fileMenu->addAction("🚪 &Cerrar Sesión");
    connect(logoutAction, &QAction::triggered, this, &DashboardWindow::logout);
    
    QAction* exitAction = fileMenu->addAction("❌ &Salir");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &DashboardWindow::close);
    
    // Menú Herramientas
    QMenu* toolsMenu = menuBar->addMenu("&Herramientas");
    QAction* calcAction = toolsMenu->addAction("🧮 Calculadora de Talentos");
    connect(calcAction, &QAction::triggered, this, &DashboardWindow::showTalentCalculator);
    
    QAction* gearAction = toolsMenu->addAction("🛡️ Planificador de Equipo");
    connect(gearAction, &QAction::triggered, this, &DashboardWindow::showGearPlanner);
    
    QAction* addonsAction = toolsMenu->addAction("📦 Gestor de Addons");
    connect(addonsAction, &QAction::triggered, this, &DashboardWindow::showAddonManager);
    
    toolsMenu->addSeparator();
    
    QAction* optimizeAction = toolsMenu->addAction("⚡ Optimizar Juego");
    connect(optimizeAction, &QAction::triggered, this, &DashboardWindow::showOptimizer);
    
    // Menú Ayuda
    QMenu* helpMenu = menuBar->addMenu("&Ayuda");
    QAction* docsAction = helpMenu->addAction("📚 Documentación");
    docsAction->setShortcut(QKeySequence::HelpContents);
    
    QAction* aboutAction = helpMenu->addAction("ℹ️ Acerca de");
    connect(aboutAction, &QAction::triggered, this, &DashboardWindow::showAbout);
}

void DashboardWindow::setupToolBar() {
    QToolBar* toolBar = addToolBar("Herramientas");
    toolBar->setMovable(false);
    toolBar->setIconSize(QSize(24, 24));
    toolBar->setStyleSheet(
        "QToolBar { background: rgba(20, 30, 50, 0.9); "
        "border-bottom: 1px solid #2a3f6f; spacing: 5px; }"
        "QToolButton { color: #aaccff; padding: 5px; }"
        "QToolButton:hover { background: rgba(0, 174, 255, 0.2); }"
    );
    
    QAction* homeAction = toolBar->addAction("🏠 Inicio");
    connect(homeAction, &QAction::triggered, this, [this]() {
        m_mainTabs->setCurrentIndex(0);
    });
    
    toolBar->addSeparator();
    
    QAction* charsAction = toolBar->addAction("👤 Personajes");
    connect(charsAction, &QAction::triggered, this, [this]() {
        m_mainTabs->setCurrentIndex(1);
    });
    
    QAction* socialAction = toolBar->addAction("👥 Social");
    connect(socialAction, &QAction::triggered, this, [this]() {
        m_mainTabs->setCurrentIndex(2);
    });
    
    QAction* storeAction = toolBar->addAction("🏪 Tienda");
    connect(storeAction, &QAction::triggered, this, [this]() {
        m_mainTabs->setCurrentIndex(4);
    });
    
    toolBar->addSeparator();
    
    QAction* playAction = toolBar->addAction("🎮 JUGAR");
    playAction->setIcon(QIcon(":/images/icons/play.png"));
    connect(playAction, &QAction::triggered, this, &DashboardWindow::launchGame);
}

void DashboardWindow::setupStatusBar() {
    QStatusBar* statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    
    m_statusLabel = new QLabel("Conectado a ThunderCore");
    m_statusLabel->setStyleSheet("color: #88aaff;");
    
    m_pingLabel = new QLabel("Ping: -- ms");
    m_pingLabel->setStyleSheet("color: #00ff88;");
    
    m_timeLabel = new QLabel();
    m_timeLabel->setStyleSheet("color: #ffcc00;");
    
    statusBar->addWidget(m_statusLabel, 1);
    statusBar->addPermanentWidget(m_pingLabel);
    statusBar->addPermanentWidget(m_timeLabel);
    
    // Actualizar hora cada segundo
    QTimer* timeTimer = new QTimer(this);
    connect(timeTimer, &QTimer::timeout, this, [this]() {
        m_timeLabel->setText(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss"));
    });
    timeTimer->start(1000);
}

void DashboardWindow::setupDockWidgets() {
    // Dock izquierdo - Estadísticas rápidas
    QDockWidget* statsDock = new QDockWidget("Estadísticas", this);
    statsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    statsDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    
    QWidget* statsWidget = new QWidget();
    QVBoxLayout* statsLayout = new QVBoxLayout(statsWidget);
    
    // Aquí irían widgets de estadísticas
    addDockWidget(Qt::LeftDockWidgetArea, statsDock);
    
    // Dock derecho - Amigos online
    QDockWidget* friendsDock = new QDockWidget("Amigos Online", this);
    friendsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    
    QListWidget* onlineFriends = new QListWidget();
    onlineFriends->setStyleSheet(
        "QListWidget { background: transparent; color: #cce0ff; }"
        "QListWidget::item { padding: 5px; }"
    );
    
    friendsDock->setWidget(onlineFriends);
    addDockWidget(Qt::RightDockWidgetArea, friendsDock);
}

void DashboardWindow::setupSystemTray() {
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        m_trayIcon = new QSystemTrayIcon(QIcon(":/images/icon.png"), this);
        
        QMenu* trayMenu = new QMenu(this);
        trayMenu->addAction("Mostrar", this, &DashboardWindow::show);
        trayMenu->addAction("Ocultar", this, &DashboardWindow::hide);
        trayMenu->addSeparator();
        trayMenu->addAction("Lanzar Juego", this, &DashboardWindow::launchGame);
        trayMenu->addSeparator();
        trayMenu->addAction("Salir", qApp, &QApplication::quit);
        
        m_trayIcon->setContextMenu(trayMenu);
        m_trayIcon->setToolTip("ThunderCore Launcher Pro");
        m_trayIcon->show();
        
        connect(m_trayIcon, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
            if (reason == QSystemTrayIcon::DoubleClick) {
                if (isVisible()) hide();
                else show();
            }
        });
    }
}

void DashboardWindow::setupConnections() {
    // Conectar cambio de pestaña
    connect(m_mainTabs, &QTabWidget::currentChanged, this, [this](int index) {
        LOG_DEBUG("Tab changed to: " + QString::number(index));
    });
}

void DashboardWindow::loadUserData() {
    // Cargar estadísticas del usuario
    QJsonObject stats = LauncherDatabase::instance().getUserStats(m_accountId);
    
    // Cargar personajes
    refreshCharacters();
    
    // Cargar amigos
    refreshFriends();
    
    // Cargar mensajes
    refreshMessages();
}

void DashboardWindow::refreshCharacters() {
    QVector<CharacterInfo> characters = DatabaseManager::instance().getUserCharacters(m_accountId);
    
    m_charactersTable->setRowCount(characters.size());
    
    for (int i = 0; i < characters.size(); ++i) {
        const CharacterInfo& charInfo = characters[i];
        
        // Nombre
        QTableWidgetItem* nameItem = new QTableWidgetItem(charInfo.name);
        nameItem->setForeground(QBrush(QColor("#ffffff")));
        
        // Clase
        QTableWidgetItem* classItem = new QTableWidgetItem(charInfo.className);
        
        // Nivel
        QTableWidgetItem* levelItem = new QTableWidgetItem(QString::number(charInfo.level));
        levelItem->setTextAlignment(Qt::AlignCenter);
        
        // Online
        QTableWidgetItem* onlineItem = new QTableWidgetItem(charInfo.online ? "✅" : "❌");
        onlineItem->setTextAlignment(Qt::AlignCenter);
        
        // Acciones
        QWidget* actionWidget = new QWidget();
        QHBoxLayout* actionLayout = new QHBoxLayout(actionWidget);
        actionLayout->setContentsMargins(2, 2, 2, 2);
        actionLayout->setSpacing(2);
        
        QPushButton* selectButton = new QPushButton("Seleccionar");
        selectButton->setStyleSheet(
            "QPushButton { background: #2a3f6f; color: #aaccff; "
            "border: 1px solid #1a2f57; padding: 3px; font-size: 11px; }"
            "QPushButton:hover { background: #3a4f7f; }"
        );
        
        QPushButton* unstuckButton = new QPushButton("Destrabar");
        unstuckButton->setProperty("characterGuid", charInfo.guid);
        unstuckButton->setProperty("characterName", charInfo.name);
        unstuckButton->setStyleSheet(
            "QPushButton { background: #ff6b00; color: white; "
            "border: 1px solid #cc5500; padding: 3px; font-size: 11px; }"
            "QPushButton:hover { background: #ff8c00; }"
        );
        
        connect(unstuckButton, &QPushButton::clicked, this, [this, charInfo]() {
            unstuckCharacter(charInfo.guid, charInfo.name);
        });
        
        actionLayout->addWidget(selectButton);
        actionLayout->addWidget(unstuckButton);
        actionLayout->addStretch();
        
        m_charactersTable->setItem(i, 0, nameItem);
        m_charactersTable->setItem(i, 1, classItem);
        m_charactersTable->setItem(i, 2, levelItem);
        m_charactersTable->setItem(i, 3, new QTableWidgetItem(QString::number(charInfo.zone)));
        m_charactersTable->setItem(i, 4, onlineItem);
        m_charactersTable->setCellWidget(i, 5, actionWidget);
    }
}

void DashboardWindow::refreshFriends() {
    QJsonArray friends = LauncherDatabase::instance().getUserFriends(m_accountId);
    
    m_friendsList->clear();
    for (const QJsonValue& friendValue : friends) {
        QJsonObject friendObj = friendValue.toObject();
        
        QString status = friendObj["online"].toBool() ? "🟢" : "⚫";
        QString itemText = QString("%1 %2 - %3")
            .arg(status)
            .arg(friendObj["username"].toString())
            .arg(friendObj["last_login"].toString());
        
        QListWidgetItem* item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, friendObj["account_id"].toInt());
        m_friendsList->addItem(item);
    }
}

void DashboardWindow::startTimers() {
    // Actualizar estadísticas cada 30 segundos
    QTimer* statsTimer = new QTimer(this);
    connect(statsTimer, &QTimer::timeout, this, [this]() {
        updateServerStats();
        refreshFriends();
    });
    statsTimer->start(30000);
    
    // Actualizar personajes cada minuto
    QTimer* charsTimer = new QTimer(this);
    connect(charsTimer, &QTimer::timeout, this, &DashboardWindow::refreshCharacters);
    charsTimer->start(60000);
    
    // Actualizar mensajes cada 15 segundos
    QTimer* messagesTimer = new QTimer(this);
    connect(messagesTimer, &QTimer::timeout, this, &DashboardWindow::refreshMessages);
    messagesTimer->start(15000);
}

// Métodos de slots
void DashboardWindow::launchGame() {
    LOG_INFO("Launching game for user: " + m_username);
    
    // Obtener configuración del juego
    Settings& settings = Settings::instance();
    QString gamePath = settings.getString("Game/DefaultPath");
    QString realmlist = settings.getString("Game/Realmlist");
    
    if (gamePath.isEmpty()) {
        QMessageBox::warning(this, "Ruta no configurada", 
                            "Por favor, configura la ruta del cliente WoW en Ajustes.");
        showSettings();
        return;
    }
    
    QString wowExe = gamePath + "/WoW.exe";
    if (!QFile::exists(wowExe)) {
        QMessageBox::critical(this, "Error", 
                            "No se encontró WoW.exe en la ruta especificada.");
        return;
    }
    
    // Actualizar realmlist
    QFile realmlistFile(gamePath + "/realmlist.wtf");
    if (realmlistFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&realmlistFile);
        stream << "set realmlist " << realmlist << "\n";
        stream << "set patchlist " << realmlist << "\n";
        realmlistFile.close();
    }
    
    // Lanzar juego
    QProcess* gameProcess = new QProcess(this);
    gameProcess->setProgram(wowExe);
    gameProcess->setArguments({"-console"});
    gameProcess->setWorkingDirectory(gamePath);
    
    if (!gameProcess->startDetached()) {
        QMessageBox::critical(this, "Error", 
                            "No se pudo iniciar el juego. Verifica los permisos.");
    } else {
        statusBar()->showMessage("Juego iniciado", 5000);
        
        if (m_trayIcon) {
            m_trayIcon->showMessage("ThunderCore Launcher", 
                                  "World of Warcraft se está iniciando...",
                                  QSystemTrayIcon::Information, 3000);
        }
    }
}

void DashboardWindow::showSettings() {
    // Crear diálogo de ajustes
    QDialog settingsDialog(this);
    settingsDialog.setWindowTitle("Ajustes - ThunderCore Launcher Pro");
    settingsDialog.setMinimumSize(600, 400);
    
    QTabWidget* settingsTabs = new QTabWidget(&settingsDialog);
    
    // Pestaña de Juego
    QWidget* gameTab = new QWidget();
    QFormLayout* gameLayout = new QFormLayout(gameTab);
    
    QLineEdit* pathEdit = new QLineEdit();
    pathEdit->setPlaceholderText("C:\\Program Files\\World of Warcraft");
    
    QPushButton* browseButton = new QPushButton("Examinar...");
    connect(browseButton, &QPushButton::clicked, this, [pathEdit]() {
        QString dir = QFileDialog::getExistingDirectory(nullptr, 
            "Seleccionar carpeta de WoW", 
            pathEdit->text());
        if (!dir.isEmpty()) {
            pathEdit->setText(dir);
        }
    });
    
    QHBoxLayout* pathLayout = new QHBoxLayout();
    pathLayout->addWidget(pathEdit);
    pathLayout->addWidget(browseButton);
    
    QLineEdit* realmlistEdit = new QLineEdit();
    realmlistEdit->setPlaceholderText("logon.tuserver.com");
    
    QComboBox* expansionCombo = new QComboBox();
    expansionCombo->addItems({"Clásico", "TBC", "WotLK", "Cataclysm", "MoP", "WoD", "Legion", "BfA", "Shadowlands", "Dragonflight"});
    
    QComboBox* emulatorCombo = new QComboBox();
    emulatorCombo->addItems({"TrinityCore", "AzerothCore", "CMaNGOS", "SkyFire", "MangOS"});
    
    QCheckBox* autoPatchCheck = new QCheckBox("Actualizar automáticamente");
    QCheckBox* clearCacheCheck = new QCheckBox("Limpiar caché al iniciar");
    
    gameLayout->addRow("Ruta del juego:", pathLayout);
    gameLayout->addRow("Realmlist:", realmlistEdit);
    gameLayout->addRow("Expansión:", expansionCombo);
    gameLayout->addRow("Emulador:", emulatorCombo);
    gameLayout->addRow(autoPatchCheck);
    gameLayout->addRow(clearCacheCheck);
    
    // Pestaña de Interfaz
    QWidget* uiTab = new QWidget();
    QFormLayout* uiLayout = new QFormLayout(uiTab);
    
    QComboBox* themeCombo = new QComboBox();
    themeCombo->addItems({"Battle.Net", "Oscuro", "Claro", "Personalizado"});
    
    QCheckBox* animationsCheck = new QCheckBox("Habilitar animaciones");
    QCheckBox* effectsCheck = new QCheckBox("Habilitar efectos");
    
    QSlider* opacitySlider = new QSlider(Qt::Horizontal);
    opacitySlider->setRange(30, 100);
    opacitySlider->setValue(100);
    
    uiLayout->addRow("Tema:", themeCombo);
    uiLayout->addRow("Opacidad:", opacitySlider);
    uiLayout->addRow(animationsCheck);
    uiLayout->addRow(effectsCheck);
    
    settingsTabs->addTab(gameTab, "🎮 Juego");
    settingsTabs->addTab(uiTab, "🎨 Interfaz");
    
    // Botones
    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply,
        Qt::Horizontal, &settingsDialog);
    
    connect(buttons, &QDialogButtonBox::accepted, &settingsDialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &settingsDialog, &QDialog::reject);
    connect(buttons->button(QDialogButtonBox::Apply), &QPushButton::clicked, 
            &settingsDialog, [&settingsDialog]() { settingsDialog.accept(); });
    
    QVBoxLayout* mainLayout = new QVBoxLayout(&settingsDialog);
    mainLayout->addWidget(settingsTabs);
    mainLayout->addWidget(buttons);
    
    // Cargar valores actuales
    Settings& settings = Settings::instance();
    pathEdit->setText(settings.getString("Game/DefaultPath"));
    realmlistEdit->setText(settings.getString("Game/Realmlist"));
    
    QString expansion = settings.getString("Game/Expansion");
    int expansionIndex = expansionCombo->findText(expansion, Qt::MatchContains);
    if (expansionIndex >= 0) expansionCombo->setCurrentIndex(expansionIndex);
    
    QString emulator = settings.getString("Game/Emulator");
    int emulatorIndex = emulatorCombo->findText(emulator, Qt::MatchContains);
    if (emulatorIndex >= 0) emulatorCombo->setCurrentIndex(emulatorIndex);
    
    autoPatchCheck->setChecked(settings.getBool("Game/AutoPatch"));
    clearCacheCheck->setChecked(settings.getBool("Game/ClearCacheOnLaunch"));
    
    QString theme = settings.getString("Window/Theme");
    int themeIndex = themeCombo->findText(theme, Qt::MatchContains);
    if (themeIndex >= 0) themeCombo->setCurrentIndex(themeIndex);
    
    animationsCheck->setChecked(settings.getBool("Window/EnableAnimations"));
    effectsCheck->setChecked(settings.getBool("Window/EnableEffects"));
    opacitySlider->setValue(settings.getInt("Window/Opacity"));
    
    if (settingsDialog.exec() == QDialog::Accepted) {
        // Guardar configuración
        settings.setString("Game/DefaultPath", pathEdit->text());
        settings.setString("Game/Realmlist", realmlistEdit->text());
        settings.setString("Game/Expansion", expansionCombo->currentText());
        settings.setString("Game/Emulator", emulatorCombo->currentText());
        settings.setBool("Game/AutoPatch", autoPatchCheck->isChecked());
        settings.setBool("Game/ClearCacheOnLaunch", clearCacheCheck->isChecked());
        
        settings.setString("Window/Theme", themeCombo->currentText());
        settings.setBool("Window/EnableAnimations", animationsCheck->isChecked());
        settings.setBool("Window/EnableEffects", effectsCheck->isChecked());
        settings.setInt("Window/Opacity", opacitySlider->value());
        
        settings.save();
        
        QMessageBox::information(this, "Ajustes", 
                               "Los cambios se aplicarán la próxima vez que inicies el launcher.");
    }
}

void DashboardWindow::unstuckCharacter(CharacterGuid guid, const QString& name) {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Destrabar Personaje",
        QString("¿Estás seguro de que quieres destrabar a <b>%1</b>?\n\n"
               "El personaje será teletransportado a su ubicación inicial.")
               .arg(name),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (DatabaseManager::instance().unstuckCharacter(guid, name)) {
            QMessageBox::information(this, "Éxito", 
                QString("El personaje <b>%1</b> ha sido destrabado exitosamente.")
                .arg(name));
            
            refreshCharacters();
        } else {
            QMessageBox::critical(this, "Error", 
                "No se pudo destrabar el personaje. Inténtalo de nuevo más tarde.");
        }
    }
}

void DashboardWindow::unstuckSelectedCharacter() {
    int row = m_charactersTable->currentRow();
    if (row >= 0) {
        QString name = m_charactersTable->item(row, 0)->text();
        
        // En una implementación real, obtendrías el GUID del personaje
        CharacterGuid guid = 0; // Esto debería venir de los datos
        
        unstuckCharacter(guid, name);
    } else {
        QMessageBox::warning(this, "Selección", 
                           "Por favor, selecciona un personaje de la lista.");
    }
}

void DashboardWindow::updateServerStats() {
    ServerStats stats = DatabaseManager::instance().getServerStats();
    
    // Actualizar UI con las estadísticas
    // (Implementación específica depende de cómo muestres las estadísticas)
}

void DashboardWindow::logout() {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Cerrar Sesión",
        "¿Estás seguro de que quieres cerrar sesión?",
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        emit logoutRequested();
        close();
    }
}

void DashboardWindow::closeEvent(QCloseEvent* event) {
    if (m_trayIcon && m_trayIcon->isVisible()) {
        hide();
        event->ignore();
        
        if (m_trayIcon) {
            m_trayIcon->showMessage("ThunderCore Launcher",
                                  "El launcher continúa ejecutándose en segundo plano.",
                                  QSystemTrayIcon::Information, 3000);
        }
    } else {
        QMainWindow::closeEvent(event);
    }
}

void DashboardWindow::showAbout() {
    QMessageBox::about(this, "Acerca de ThunderCore Launcher Pro",
        "<h3>ThunderNet Launcher Pro</h3>"
        "<p><b>Versión:</b> 1.0.0</p>"
        "<p><b>Compilación:</b> " __DATE__ " " __TIME__ "</p>"
        "<p><b>Sistema:</b> " + QSysInfo::prettyProductName() + "</p>"
        "<p><b>Qt:</b> " + QString(QT_VERSION_STR) + "</p>"
        "<hr>"
        "<p>Launcher avanzado para servidores World of Warcraft</p>"
        "<p>© 2026 ThunderCore WoW. Todos los derechos reservados.</p>"
        "<p><a href='https://thundercore.com'>https://thundercore.com</a></p>");
}