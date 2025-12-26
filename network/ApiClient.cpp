#include "ApiClient.h"
#include "core/Logger.h"
#include <QUrlQuery>
#include <QJsonDocument>

ApiClient::ApiClient(QObject* parent) 
    : QObject(parent)
    , m_manager(new QNetworkAccessManager(this)) {
    
    connect(m_manager, &QNetworkAccessManager::finished,
            this, &ApiClient::onRequestFinished);
}

ApiClient::~ApiClient() {
    m_manager->deleteLater();
}

void ApiClient::get(const QString& endpoint, const QJsonObject& params) {
    QUrl url(m_baseUrl + endpoint);
    
    if (!params.isEmpty()) {
        QUrlQuery query;
        for (auto it = params.begin(); it != params.end(); ++it) {
            query.addQueryItem(it.key(), it.value().toString());
        }
        url.setQuery(query);
    }
    
    QNetworkRequest request(url);
    if (!m_apiKey.isEmpty()) {
        request.setRawHeader("Authorization", "Bearer " + m_apiKey.toUtf8());
    }
    
    m_manager->get(request);
    LOG_DEBUG("API GET: " + url.toString());
}

void ApiClient::post(const QString& endpoint, const QJsonObject& data) {
    QNetworkRequest request = createRequest(endpoint);
    
    QJsonDocument doc(data);
    QByteArray jsonData = doc.toJson();
    
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, jsonData.size());
    
    m_manager->post(request, jsonData);
    LOG_DEBUG("API POST: " + m_baseUrl + endpoint);
}

void ApiClient::onRequestFinished(QNetworkReply* reply) {
    if (!reply) return;
    
    reply->deleteLater();
    
    if (reply->error() == QNetworkReply::NoError) {
        QJsonObject response = parseResponse(reply);
        emit responseReceived(response);
    } else {
        QString error = reply->errorString();
        LOG_ERROR("API request failed: " + error);
        emit errorOccurred(error);
        emit networkError(reply->error());
    }
}

QNetworkRequest ApiClient::createRequest(const QString& endpoint) {
    QUrl url(m_baseUrl + endpoint);
    QNetworkRequest request(url);
    
    request.setHeader(QNetworkRequest::UserAgentHeader, "ThunderNet Launcher/1.0");
    request.setRawHeader("Accept", "application/json");
    
    if (!m_apiKey.isEmpty()) {
        request.setRawHeader("Authorization", "Bearer " + m_apiKey.toUtf8());
    }
    
    return request;
}

QJsonObject ApiClient::parseResponse(QNetworkReply* reply) {
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (doc.isNull()) {
        LOG_WARNING("Invalid JSON response");
        return QJsonObject();
    }
    
    return doc.object();
}