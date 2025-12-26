#include "SocialModule.h"
#include "core/Logger.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QListWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>

SocialModule::SocialModule(QObject* parent) 
    : QObject(parent)
    , m_webSocket(nullptr) {
}

SocialModule::~SocialModule() {
    shutdown();
}

QString SocialModule::getName() const { 
    return "SocialModule"; 
}

QString SocialModule::getVersion() const { 
    return "1.0.0"; 
}

QString SocialModule::getDescription() const { 
    return "Sistema de mensajería y amigos"; 
}

QJsonObject SocialModule::getConfig() const {
    QJsonObject config;
    config["enable_friends"] = true;
    config["enable_messages"] = true;
    config["enable_notifications"] = true;
    return config;
}

QWidget* SocialModule::createWidget(QWidget* parent) {
    QWidget* widget = new QWidget(parent);
    QHBoxLayout* mainLayout = new QHBoxLayout(widget);
    
    // Splitter para dividir la ventana
    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    
    // Panel izquierdo: Lista de amigos
    QWidget* friendsPanel = new QWidget();
    QVBoxLayout* friendsLayout = new QVBoxLayout(friendsPanel);
    
    QLabel* friendsLabel = new QLabel("👥 Amigos");
    friendsLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    
    m_friendsList = new QListWidget();
    m_friendsList->setStyleSheet(
        "QListWidget { border: 1px solid #2a3f6f; border-radius: 4px; }"
        "QListWidget::item { padding: 8px; border-bottom: 1px solid #1a2f57; }"
        "QListWidget::item:selected { background-color: rgba(0, 174, 255, 0.3); }"
    );
    
    QHBoxLayout* friendActionsLayout = new QHBoxLayout();
    QPushButton* addFriendButton = new QPushButton("➕ Añadir");
    QPushButton* removeFriendButton = new QPushButton("➖ Eliminar");
    
    friendActionsLayout->addWidget(addFriendButton);
    friendActionsLayout->addWidget(removeFriendButton);
    friendActionsLayout->addStretch();
    
    friendsLayout->addWidget(friendsLabel);
    friendsLayout->addWidget(m_friendsList);
    friendsLayout->addLayout(friendActionsLayout);
    
    // Panel derecho: Mensajería
    QWidget* chatPanel = new QWidget();
    QVBoxLayout* chatLayout = new QVBoxLayout(chatPanel);
    
    QLabel* chatLabel = new QLabel("💬 Chat");
    chatLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    
    m_chatHistory = new QTextEdit();
    m_chatHistory->setReadOnly(true);
    m_chatHistory->setStyleSheet(
        "QTextEdit { border: 1px solid #2a3f6f; border-radius: 4px; "
        "background-color: rgba(15, 25, 45, 0.8); }"
    );
    
    QHBoxLayout* messageLayout = new QHBoxLayout();
    m_messageEdit = new QLineEdit();
    m_messageEdit->setPlaceholderText("Escribe un mensaje...");
    
    QPushButton* sendButton = new QPushButton("📤 Enviar");
    sendButton->setFixedWidth(80);
    
    messageLayout->addWidget(m_messageEdit);
    messageLayout->addWidget(sendButton);
    
    chatLayout->addWidget(chatLabel);
    chatLayout->addWidget(m_chatHistory);
    chatLayout->addLayout(messageLayout);
    
    // Añadir paneles al splitter
    splitter->addWidget(friendsPanel);
    splitter->addWidget(chatPanel);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);
    
    mainLayout->addWidget(splitter);
    
    // Conectar señales
    connect(addFriendButton, &QPushButton::clicked, this, &SocialModule::showAddFriendDialog);
    connect(removeFriendButton, &QPushButton::clicked, this, &SocialModule::removeSelectedFriend);
    connect(sendButton, &QPushButton::clicked, this, &SocialModule::sendMessage);
    connect(m_messageEdit, &QLineEdit::returnPressed, this, &SocialModule::sendMessage);
    
    return widget;
}

bool SocialModule::initialize() {
    LOG_INFO("SocialModule initialized");
    
    // Inicializar WebSocket para mensajería en tiempo real
    m_webSocket = new QWebSocket();
    connect(m_webSocket, &QWebSocket::connected, this, &SocialModule::onWebSocketConnected);
    connect(m_webSocket, &QWebSocket::disconnected, this, &SocialModule::onWebSocketDisconnected);
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &SocialModule::onMessageReceived);
    
    return true;
}

void SocialModule::shutdown() {
    if (m_webSocket) {
        m_webSocket->close();
        m_webSocket->deleteLater();
        m_webSocket = nullptr;
    }
    
    LOG_INFO("SocialModule shutdown");
}

void SocialModule::update() {
    // Actualizar estado de amigos
}

void SocialModule::onWebSocketConnected() {
    LOG_INFO("WebSocket connected for social module");
    m_chatHistory->append("<span style='color: #00ff88;'>✅ Conectado al chat</span>");
}

void SocialModule::onWebSocketDisconnected() {
    LOG_INFO("WebSocket disconnected");
    m_chatHistory->append("<span style='color: #ff5555;'>❌ Desconectado del chat</span>");
}

void SocialModule::onMessageReceived(const QString& message) {
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (doc.isObject()) {
        QJsonObject obj = doc.object();
        QString type = obj["type"].toString();
        QString content = obj["content"].toString();
        QString sender = obj["sender"].toString();
        
        m_chatHistory->append(QString("<b>%1:</b> %2").arg(sender).arg(content));
    }
}

void SocialModule::showAddFriendDialog() {
    // Implementar diálogo para añadir amigo
    LOG_INFO("Showing add friend dialog");
}

void SocialModule::removeSelectedFriend() {
    QListWidgetItem* item = m_friendsList->currentItem();
    if (item) {
        QString friendName = item->text();
        LOG_INFO("Removing friend: " + friendName);
        delete item;
    }
}

void SocialModule::sendMessage() {
    QString message = m_messageEdit->text().trimmed();
    if (!message.isEmpty()) {
        m_chatHistory->append(QString("<b>Tú:</b> %1").arg(message));
        m_messageEdit->clear();
        
        // Enviar a través de WebSocket
        if (m_webSocket && m_webSocket->state() == QAbstractSocket::ConnectedState) {
            QJsonObject msgObj;
            msgObj["type"] = "message";
            msgObj["content"] = message;
            msgObj["timestamp"] = QDateTime::currentDateTime().toString();
            
            QJsonDocument doc(msgObj);
            m_webSocket->sendTextMessage(doc.toJson());
        }
    }
}