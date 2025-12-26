#include "MainWindow.h"
#include "LoginWindow.h"
#include "DashboardWindow.h"
#include "core/Logger.h"
#include "core/Settings.h"
#include "database/DatabaseManager.h"
#include <QStackedWidget>
#include <QStatusBar>
#include <QMenuBar>
#include <QApplication>

MainWindow::MainWindow(QWidget* parent) 
    : QMainWindow(parent)
    , m_stackedWidget(nullptr)
    , m_loginWindow(nullptr)
    , m_dashboardWindow(nullptr) {
    
    setupUI();
    setupConnections();
    
    LOG_INFO("MainWindow created");
}

MainWindow::~MainWindow() {
    LOG_INFO("MainWindow destroyed");
}

void MainWindow::setupUI() {
    // Configurar ventana principal
    setWindowTitle("ThunderCore Launcher Pro");
    setMinimumSize(1024, 768);
    
    // Crear widget apilado
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);
    
    // Crear ventana de login
    m_loginWindow = new LoginWindow();
    m_stackedWidget->addWidget(m_loginWindow);
    
    // Barra de estado
    statusBar()->showMessage("Listo");
    
    // Menú
    setupMenuBar();
    
    LOG_DEBUG("MainWindow UI setup complete");
}

void MainWindow::setupMenuBar() {
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    
    // Menú Archivo
    QMenu* fileMenu = menuBar->addMenu("&Archivo");
    QAction* exitAction = fileMenu->addAction("&Salir");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);
    
    // Menú Ayuda
    QMenu* helpMenu = menuBar->addMenu("&Ayuda");
    QAction* aboutAction = helpMenu->addAction("&Acerca de");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);
    
    QAction* docsAction = helpMenu->addAction("&Documentación");
    docsAction->setShortcut(QKeySequence::HelpContents);
    connect(docsAction, &QAction::triggered, this, &MainWindow::showDocumentation);
}

void MainWindow::setupConnections() {
    // Conectar señales de login
    connect(m_loginWindow, &LoginWindow::loginSuccess, 
            this, &MainWindow::onLoginSuccess);
    connect(m_loginWindow, &LoginWindow::registerRequested,
            this, &MainWindow::onRegisterRequested);
    connect(m_loginWindow, &LoginWindow::settingsRequested,
            this, &MainWindow::onSettingsRequested);
}

void MainWindow::onLoginSuccess(const QString& username, quint32 accountId) {
    LOG_INFO("Login successful for user: " + username);
    
    // Crear dashboard
    m_dashboardWindow = new DashboardWindow(username, accountId);
    m_stackedWidget->addWidget(m_dashboardWindow);
    m_stackedWidget->setCurrentWidget(m_dashboardWindow);
    
    // Conectar señal de logout
    connect(m_dashboardWindow, &DashboardWindow::logoutRequested,
            this, &MainWindow::onLogout);
    
    statusBar()->showMessage("Bienvenido, " + username);
}

void MainWindow::onLogout() {
    LOG_INFO("Logout requested");
    
    if (m_dashboardWindow) {
        m_stackedWidget->removeWidget(m_dashboardWindow);
        delete m_dashboardWindow;
        m_dashboardWindow = nullptr;
    }
    
    m_stackedWidget->setCurrentWidget(m_loginWindow);
    statusBar()->showMessage("Sesión cerrada");
}

void MainWindow::showAboutDialog() {
    QMessageBox::about(this, "Acerca de ThunderNet Launcher Pro",
        "<h3>ThunderCore Launcher Pro</h3>"
        "<p>Versión: 1.0.0</p>"
        "<p>Launcher avanzado para servidores World of Warcraft</p>"
        "<p>© 2026 ThunderCore WoW. Todos los derechos reservados.</p>");
}

void MainWindow::showDocumentation() {
    QDesktopServices::openUrl(QUrl("https://docs.thundercore.com"));
}

void MainWindow::closeEvent(QCloseEvent* event) {
    LOG_INFO("MainWindow closing");
    
    // Guardar configuración
    Settings::instance().save();
    
    // Cerrar base de datos
    DatabaseManager::instance().disconnect();
    
    QMainWindow::closeEvent(event);
}