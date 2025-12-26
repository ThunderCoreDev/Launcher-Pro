#pragma once
#include "ModuleInterface.h"
#include <QWidget>
#include <QCalendarWidget>
#include <QTableWidget>
#include <QTimer>

class EventsModule : public QObject, public ModuleInterface {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)
    Q_PLUGIN_METADATA(IID "com.thundercore.ModuleInterface.EventsModule")
    
public:
    EventsModule();
    ~EventsModule();
    
    QString getName() const override { return "EventsModule"; }
    QString getVersion() const override { return "1.0.0"; }
    QString getDescription() const override { return "Gestión de eventos y calendario"; }
    QJsonObject getConfig() const override;
    QWidget* createWidget(QWidget* parent = nullptr) override;
    
    bool initialize() override;
    void shutdown() override;
    void update() override;
    
    QStringList getDependencies() const override { return {"DatabaseModule", "SocialModule"}; }
    bool isCompatible(const QString& coreVersion) const override;
    
    // Estructuras
    enum class EventType {
        PvPTournament,
        RaidEvent,
        DungeonEvent,
        SocialEvent,
        HolidayEvent,
        CustomEvent
    };
    
    enum class EventStatus {
        Scheduled,
        RegistrationOpen,
        InProgress,
        Completed,
        Cancelled
    };
    
    struct Event {
        quint32 id;
        QString title;
        QString description;
        EventType type;
        EventStatus status;
        QDateTime startTime;
        QDateTime endTime;
        QDateTime registrationStart;
        QDateTime registrationEnd;
        int maxParticipants;
        int currentParticipants;
        QString requirements;
        QString rewards;
        QString organizer;
        quint32 organizerId;
        QString location;
        QString rules;
        QJsonObject extraData;
    };
    
    struct Participant {
        quint32 accountId;
        QString username;
        quint64 characterGuid;
        QString characterName;
        QString characterClass;
        int characterLevel;
        QDateTime registeredAt;
        QString status; // "registered", "checked_in", "participated", "won"
    };
    
    // Funciones principales
    void loadEvents(int days = 30);
    void createEvent(const Event& event);
    void updateEvent(quint32 eventId, const Event& event);
    void deleteEvent(quint32 eventId);
    void registerForEvent(quint32 eventId, quint64 characterGuid);
    void unregisterFromEvent(quint32 eventId);
    void startEvent(quint32 eventId);
    void endEvent(quint32 eventId, const QJsonObject& results);
    
    QList<Event> getUpcomingEvents() const;
    QList<Event> getOngoingEvents() const;
    QList<Participant> getEventParticipants(quint32 eventId) const;
    
signals:
    void eventCreated(const Event& event);
    eventUpdated(quint32 eventId, const Event& event);
    eventDeleted(quint32 eventId);
    eventStartingSoon(const Event& event, int minutesLeft);
    eventStarted(quint32 eventId);
    eventEnded(quint32 eventId, const QJsonObject& results);
    participantRegistered(quint32 eventId, const Participant& participant);
    participantUnregistered(quint32 eventId, quint32 accountId);
    
private slots:
    void onDateSelected(const QDate& date);
    void onEventSelected(QTableWidgetItem* item);
    void onCreateEventClicked();
    void onEditEventClicked();
    void onDeleteEventClicked();
    void onRegisterClicked();
    void onUnregisterClicked();
    void onStartEventClicked();
    void onEndEventClicked();
    void onRefreshClicked();
    void onExportCalendar();
    
    void checkEventReminders();
    void updateCountdowns();
    
private:
    void setupUI();
    void setupCalendar();
    void loadEventsForDate(const QDate& date);
    void updateEventsTable();
    void updateParticipantsTable(quint32 eventId);
    void sendEventNotifications(const Event& event, const QString& type);
    void processEventRewards(quint32 eventId, const QJsonObject& results);
    
    // Widgets
    QCalendarWidget* m_calendar;
    QTableWidget* m_eventsTable;
    QTableWidget* m_participantsTable;
    QTextEdit* m_eventDetails;
    QLabel* m_countdownLabel;
    
    // Controles
    QPushButton* m_createButton;
    QPushButton* m_editButton;
    QPushButton* m_deleteButton;
    QPushButton* m_registerButton;
    QPushButton* m_unregisterButton;
    QPushButton* m_startButton;
    QPushButton* m_endButton;
    QPushButton* m_refreshButton;
    QPushButton* m_exportButton;
    
    // Datos
    QMap<quint32, Event> m_events;
    QMap<quint32, QList<Participant>> m_participants;
    QMap<QDate, QList<quint32>> m_eventsByDate;
    
    quint32 m_currentUserId;
    quint32 m_selectedEventId;
    
    // Temporizadores
    QTimer* m_reminderTimer;
    QTimer* m_countdownTimer;
    
    Q_DISABLE_COPY(EventsModule)
};