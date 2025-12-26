#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QString>
#include <QCryptographicHash>

class FileUtils {
public:
    static QString getAppDataPath();
    static QString getConfigPath();
    static QString getCachePath();
    
    static bool validateWoWPath(const QString& path);
    static QString getFileHash(const QString& filePath, 
                              QCryptographicHash::Algorithm algorithm = QCryptographicHash::Sha256);
    static qint64 getDirectorySize(const QString& path);
    static bool cleanDirectory(const QString& path, const QStringList& extensions = QStringList());
    static QString formatSize(qint64 bytes);
    static bool copyDirectory(const QString& source, const QString& destination);
    
private:
    FileUtils() = default;
};

#endif // FILEUTILS_H