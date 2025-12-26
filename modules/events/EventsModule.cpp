#include "EventsModule.h"
#include "core/Logger.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCalendarWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QDateTimeEdit>

EventsModule::EventsModule(QObject* parent) 
    : QObject(parent)
    , m_calendar(nullptr) {
}

EventsModule::~EventsModule() {
    shutdown();
}

QString EventsModule::getName() const { 
    return "EventsModule"; 
}

QString EventsModule::getVersion() const { 
    return "1.0.0"; 
}

QString EventsModule::getDescription() const { 
    return "Gestión de eventos y calendario"; 
}

QJsonObject EventsModule::getConfig() const {
    QJsonObject config;
    config["show_calendar"] = true;
    config["auto_refresh"] = true;
    config["refresh_interval"] = 60;
    return config;
}

QWidget* EventsModule::createWidget(QWidget* parent) {
    QWidget* widget = new QWidget(parent);
    QVBoxLayout* mainLayout = new QVBoxLayout(widget);
    
    // Título
    QLabel* titleLabel = new QLabel("📅 Calendario de Eventos");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #00aeff;");
    titleLabel->setAlignment(Qt::AlignCenter);
    
    // Calendario
    m_calendar = new QCalendarWidget();
    m_calendar->setGridVisible(true);
    m_calendar->setStyleSheet(
        "QCalendarWidget { background-color: rgba(20, 30, 50, 0.8); "
        "border: 1px solid #2a3f6f; }"
        "QCalendarWidget QToolButton { color: #aaccff; }"
        "QCalendarWidget QMenu { background-color: rgba(25, 40, 65, 0.95); }"
    );
    
    // Lista de eventos
    QLabel* eventsLabel = new QLabel("📋 Eventos Programados");
    eventsLabel->setStyleSheet("font-weight: bold;");
    
    m_eventsTable = new QTableWidget();
    m_eventsTable->setColumnCount(5);
    m_eventsTable->setHorizontalHeaderLabels({
        "Evento", "Tipo", "Fecha", "Hora", "Acción"
    });
    m_eventsTable->horizontalHeader()->setStretchLastSection(true);
    
    // Detalles del evento
    QLabel* detailsLabel = new QLabel("📝 Detalles del Evento");
    detailsLabel->setStyleSheet("font-weight: bold;");
    
    m_eventDetails = new QTextEdit();
    m_eventDetails->setReadOnly(true);
    m_eventDetails->setMaximumHeight(150);
    m_eventDetails->setStyleSheet(
        "QTextEdit { border: 1px solid #2a3f6f; border-radius: 4px; "
        "background-color: rgba(15, 25, 45, 0.8); }"
    );
    
    // Botones de acción
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* createButton = new QPushButton("➕ Crear Evento");
    QPushButton* joinButton = new QPushButton("🎮 Unirse");
    QPushButton* refreshButton = new QPushButton("🔄 Actualizar");
    
    buttonLayout->addWidget(createButton);
    buttonLayout->addWidget(joinButton);
    buttonLayout->addWidget(refreshButton);
    buttonLayout->addStretch();
    
    // Ensamblar layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(m_calendar);
    mainLayout->addWidget(eventsLabel);
    mainLayout->addWidget(m_eventsTable);
    mainLayout->addWidget(detailsLabel);
    mainLayout->addWidget(m_eventDetails);
    mainLayout->addLayout(buttonLayout);
    
    // Conectar señales
    connect(m_calendar, &QCalendarWidget::clicked, this, &EventsModule::onDateSelected);
    connect(createButton, &QPushButton::clicked, this, &EventsModule::createEvent);
    connect(joinButton, &QPushButton::clicked, this, &EventsModule::joinEvent);
    connect(refreshButton, &QPushButton::clicked, this, &EventsModule::refreshEvents);
    
    // Cargar eventos iniciales
    loadEvents();
    
    return widget;
}

bool EventsModule::initialize() {
    LOG_INFO("EventsModule initialized");
    return true;
}

void EventsModule::shutdown() {
    LOG_INFO("EventsModule shutdown");
}

void EventsModule::update() {
    refreshEvents();
}

