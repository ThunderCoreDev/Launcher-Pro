#include "FileUtils.h"
#include "core/Logger.h"
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QCryptographicHash>

FileUtils::FileUtils() {
}

QString FileUtils::getAppDataPath() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    return path;
}

QString FileUtils::getConfigPath() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    return path;
}

QString FileUtils::getCachePath() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    return path;
}

bool FileUtils::validateWoWPath(const QString& path) {
    if (path.isEmpty()) return false;
    
    QDir dir(path);
    if (!dir.exists()) return false;
    
    // Verificar archivos esenciales de WoW
    QStringList essentialFiles = {
        "WoW.exe",
        "Wow.mfil",
        "Wow.tfil",
        "Data/enUS/locale-enUS.MPQ"
    };
    
    for (const QString& file : essentialFiles) {
        if (!QFile::exists(path + "/" + file)) {
            LOG_WARNING("Missing WoW file: " + file);
            return false;
        }
    }
    
    return true;
}

QString FileUtils::getFileHash(const QString& filePath, QCryptographicHash::Algorithm algorithm) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG_ERROR("Cannot open file for hash: " + filePath);
        return QString();
    }
    
    QCryptographicHash hash(algorithm);
    if (!hash.addData(&file)) {
        LOG_ERROR("Failed to compute hash for: " + filePath);
        return QString();
    }
    
    return hash.result().toHex();
}

qint64 FileUtils::getDirectorySize(const QString& path) {
    qint64 size = 0;
    QDir dir(path);
    
    QFileInfoList entries = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo& entry : entries) {
        if (entry.isDir()) {
            size += getDirectorySize(entry.absoluteFilePath());
        } else {
            size += entry.size();
        }
    }
    
    return size;
}

bool FileUtils::cleanDirectory(const QString& path, const QStringList& extensions) {
    QDir dir(path);
    if (!dir.exists()) return false;
    
    int removed = 0;
    QFileInfoList files = dir.entryInfoList(QDir::Files);
    for (const QFileInfo& file : files) {
        if (extensions.isEmpty() || extensions.contains(file.suffix(), Qt::CaseInsensitive)) {
            if (QFile::remove(file.absoluteFilePath())) {
                removed++;
            }
        }
    }
    
    LOG_INFO(QString("Cleaned %1 files from %2").arg(removed).arg(path));
    return removed > 0;
}

QString FileUtils::formatSize(qint64 bytes) {
    const qint64 kb = 1024;
    const qint64 mb = kb * 1024;
    const qint64 gb = mb * 1024;
    
    if (bytes >= gb) {
        return QString("%1 GB").arg(bytes / (double)gb, 0, 'f', 2);
    } else if (bytes >= mb) {
        return QString("%1 MB").arg(bytes / (double)mb, 0, 'f', 2);
    } else if (bytes >= kb) {
        return QString("%1 KB").arg(bytes / (double)kb, 0, 'f', 2);
    } else {
        return QString("%1 bytes").arg(bytes);
    }
}

bool FileUtils::copyDirectory(const QString& source, const QString& destination) {
    QDir sourceDir(source);
    if (!sourceDir.exists()) return false;
    
    QDir destDir(destination);
    if (!destDir.exists()) {
        destDir.mkpath(".");
    }
    
    bool success = true;
    QFileInfoList entries = sourceDir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    
    for (const QFileInfo& entry : entries) {
        QString srcPath = entry.absoluteFilePath();
        QString dstPath = destination + "/" + entry.fileName();
        
        if (entry.isDir()) {
            success = success && copyDirectory(srcPath, dstPath);
        } else {
            success = success && QFile::copy(srcPath, dstPath);
        }
    }
    
    return success;
}