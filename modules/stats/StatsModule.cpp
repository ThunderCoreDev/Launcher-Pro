#include "StatsModule.h"
#include "core/Logger.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QTimer>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>

using namespace QtCharts;

StatsModule::StatsModule(QObject* parent) 
    : QObject(parent)
    , m_updateTimer(nullptr) {
}

StatsModule::~StatsModule() {
    shutdown();
}

QString StatsModule::getName() const { 
    return "StatsModule"; 
}

QString StatsModule::getVersion() const { 
    return "1.0.0"; 
}

QString StatsModule::getDescription() const { 
    return "Estadísticas del servidor en tiempo real"; 
}

QJsonObject StatsModule::getConfig() const {
    QJsonObject config;
    config["refresh_interval"] = 30;
    config["show_online_chart"] = true;
    config["show_faction_chart"] = true;
    return config;
}

QWidget* StatsModule::createWidget(QWidget* parent) {
    QWidget* widget = new QWidget(parent);
    QVBoxLayout* mainLayout = new QVBoxLayout(widget);
    
    // Título
    QLabel* titleLabel = new QLabel("📊 Estadísticas del Servidor");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #00aeff;");
    titleLabel->setAlignment(Qt::AlignCenter);
    
    // Gráficos
    QHBoxLayout* chartsLayout = new QHBoxLayout();
    
    // Gráfico de jugadores online
    m_onlineChart = new QChart();
    m_onlineChart->setTitle("Jugadores Online");
    m_onlineChart->setTheme(QChart::ChartThemeDark);
    
    QChartView* onlineChartView = new QChartView(m_onlineChart);
    onlineChartView->setRenderHint(QPainter::Antialiasing);
    onlineChartView->setMinimumHeight(300);
    
    // Gráfico de distribución de facciones
    m_factionChart = new QChart();
    m_factionChart->setTitle("Distribución por Facción");
    m_factionChart->setTheme(QChart::ChartThemeDark);
    
    QChartView* factionChartView = new QChartView(m_factionChart);
    factionChartView->setRenderHint(QPainter::Antialiasing);
    factionChartView->setMinimumHeight(300);
    
    chartsLayout->addWidget(onlineChartView);
    chartsLayout->addWidget(factionChartView);
    
    // Estadísticas numéricas
    QGroupBox* statsBox = new QGroupBox("Estadísticas en Tiempo Real");
    statsBox->setStyleSheet(
        "QGroupBox { border: 2px solid #2a3f6f; border-radius: 5px; "
        "margin-top: 10px; font-weight: bold; color: #00aeff; }"
    );
    
    QGridLayout* statsGrid = new QGridLayout();
    
    // Crear labels para estadísticas
    m_onlinePlayersLabel = new QLabel("0");
    m_onlinePlayersLabel->setStyleSheet("font-size: 24px; color: #00ff88; font-weight: bold;");
    
    m_allianceLabel = new QLabel("0");
    m_allianceLabel->setStyleSheet("font-size: 20px; color: #0070dd; font-weight: bold;");
    
    m_hordeLabel = new QLabel("0");
    m_hordeLabel->setStyleSheet("font-size: 20px; color: #c41f3b; font-weight: bold;");
    
    m_uptimeLabel = new QLabel("0h");
    m_uptimeLabel->setStyleSheet("font-size: 20px; color: #ffcc00; font-weight: bold;");
    
    statsGrid->addWidget(new QLabel("Jugadores Online:"), 0, 0);
    statsGrid->addWidget(m_onlinePlayersLabel, 0, 1);
    statsGrid->addWidget(new QLabel("Alianza:"), 1, 0);
    statsGrid->addWidget(m_allianceLabel, 1, 1);
    statsGrid->addWidget(new QLabel("Horda:"), 2, 0);
    statsGrid->addWidget(m_hordeLabel, 2, 1);
    statsGrid->addWidget(new QLabel("Uptime:"), 3, 0);
    statsGrid->addWidget(m_uptimeLabel, 3, 1);
    
    statsBox->setLayout(statsGrid);
    
    // Controles
    QHBoxLayout* controlsLayout = new QHBoxLayout();
    QPushButton* refreshButton = new QPushButton("🔄 Actualizar");
    QPushButton* exportButton = new QPushButton("📤 Exportar");
    
    controlsLayout->addWidget(refreshButton);
    controlsLayout->addWidget(exportButton);
    controlsLayout->addStretch();
    
    // Ensamblar layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(chartsLayout);
    mainLayout->addWidget(statsBox);
    mainLayout->addLayout(controlsLayout);
    
    // Conectar señales
    connect(refreshButton, &QPushButton::clicked, this, &StatsModule::refreshStats);
    connect(exportButton, &QPushButton::clicked, this, &StatsModule::exportStats);
    
    // Crear series para gráficos
    m_onlineSeries = new QLineSeries();
    m_onlineSeries->setName("Jugadores Online");
    m_onlineChart->addSeries(m_onlineSeries);
    m_onlineChart->createDefaultAxes();
    
    // Actualizar inicialmente
    refreshStats();
    
    return widget;
}

bool StatsModule::initialize() {
    LOG_INFO("StatsModule initialized");
    
    // Configurar timer para actualizaciones automáticas
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &StatsModule::refreshStats);
    m_updateTimer->start(30000); // 30 segundos
    
    return true;
}

void StatsModule::shutdown() {
    if (m_updateTimer) {
        m_updateTimer->stop();
        delete m_updateTimer;
        m_updateTimer = nullptr;
    }
    
    LOG_INFO("StatsModule shutdown");
}

void StatsModule::update() {
    refreshStats();
}

void StatsModule::refreshStats() {
    LOG_INFO("Refreshing server statistics...");
    
    // Simular datos (en implementación real, consultarías la BD)
    static int counter = 0;
    counter++;
    
    int onlinePlayers = 150 + (qrand() % 50);
    int alliancePlayers = onlinePlayers * 0.55;
    int hordePlayers = onlinePlayers - alliancePlayers;
    int uptimeHours = 720 + counter;
    
    // Actualizar labels
    m_onlinePlayersLabel->setText(QString::number(onlinePlayers));
    m_allianceLabel->setText(QString::number(alliancePlayers));
    m_hordeLabel->setText(QString::number(hordePlayers));
    m_uptimeLabel->setText(QString::number(uptimeHours) + "h");
    
    // Actualizar gráfico
    QDateTime now = QDateTime::currentDateTime();
    m_onlineSeries->append(now.toMSecsSinceEpoch(), onlinePlayers);
    
    // Mantener solo los últimos 30 puntos
    if (m_onlineSeries->count() > 30) {
        m_onlineSeries->remove(0);
    }
    
    m_onlineChart->axisX()->setRange(
        now.addSecs(-1800).toMSecsSinceEpoch(),
        now.toMSecsSinceEpoch()
    );
    
    m_onlineChart->axisY()->setRange(0, onlinePlayers + 50);
}

void StatsModule::exportStats() {
    LOG_INFO("Exporting statistics...");
    // Implementar exportación a CSV/JSON
}