void EventsModule::loadEvents() {
    // Limpiar tabla
    m_eventsTable->setRowCount(0);
    
    // Añadir eventos de ejemplo
    addEventToTable("Torneo Arena 3v3", "PvP", QDate::currentDate().addDays(1), QTime(20, 0));
    addEventToTable("Raídas Conjuntas", "PvE", QDate::currentDate().addDays(2), QTime(19, 0));
    addEventToTable("Fiesta del Solsticio", "Social", QDate::currentDate().addDays(3), QTime(18, 0));
    addEventToTable("Duelos 1v1", "PvP", QDate::currentDate().addDays(5), QTime(21, 0));
    addEventToTable("Farmeo Masivo", "PvE", QDate::currentDate().addDays(7), QTime(17, 0));
}

void EventsModule::addEventToTable(const QString& name, const QString& type, const QDate& date, const QTime& time) {
    int row = m_eventsTable->rowCount();
    m_eventsTable->insertRow(row);
    
    // Establecer color según tipo
    QString typeColor;
    if (type == "PvP") typeColor = "#ff5555";
    else if (type == "PvE") typeColor = "#00aeff";
    else typeColor = "#00ff88";
    
    // Nombre
    QTableWidgetItem* nameItem = new QTableWidgetItem(name);
    m_eventsTable->setItem(row, 0, nameItem);
    
    // Tipo
    QTableWidgetItem* typeItem = new QTableWidgetItem(type);
    typeItem->setForeground(QBrush(QColor(typeColor)));
    m_eventsTable->setItem(row, 1, typeItem);
    
    // Fecha
    QTableWidgetItem* dateItem = new QTableWidgetItem(date.toString("dd/MM/yyyy"));
    m_eventsTable->setItem(row, 2, dateItem);
    
    // Hora
    QTableWidgetItem* timeItem = new QTableWidgetItem(time.toString("hh:mm"));
    m_eventsTable->setItem(row, 3, timeItem);
    
    // Botón de acción
    QPushButton* actionButton = new QPushButton("Ver Detalles");
    actionButton->setStyleSheet(
        "QPushButton { background: #2a3f6f; color: #aaccff; "
        "border: 1px solid #1a2f57; padding: 3px; font-size: 11px; }"
        "QPushButton:hover { background: #3a4f7f; }"
    );
    
    m_eventsTable->setCellWidget(row, 4, actionButton);
    
    // Conectar botón
    connect(actionButton, &QPushButton::clicked, this, [this, name, type, date, time]() {
        showEventDetails(name, type, date, time);
    });
}

void EventsModule::onDateSelected(const QDate& date) {
    LOG_INFO("Date selected: " + date.toString("yyyy-MM-dd"));
    
    // Resaltar fecha en el calendario
    QTextCharFormat format;
    format.setBackground(QBrush(QColor(0, 174, 255, 100)));
    m_calendar->setDateTextFormat(date, format);
    
    // Filtrar eventos por fecha
    // (En implementación real, filtrarías de la BD)
}

void EventsModule::createEvent() {
    LOG_INFO("Creating new event...");
    // Implementar diálogo de creación de evento
}

void EventsModule::joinEvent() {
    int row = m_eventsTable->currentRow();
    if (row >= 0) {
        QString eventName = m_eventsTable->item(row, 0)->text();
        LOG_INFO("Joining event: " + eventName);
        // Implementar lógica de unirse a evento
    }
}

void EventsModule::refreshEvents() {
    LOG_INFO("Refreshing events...");
    loadEvents();
}

void EventsModule::showEventDetails(const QString& name, const QString& type, const QDate& date, const QTime& time) {
    QString details = QString(
        "<h3>%1</h3>"
        "<p><b>Tipo:</b> %2</p>"
        "<p><b>Fecha:</b> %3</p>"
        "<p><b>Hora:</b> %4</p>"
        "<p><b>Descripción:</b> Evento programado para la comunidad. "
        "¡No te lo pierdas!</p>"
        "<p><b>Requisitos:</b> Nivel máximo, equipo adecuado.</p>"
        "<p><b>Premios:</b> Honor, objetos, títulos.</p>"
    ).arg(name).arg(type).arg(date.toString("dd/MM/yyyy")).arg(time.toString("hh:mm"));
    
    m_eventDetails->setHtml(details);
}