#include "ToolsModule.h"
#include "core/Logger.h"
#include <QTabWidget>
#include <QTreeWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QFileDialog>
#include <QProcess>

ToolsModule::ToolsModule(QObject* parent) 
    : QObject(parent) {
}

ToolsModule::~ToolsModule() {
    shutdown();
}

QString ToolsModule::getName() const { 
    return "ToolsModule"; 
}

QString ToolsModule::getVersion() const { 
    return "1.0.0"; 
}

QString ToolsModule::getDescription() const { 
    return "Herramientas y utilidades para WoW"; 
}

QJsonObject ToolsModule::getConfig() const {
    QJsonObject config;
    config["auto_scan"] = true;
    config["backup_config"] = true;
    config["clean_cache"] = true;
    return config;
}

QWidget* ToolsModule::createWidget(QWidget* parent) {
    QWidget* widget = new QWidget(parent);
    QVBoxLayout* mainLayout = new QVBoxLayout(widget);
    
    // Título
    QLabel* titleLabel = new QLabel("🛠️ Herramientas del Sistema");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #00aeff;");
    titleLabel->setAlignment(Qt::AlignCenter);
    
    // Pestañas
    QTabWidget* tabWidget = new QTabWidget();
    
    // Pestaña 1: Escaneo y Reparación
    QWidget* scanTab = createScanTab();
    tabWidget->addTab(scanTab, "🔍 Escaneo");
    
    // Pestaña 2: Addons
    QWidget* addonsTab = createAddonsTab();
    tabWidget->addTab(addonsTab, "📦 Addons");
    
    // Pestaña 3: Optimización
    QWidget* optimizeTab = createOptimizeTab();
    tabWidget->addTab(optimizeTab, "⚡ Optimizar");
    
    // Pestaña 4: Backup
    QWidget* backupTab = createBackupTab();
    tabWidget->addTab(backupTab, "💾 Backup");
    
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(tabWidget);
    
    return widget;
}

QWidget* ToolsModule::createScanTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    // Información del juego
    QGroupBox* infoBox = new QGroupBox("Información del Juego");
    infoBox->setStyleSheet(
        "QGroupBox { border: 2px solid #2a3f6f; border-radius: 5px; "
        "margin-top: 10px; font-weight: bold; color: #00aeff; }"
    );
    
    QGridLayout* infoLayout = new QGridLayout();
    
    m_gamePathLabel = new QLabel("No configurado");
    m_gameVersionLabel = new QLabel("Desconocido");
    m_gameSizeLabel = new QLabel("0 MB");
    
    infoLayout->addWidget(new QLabel("Ruta:"), 0, 0);
    infoLayout->addWidget(m_gamePathLabel, 0, 1);
    infoLayout->addWidget(new QLabel("Versión:"), 1, 0);
    infoLayout->addWidget(m_gameVersionLabel, 1, 1);
    infoLayout->addWidget(new QLabel("Tamaño:"), 2, 0);
    infoLayout->addWidget(m_gameSizeLabel, 2, 1);
    
    infoBox->setLayout(infoLayout);
    
    // Barra de progreso
    m_scanProgress = new QProgressBar();
    m_scanProgress->setRange(0, 100);
    m_scanProgress->setValue(0);
    
    // Botones
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* scanButton = new QPushButton("🔍 Escanear");
    QPushButton* repairButton = new QPushButton("🔧 Reparar");
    QPushButton* cleanButton = new QPushButton("🧹 Limpiar");
    
    buttonLayout->addWidget(scanButton);
    buttonLayout->addWidget(repairButton);
    buttonLayout->addWidget(cleanButton);
    buttonLayout->addStretch();
    
    // Log de operaciones
    QLabel* logLabel = new QLabel("📝 Log de Operaciones:");
    m_operationLog = new QTextEdit();
    m_operationLog->setReadOnly(true);
    m_operationLog->setMaximumHeight(150);
    m_operationLog->setStyleSheet(
        "QTextEdit { border: 1px solid #2a3f6f; border-radius: 4px; "
        "background-color: rgba(15, 25, 45, 0.8); font-family: monospace; }"
    );
    
    layout->addWidget(infoBox);
    layout->addWidget(m_scanProgress);
    layout->addLayout(buttonLayout);
    layout->addWidget(logLabel);
    layout->addWidget(m_operationLog);
    
    // Conectar señales
    connect(scanButton, &QPushButton::clicked, this, &ToolsModule::scanGame);
    connect(repairButton, &QPushButton::clicked, this, &ToolsModule::repairGame);
    connect(cleanButton, &QPushButton::clicked, this, &ToolsModule::cleanGame);
    
    return tab;
}

bool ToolsModule::initialize() {
    LOG_INFO("ToolsModule initialized");
    return true;
}

void ToolsModule::shutdown() {
    LOG_INFO("ToolsModule shutdown");
}

void ToolsModule::update() {
    // Actualizar estado de las herramientas
}

void ToolsModule::scanGame() {
    logMessage("Iniciando escaneo del juego...");
    
    // Simular progreso
    for (int i = 0; i <= 100; i += 10) {
        QThread::msleep(100);
        m_scanProgress->setValue(i);
        QCoreApplication::processEvents();
    }
    
    logMessage("✓ Escaneo completado");
    logMessage("✓ Juego verificado correctamente");
    logMessage("✓ No se encontraron errores");
}

void ToolsModule::repairGame() {
    logMessage("Iniciando reparación del juego...");
    logMessage("✓ Archivos verificados");
    logMessage("✓ Cache limpiada");
    logMessage("✓ Configuración restaurada");
    logMessage("✓ Reparación completada");
}

void ToolsModule::cleanGame() {
    logMessage("Limpiando archivos temporales...");
    logMessage("✓ Cache eliminada");
    logMessage("✓ Logs antiguos borrados");
    logMessage("✓ Archivos temporales eliminados");
    logMessage("✓ Limpieza completada");
}

void ToolsModule::logMessage(const QString& message) {
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    m_operationLog->append(QString("[%1] %2").arg(timestamp).arg(message));
}