#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QVariant>
#include <QVector>

// 日志类型枚举
enum LogType {
    LOG_MOTION = 0,       // 运动检测
    LOG_DETECTION = 1,    // 目标检测
    LOG_ZONE_ALERT = 2,   // 区域入侵
    LOG_LINE_ALERT = 3,   // 越线检测
    LOG_FIRE = 4,         // 火焰检测
    LOG_SMOKE = 5,        // 烟雾检测
    LOG_CROWD = 6,        // 人群聚集
    LOG_OTHER = 99        // 其他
};

// 日志记录结构体
struct LogRecord {
    int id;
    QDateTime timestamp;
    LogType type;
    QString typeName;
    QString description;
    QString objectType;
    float confidence;
    int x;
    int y;
    QString zoneName;
    QString screenshotPath;
    QString videoPath;
    QString cameraId;

    LogRecord() : id(-1), type(LOG_OTHER), confidence(0.0f), x(0), y(0) {}
};

class Database
{
public:
    static Database& instance();

    bool init(const QString& dbPath = "security_logs.db");
    void close();

    bool isOpen() const;
    QString lastError() const;

    // 日志操作
    bool insertLog(LogType type, const QString& description, 
                   const QString& objectType = "", float confidence = 0.0f,
                   int x = 0, int y = 0, const QString& zoneName = "",
                   const QString& screenshotPath = "", const QString& videoPath = "",
                   const QString& cameraId = "");

    QVector<LogRecord> getAllLogs(int limit = 100);
    QVector<LogRecord> getLogsByType(LogType type, int limit = 100);
    QVector<LogRecord> getLogsByDate(const QDateTime& start, const QDateTime& end, int limit = 100);

    bool deleteLog(int id);
    bool clearAllLogs();

    LogRecord getLogById(int id);

    // 日志类型转换
    static QString logTypeToString(LogType type);
    static LogType stringToLogType(const QString& str);

private:
    Database();
    ~Database();
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    bool createTables();

    QSqlDatabase m_db;
    QString m_lastError;
};

#endif // DATABASE_H