#pragma once
#include "ModuleInterface.h"
#include <QWidget>
#include <QWebSocket>
#include <QTreeWidget>
#include <QTextEdit>
#include <QListWidget>

class SocialModule : public QObject, public ModuleInterface {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)
    Q_PLUGIN_METADATA(IID "com.thundercore.ModuleInterface.SocialModule")
    
public:
    SocialModule();
    ~SocialModule();
    
    // ModuleInterface implementation
    QString getName() const override { return "SocialModule"; }
    QString getVersion() const override { return "1.0.0"; }
    QString getDescription() const override { return "Sistema de mensajería y amigos"; }
    QJsonObject getConfig() const override;
    QWidget* createWidget(QWidget* parent = nullptr) override;
    
    bool initialize() override;
    void shutdown() override;
    void update() override;
    
    QStringList getDependencies() const override { return {"DatabaseModule"}; }
    bool isCompatible(const QString& coreVersion) const override;
    
    // Funcionalidades específicas
    struct FriendInfo {
        quint32 accountId;
        QString username;
        QString avatar;
        bool isOnline;
        QString status;
        QString playingCharacter;
        QDateTime lastSeen;
    };
    
    struct Message {
        quint64 id;
        quint32 senderId;
        quint32 receiverId;
        QString subject;
        QString content;
        QDateTime timestamp;
        bool isRead;
        bool isDeleted;
    };
    
    void sendMessage(quint32 receiverId, const QString& subject, const QString& content);
    void addFriend(quint32 accountId, const QString& message = "");
    void removeFriend(quint32 accountId);
    void blockUser(quint32 accountId);
    void setStatus(const QString& status);
    
    QList<FriendInfo> getFriends() const;
    QList<Message> getConversation(quint32 otherUserId, int limit = 50) const;
    int getUnreadMessageCount() const;
    
signals:
    void friendRequestReceived(quint32 fromId, const QString& username, const QString& message);
    void friendStatusChanged(quint32 accountId, bool online, const QString& status);
    void messageReceived(quint32 fromId, const QString& subject, const QString& content);
    void messageSent(quint32 toId, const QString& subject, const QString& content);
    void friendAdded(quint32 accountId);
    void friendRemoved(quint32 accountId);
    
private slots:
    void onWebSocketConnected();
    void onWebSocketDisconnected();
    void onWebSocketError(QAbstractSocket::SocketError error);
    void onWebSocketMessageReceived(const QByteArray& message);
    
    void onMessageSendClicked();
    void onFriendAddClicked();
    void onFriendRemoveClicked();
    void onConversationSelected(QListWidgetItem* item);
    void onFriendSelected(QTreeWidgetItem* item, int column);
    void onStatusChanged(const QString& status);
    
private:
    void setupUI();
    void loadFriends();
    void loadConversations();
    void updatePresence();
    void sendPresenceUpdate();
    void processIncomingMessage(const QJsonObject& message);
    
    QWebSocket* m_webSocket;
    QTimer* m_presenceTimer;
    
    // Widgets
    QTreeWidget* m_friendsTree;
    QListWidget* m_conversationsList;
    QTextEdit* m_messageEdit;
    QLineEdit* m_searchEdit;
    QPushButton* m_sendButton;
    QComboBox* m_statusCombo;
    
    // Datos
    QMap<quint32, FriendInfo> m_friends;
    QMap<quint32, QList<Message>> m_conversations;
    quint32 m_currentUserId;
    QString m_currentUsername;
    QString m_currentStatus;
    
    Q_DISABLE_COPY(SocialModule)
};