#include "Logger.h"
#include <QDateTime>
#include <QTextStream>
#include <iostream>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

Logger::Logger() 
    : m_level(Level::Info)
    , m_consoleOutput(true)
    , m_fileOutput(true)
    , m_maxFileSize(10 * 1024 * 1024) {
}

Logger::~Logger() {
    shutdown();
}

void Logger::initialize(const QString& logFile) {
    QMutexLocker locker(&m_mutex);
    
    if (m_fileOutput && !logFile.isEmpty()) {
        m_logFile.setFileName(logFile);
        if (m_logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            LOG_INFO("Log file opened: " + logFile);
        } else {
            std::cerr << "Failed to open log file: " << logFile.toStdString() << std::endl;
            m_fileOutput = false;
        }
    }
}

void Logger::log(Level level, const QString& message, const char* file, int line) {
    if (level < m_level) return;
    
    QString formatted = formatMessage(level, message, file, line);
    
    QMutexLocker locker(&m_mutex);
    
    if (m_consoleOutput) {
        writeToConsole(formatted, level);
    }
    
    if (m_fileOutput && m_logFile.isOpen()) {
        writeToFile(formatted);
    }
}

void Logger::writeToFile(const QString& message) {
    QTextStream stream(&m_logFile);
    stream << message << "\n";
    stream.flush();
}

void Logger::writeToConsole(const QString& message, Level level) {
#ifdef Q_OS_WIN
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    
    switch (level) {
        case Level::Error:
        case Level::Critical:
            color = FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case Level::Warning:
            color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case Level::Info:
            color = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case Level::Debug:
            color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case Level::Trace:
            color = FOREGROUND_BLUE | FOREGROUND_GREEN;
            break;
        default:
            break;
    }
    
    SetConsoleTextAttribute(hConsole, color);
    std::cout << message.toStdString() << std::endl;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    // Linux/Mac
    const char* colorCode = "";
    const char* resetCode = "\033[0m";
    
    switch (level) {
        case Level::Error:
        case Level::Critical:
            colorCode = "\033[31m"; // Red
            break;
        case Level::Warning:
            colorCode = "\033[33m"; // Yellow
            break;
        case Level::Info:
            colorCode = "\033[32m"; // Green
            break;
        case Level::Debug:
            colorCode = "\033[34m"; // Blue
            break;
        case Level::Trace:
            colorCode = "\033[36m"; // Cyan
            break;
        default:
            colorCode = "";
    }
    
    std::cout << colorCode << message.toStdString() << resetCode << std::endl;
#endif
}

QString Logger::levelToString(Level level) const {
    switch (level) {
        case Level::Trace:    return "TRACE";
        case Level::Debug:    return "DEBUG";
        case Level::Info:     return "INFO ";
        case Level::Warning:  return "WARN ";
        case Level::Error:    return "ERROR";
        case Level::Critical: return "CRIT ";
        default:              return "UNKNOWN";
    }
}

QString Logger::formatMessage(Level level, const QString& message, const char* file, int line) {
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    QString levelStr = levelToString(level);
    QString formatted = QString("[%1] [%2] %3").arg(timestamp).arg(levelStr).arg(message);
    
    if (file && line > 0) {
        QString fileName = QFileInfo(file).fileName();
        formatted += QString(" [%1:%2]").arg(fileName).arg(line);
    }
    
    return formatted;
}

// Métodos estáticos
void Logger::trace(const QString& message, const char* file, int line) {
    instance().log(Level::Trace, message, file, line);
}

void Logger::info(const QString& message, const char* file, int line) {
    instance().log(Level::Info, message, file, line);
}

void Logger::error(const QString& message, const char* file, int line) {
    instance().log(Level::Error, message, file, line);
}