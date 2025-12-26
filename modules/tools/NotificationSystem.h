#pragma once
#include <QObject>
#include <QSystemTrayIcon>
#include <QSoundEffect>
#include <QTimer>

class NotificationSystem : public QObject {
    Q_OBJECT
public:
    enum NotificationType {
        Info,
        Warning,
        Error,
        Success,
        FriendRequest,
        PrivateMessage,
        EventReminder,
        RewardAvailable,
        ServerRestart,
        GameInvite
    };
    
    struct Notification {
        QString id;
        QString title;
        QString message;
        NotificationType type;
        QDateTime timestamp;
        QVariant data;
        QString actionUrl;
        bool persistent;
        int duration;
    };
    
    static NotificationSystem& instance();
    
    void showNotification(const Notification& notification);
    void showTrayNotification(const QString& title, const QString& message, 
                              NotificationType type = Info);
    void playSound(NotificationType type);
    void scheduleReminder(const QString& title, const QString& message,
                          const QDateTime& when);
    
    void enablePushNotifications(bool enable);
    void setNotificationDuration(int seconds);
    void setSoundEnabled(bool enabled);
    
signals:
    void notificationClicked(const QString& notificationId, const QVariant& data);
    void actionTriggered(const QString& actionUrl);
    
private:
    NotificationSystem();
    void setupTrayIcon();
    void loadSounds();
    void showPopup(const Notification& notification);
    
    QSystemTrayIcon* trayIcon;
    QMap<NotificationType, QSoundEffect*> sounds;
    QTimer* reminderTimer;
    QList<Notification> scheduledReminders;
    QSettings settings;
};