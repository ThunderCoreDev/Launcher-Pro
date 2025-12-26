#include "AdminModule.h"
#include "core/Logger.h"
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>

AdminModule::AdminModule(QObject* parent) 
    : QObject(parent) {
}

AdminModule::~AdminModule() {
    shutdown();
}

QString AdminModule::getName() const { 
    return "AdminModule"; 
}

QString AdminModule::getVersion() const { 
    return "1.0.0"; 
}

QString AdminModule::getDescription() const { 
    return "Panel de administración del servidor"; 
}

QJsonObject AdminModule::getConfig() const {
    QJsonObject config;
    config["enabled"] = true;
    config["permissions"] = "admin_only";
    return config;
}

QWidget* AdminModule::createWidget(QWidget* parent) {
    QWidget* widget = new QWidget(parent);
    QVBoxLayout* layout = new QVBoxLayout(widget);
    
    // Título
    QLabel* titleLabel = new QLabel("Panel de Administración");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #00aeff;");
    titleLabel->setAlignment(Qt::AlignCenter);
    
    // Pestañas
    QTabWidget* tabWidget = new QTabWidget();
    
    // Pestaña de usuarios
    QWidget* usersTab = createUsersTab();
    tabWidget->addTab(usersTab, "👤 Usuarios");
    
    // Pestaña de tickets
    QWidget* ticketsTab = createTicketsTab();
    tabWidget->addTab(ticketsTab, "🎫 Tickets");
    
    // Pestaña de anuncios
    QWidget* announcementsTab = createAnnouncementsTab();
    tabWidget->addTab(announcementsTab, "📢 Anuncios");
    
    // Pestaña de estadísticas
    QWidget* statsTab = createStatsTab();
    tabWidget->addTab(statsTab, "📊 Estadísticas");
    
    layout->addWidget(titleLabel);
    layout->addWidget(tabWidget);
    
    return widget;
}

bool AdminModule::initialize() {
    LOG_INFO("AdminModule initialized");
    return true;
}

void AdminModule::shutdown() {
    LOG_INFO("AdminModule shutdown");
}

void AdminModule::update() {
    // Actualizar datos del módulo
}

QStringList AdminModule::getDependencies() const { 
    return {"DatabaseModule", "StatsModule"}; 
}

bool AdminModule::isCompatible(const QString& coreVersion) const {
    return coreVersion.startsWith("1.");
}

QWidget* AdminModule::createUsersTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    // Barra de búsqueda
    QHBoxLayout* searchLayout = new QHBoxLayout();
    QLineEdit* searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Buscar usuario...");
    QPushButton* searchButton = new QPushButton("🔍 Buscar");
    
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(searchButton);
    
    // Tabla de usuarios
    m_usersTable = new QTableWidget();
    m_usersTable->setColumnCount(6);
    m_usersTable->setHorizontalHeaderLabels({
        "ID", "Usuario", "Nivel", "Último Login", "Estado", "Acciones"
    });
    m_usersTable->horizontalHeader()->setStretchLastSection(true);
    
    // Botones de acción
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* refreshButton = new QPushButton("🔄 Actualizar");
    QPushButton* banButton = new QPushButton("⛔ Banear");
    QPushButton* unbanButton = new QPushButton("✅ Desbanear");
    QPushButton* messageButton = new QPushButton("✉️ Mensaje");
    
    buttonLayout->addWidget(refreshButton);
    buttonLayout->addWidget(banButton);
    buttonLayout->addWidget(unbanButton);
    buttonLayout->addWidget(messageButton);
    buttonLayout->addStretch();
    
    layout->addLayout(searchLayout);
    layout->addWidget(m_usersTable);
    layout->addLayout(buttonLayout);
    
    // Conectar señales
    connect(searchButton, &QPushButton::clicked, this, &AdminModule::searchUsers);
    connect(refreshButton, &QPushButton::clicked, this, &AdminModule::refreshUsers);
    connect(banButton, &QPushButton::clicked, this, &AdminModule::banSelectedUser);
    connect(unbanButton, &QPushButton::clicked, this, &AdminModule::unbanSelectedUser);
    connect(messageButton, &QPushButton::clicked, this, &AdminModule::messageSelectedUser);
    
    return tab;
}

void AdminModule::searchUsers() {
    LOG_INFO("Searching users...");
    // Implementar búsqueda
}

void AdminModule::refreshUsers() {
    LOG_INFO("Refreshing users list...");
    // Implementar actualización
}

void AdminModule::banSelectedUser() {
    LOG_INFO("Banning selected user...");
    // Implementar ban
}

void AdminModule::unbanSelectedUser() {
    LOG_INFO("Unbanning selected user...");
    // Implementar unban
}

void AdminModule::messageSelectedUser() {
    LOG_INFO("Messaging selected user...");
    // Implementar mensaje
}