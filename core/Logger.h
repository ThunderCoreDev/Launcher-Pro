#pragma once
#include <QString>
#include <QFile>
#include <QMutex>
#include <memory>
#include "Singleton.h"

class Logger : public Singleton<Logger> {
public:
    enum class Level {
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Critical,
        Off
    };
    
    ~Logger();
    
    void initialize(const QString& logFile);
    void shutdown();
    
    void setLevel(Level level);
    void setConsoleOutput(bool enable);
    void setFileOutput(bool enable);
    void setMaxFileSize(qint64 size);
    
    void log(Level level, const QString& message, const char* file = nullptr, int line = 0);
    
    // Métodos estáticos de conveniencia
    static void trace(const QString& message, const char* file = nullptr, int line = 0);
    static void debug(const QString& message, const char* file = nullptr, int line = 0);
    static void info(const QString& message, const char* file = nullptr, int line = 0);
    static void warning(const QString& message, const char* file = nullptr, int line = 0);
    static void error(const QString& message, const char* file = nullptr, int line = 0);
    static void critical(const QString& message, const char* file = nullptr, int line = 0);
    
    QString getLogFilePath() const;
    QStringList getRecentLogs(int count = 100) const;
    void clearLogs();
    
private:
    Logger();
    
    void writeToFile(const QString& message);
    void writeToConsole(const QString& message);
    QString levelToString(Level level) const;
    QString formatMessage(Level level, const QString& message, const char* file, int line);
    void rotateLogIfNeeded();
    
    Level m_level;
    bool m_consoleOutput;
    bool m_fileOutput;
    QFile m_logFile;
    qint64 m_maxFileSize;
    mutable QMutex m_mutex;
    
    Q_DISABLE_COPY(Logger)
};

// Macros para logging
#define LOG_TRACE(message) Logger::trace(message, __FILE__, __LINE__)
#define LOG_DEBUG(message) Logger::debug(message, __FILE__, __LINE__)
#define LOG_INFO(message) Logger::info(message, __FILE__, __LINE__)
#define LOG_WARNING(message) Logger::warning(message, __FILE__, __LINE__)
#define LOG_ERROR(message) Logger::error(message, __FILE__, __LINE__)
#define LOG_CRITICAL(message) Logger::critical(message, __FILE__, __LINE__)