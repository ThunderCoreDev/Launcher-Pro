#ifndef APICLIENT_H
#define APICLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>

class ApiClient : public QObject {
    Q_OBJECT
    
public:
    explicit ApiClient(QObject* parent = nullptr);
    ~ApiClient();
    
    void get(const QString& endpoint, const QJsonObject& params = {});
    void post(const QString& endpoint, const QJsonObject& data);
    void put(const QString& endpoint, const QJsonObject& data);
    void deleteResource(const QString& endpoint);
    
    void setBaseUrl(const QString& url) { m_baseUrl = url; }
    void setApiKey(const QString& key) { m_apiKey = key; }
    
signals:
    void responseReceived(const QJsonObject& response);
    void errorOccurred(const QString& error);
    void networkError(QNetworkReply::NetworkError code);
    
private slots:
    void onRequestFinished(QNetworkReply* reply);
    
private:
    QNetworkAccessManager* m_manager;
    QString m_baseUrl;
    QString m_apiKey;
    
    QNetworkRequest createRequest(const QString& endpoint);
    QJsonObject parseResponse(QNetworkReply* reply);
};

#endif // APICLIENT_H