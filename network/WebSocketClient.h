#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QWebSocket>
#include <QTimer>

class WebSocketClient : public QObject {
    Q_OBJECT
    
public:
    explicit WebSocketClient(QObject* parent = nullptr);
    ~WebSocketClient();
    
    void connectToServer(const QString& url);
    void disconnectFromServer();
    void sendMessage(const QJsonObject& message);
    
    bool isConnected() const { return m_connected; }
    
signals:
    void connected();
    void disconnected();
    void messageReceived(const QJsonObject& message);
    void errorOccurred(const QString& error);
    
private slots:
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(const QString& message);
    void onError(QAbstractSocket::SocketError error);
    void reconnect();
    
private:
    QWebSocket* m_webSocket;
    QTimer* m_reconnectTimer;
    QString m_serverUrl;
    bool m_connected;
    int m_reconnectAttempts;
};

#endif // WEBSOCKETCLIENT_H