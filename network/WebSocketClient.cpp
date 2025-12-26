#include "WebSocketClient.h"
#include "core/Logger.h"
#include <QJsonDocument>

WebSocketClient::WebSocketClient(QObject* parent)
    : QObject(parent)
    , m_webSocket(new QWebSocket())
    , m_reconnectTimer(new QTimer(this))
    , m_connected(false)
    , m_reconnectAttempts(0) {
    
    connect(m_webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::onDisconnected);
    connect(m_webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            this, &WebSocketClient::onError);
    connect(m_webSocket, &QWebSocket::textMessageReceived,
            this, &WebSocketClient::onTextMessageReceived);
    
    m_reconnectTimer->setInterval(5000);
    connect(m_reconnectTimer, &QTimer::timeout, this, &WebSocketClient::reconnect);
}

WebSocketClient::~WebSocketClient() {
    disconnectFromServer();
    m_webSocket->deleteLater();
}

void WebSocketClient::connectToServer(const QString& url) {
    if (m_connected) return;
    
    m_serverUrl = url;
    LOG_INFO("Connecting to WebSocket: " + url);
    m_webSocket->open(QUrl(url));
}

void WebSocketClient::disconnectFromServer() {
    m_reconnectTimer->stop();
    m_webSocket->close();
    m_connected = false;
}

void WebSocketClient::sendMessage(const QJsonObject& message) {
    if (!m_connected) {
        LOG_WARNING("Cannot send message: WebSocket not connected");
        return;
    }
    
    QJsonDocument doc(message);
    QString jsonString = doc.toJson(QJsonDocument::Compact);
    m_webSocket->sendTextMessage(jsonString);
}

void WebSocketClient::onConnected() {
    m_connected = true;
    m_reconnectAttempts = 0;
    m_reconnectTimer->stop();
    
    LOG_INFO("WebSocket connected to: " + m_serverUrl);
    emit connected();
}

void WebSocketClient::onDisconnected() {
    m_connected = false;
    LOG_INFO("WebSocket disconnected");
    
    if (m_reconnectAttempts < 10) {
        m_reconnectTimer->start();
    }
    
    emit disconnected();
}

void WebSocketClient::onTextMessageReceived(const QString& message) {
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (doc.isNull()) {
        LOG_WARNING("Invalid JSON received: " + message);
        return;
    }
    
    emit messageReceived(doc.object());
}

void WebSocketClient::onError(QAbstractSocket::SocketError error) {
    QString errorStr = m_webSocket->errorString();
    LOG_ERROR("WebSocket error: " + errorStr);
    emit errorOccurred(errorStr);
}

void WebSocketClient::reconnect() {
    m_reconnectAttempts++;
    LOG_INFO(QString("Attempting to reconnect (%1/10)...").arg(m_reconnectAttempts));
    m_webSocket->open(QUrl(m_serverUrl));
}