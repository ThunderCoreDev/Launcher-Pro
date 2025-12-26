#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QObject>
#include <QDateTime>

class Message : public QObject {
    Q_OBJECT
    
    Q_PROPERTY(quint64 id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(quint32 senderId READ senderId WRITE setSenderId NOTIFY senderIdChanged)
    Q_PROPERTY(quint32 receiverId READ receiverId WRITE setReceiverId NOTIFY receiverIdChanged)
    Q_PROPERTY(QString subject READ subject WRITE setSubject NOTIFY subjectChanged)
    Q_PROPERTY(QString content READ content WRITE setContent NOTIFY contentChanged)
    Q_PROPERTY(bool read READ read WRITE setRead NOTIFY readChanged)
    Q_PROPERTY(QDateTime timestamp READ timestamp WRITE setTimestamp NOTIFY timestampChanged)
    
public:
    explicit Message(QObject* parent = nullptr);
    Message(quint32 senderId, quint32 receiverId, const QString& subject,