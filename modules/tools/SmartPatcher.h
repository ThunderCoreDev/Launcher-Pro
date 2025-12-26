#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QCryptographicHash>
#include <QThread>
#include <QQueue>

struct PatchFile {
    QString filename;
    QString hash;
    qint64 size;
    QString url;
    QString action; // "add", "update", "remove"
    QString version;
};

class SmartPatcher : public QThread {
    Q_OBJECT
public:
    explicit SmartPatcher(QObject* parent = nullptr);
    
    void setGamePath(const QString& path);
    void setExpansion(Expansion exp);
    
    void checkForUpdates();
    void downloadUpdates();
    void verifyIntegrity();
    void repairFiles();
    void cleanCache();
    
signals:
    void progressChanged(int percent, const QString& message);
    void statusChanged(const QString& status);
    void updateAvailable(bool available, const QString& version);
    void downloadComplete();
    void errorOccurred(const QString& error);
    
protected:
    void run() override;
    
private slots:
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onDownloadFinished();
    
private:
    QString gamePath;
    Expansion expansion;
    QNetworkAccessManager* networkManager;
    QQueue<PatchFile> downloadQueue;
    QFile* currentFile;
    QNetworkReply* currentReply;
    
    QList<PatchFile> getPatchManifest();
    bool validateFile(const QString& filePath, const QString& expectedHash);
    void downloadNextFile();
    void applyPatch(const PatchFile& file);
    void backupFile(const QString& filePath);
    void createRestorePoint();
    QString calculateFileHash(const QString& filePath);
    
    // Lista blanca de addons
    QStringList allowedAddons = {
        "DeadlyBossMods", "Details", "WeakAuras",
        "ElvUI", "Bartender4", "Recount", "Omen"
    };
};