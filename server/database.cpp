#include "database.h"
#include <QDir>
#include <QDebug>

Database::Database()
{
}

Database::~Database()
{
    close();
}

Database& Database::instance()
{
    static Database instance;
    return instance;
}

bool Database::init(const QString& dbPath)
{
    if (m_db.isOpen()) {
        qDebug() << "Database already open";
        return true;
    }

    // 确保数据库所在目录存在
    QFileInfo fileInfo(dbPath);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        qDebug() << "Failed to open database:" << m_lastError;
        return false;
    }

    qDebug() << "Database opened successfully:" << dbPath;

    if (!createTables()) {
        return false;
    }

    return true;
}

void Database::close()
{
    if (m_db.isOpen()) {
        m_db.close();
        qDebug() << "Database closed";
    }
}

bool Database::isOpen() const
{
    return m_db.isOpen();
}

QString Database::lastError() const
{
    return m_lastError;
}

bool Database::createTables()
{
    QSqlQuery query;

    // 创建日志表
    QString createTableSql = R"(
        CREATE TABLE IF NOT EXISTS logs (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp DATETIME NOT NULL,
            type INTEGER NOT NULL,
            type_name TEXT NOT NULL,
            description TEXT,
            object_type TEXT,
            confidence REAL,
            x INTEGER,
            y INTEGER,
            zone_name TEXT,
            screenshot_path TEXT,
            video_path TEXT,
            camera_id TEXT
        )
    )";

    if (!query.exec(createTableSql)) {
        m_lastError = query.lastError().text();
        qDebug() << "Failed to create logs table:" << m_lastError;
        return false;
    }

    // 创建索引
    QString createIndex1 = "CREATE INDEX IF NOT EXISTS idx_logs_timestamp ON logs(timestamp)";
    QString createIndex2 = "CREATE INDEX IF NOT EXISTS idx_logs_type ON logs(type)";

    if (!query.exec(createIndex1) || !query.exec(createIndex2)) {
        m_lastError = query.lastError().text();
        qDebug() << "Failed to create index:" << m_lastError;
        // 索引创建失败不影响使用
    }

    qDebug() << "Tables created successfully";
    return true;
}

bool Database::insertLog(LogType type, const QString& description,
                        const QString& objectType, float confidence,
                        int x, int y, const QString& zoneName,
                        const QString& screenshotPath, const QString& videoPath,
                        const QString& cameraId)
{
    if (!m_db.isOpen()) {
        m_lastError = "Database not open";
        return false;
    }

    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO logs (timestamp, type, type_name, description, 
                          object_type, confidence, x, y, zone_name,
                          screenshot_path, video_path, camera_id)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    )");

    query.addBindValue(QDateTime::currentDateTime());
    query.addBindValue(static_cast<int>(type));
    query.addBindValue(logTypeToString(type));
    query.addBindValue(description);
    query.addBindValue(objectType);
    query.addBindValue(confidence);
    query.addBindValue(x);
    query.addBindValue(y);
    query.addBindValue(zoneName);
    query.addBindValue(screenshotPath);
    query.addBindValue(videoPath);
    query.addBindValue(cameraId);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qDebug() << "Failed to insert log:" << m_lastError;
        return false;
    }

    qDebug() << "Log inserted successfully, ID:" << query.lastInsertId().toInt();
    return true;
}

QVector<LogRecord> Database::getAllLogs(int limit)
{
    QVector<LogRecord> logs;

    if (!m_db.isOpen()) {
        m_lastError = "Database not open";
        return logs;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM logs ORDER BY timestamp DESC LIMIT ?");
    query.addBindValue(limit);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qDebug() << "Failed to get logs:" << m_lastError;
        return logs;
    }

    while (query.next()) {
        LogRecord record;
        record.id = query.value("id").toInt();
        record.timestamp = query.value("timestamp").toDateTime();
        record.type = static_cast<LogType>(query.value("type").toInt());
        record.typeName = query.value("type_name").toString();
        record.description = query.value("description").toString();
        record.objectType = query.value("object_type").toString();
        record.confidence = query.value("confidence").toFloat();
        record.x = query.value("x").toInt();
        record.y = query.value("y").toInt();
        record.zoneName = query.value("zone_name").toString();
        record.screenshotPath = query.value("screenshot_path").toString();
        record.videoPath = query.value("video_path").toString();
        record.cameraId = query.value("camera_id").toString();
        logs.append(record);
    }

    return logs;
}

QVector<LogRecord> Database::getLogsByType(LogType type, int limit)
{
    QVector<LogRecord> logs;

    if (!m_db.isOpen()) {
        m_lastError = "Database not open";
        return logs;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM logs WHERE type = ? ORDER BY timestamp DESC LIMIT ?");
    query.addBindValue(static_cast<int>(type));
    query.addBindValue(limit);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qDebug() << "Failed to get logs by type:" << m_lastError;
        return logs;
    }

    while (query.next()) {
        LogRecord record;
        record.id = query.value("id").toInt();
        record.timestamp = query.value("timestamp").toDateTime();
        record.type = static_cast<LogType>(query.value("type").toInt());
        record.typeName = query.value("type_name").toString();
        record.description = query.value("description").toString();
        record.objectType = query.value("object_type").toString();
        record.confidence = query.value("confidence").toFloat();
        record.x = query.value("x").toInt();
        record.y = query.value("y").toInt();
        record.zoneName = query.value("zone_name").toString();
        record.screenshotPath = query.value("screenshot_path").toString();
        record.videoPath = query.value("video_path").toString();
        record.cameraId = query.value("camera_id").toString();
        logs.append(record);
    }

    return logs;
}

QVector<LogRecord> Database::getLogsByDate(const QDateTime& start, const QDateTime& end, int limit)
{
    QVector<LogRecord> logs;

    if (!m_db.isOpen()) {
        m_lastError = "Database not open";
        return logs;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM logs WHERE timestamp BETWEEN ? AND ? ORDER BY timestamp DESC LIMIT ?");
    query.addBindValue(start);
    query.addBindValue(end);
    query.addBindValue(limit);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qDebug() << "Failed to get logs by date:" << m_lastError;
        return logs;
    }

    while (query.next()) {
        LogRecord record;
        record.id = query.value("id").toInt();
        record.timestamp = query.value("timestamp").toDateTime();
        record.type = static_cast<LogType>(query.value("type").toInt());
        record.typeName = query.value("type_name").toString();
        record.description = query.value("description").toString();
        record.objectType = query.value("object_type").toString();
        record.confidence = query.value("confidence").toFloat();
        record.x = query.value("x").toInt();
        record.y = query.value("y").toInt();
        record.zoneName = query.value("zone_name").toString();
        record.screenshotPath = query.value("screenshot_path").toString();
        record.videoPath = query.value("video_path").toString();
        record.cameraId = query.value("camera_id").toString();
        logs.append(record);
    }

    return logs;
}

bool Database::deleteLog(int id)
{
    if (!m_db.isOpen()) {
        m_lastError = "Database not open";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM logs WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qDebug() << "Failed to delete log:" << m_lastError;
        return false;
    }

    qDebug() << "Log deleted successfully, ID:" << id;
    return true;
}

bool Database::clearAllLogs()
{
    if (!m_db.isOpen()) {
        m_lastError = "Database not open";
        return false;
    }

    QSqlQuery query;
    if (!query.exec("DELETE FROM logs")) {
        m_lastError = query.lastError().text();
        qDebug() << "Failed to clear logs:" << m_lastError;
        return false;
    }

    // 重置自增ID
    query.exec("DELETE FROM sqlite_sequence WHERE name='logs'");

    qDebug() << "All logs cleared";
    return true;
}

LogRecord Database::getLogById(int id)
{
    LogRecord record;

    if (!m_db.isOpen()) {
        m_lastError = "Database not open";
        return record;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM logs WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qDebug() << "Failed to get log by id:" << m_lastError;
        return record;
    }

    if (query.next()) {
        record.id = query.value("id").toInt();
        record.timestamp = query.value("timestamp").toDateTime();
        record.type = static_cast<LogType>(query.value("type").toInt());
        record.typeName = query.value("type_name").toString();
        record.description = query.value("description").toString();
        record.objectType = query.value("object_type").toString();
        record.confidence = query.value("confidence").toFloat();
        record.x = query.value("x").toInt();
        record.y = query.value("y").toInt();
        record.zoneName = query.value("zone_name").toString();
        record.screenshotPath = query.value("screenshot_path").toString();
        record.videoPath = query.value("video_path").toString();
        record.cameraId = query.value("camera_id").toString();
    }

    return record;
}

QString Database::logTypeToString(LogType type)
{
    switch (type) {
        case LOG_MOTION: return "运动检测";
        case LOG_DETECTION: return "目标检测";
        case LOG_ZONE_ALERT: return "区域入侵";
        case LOG_LINE_ALERT: return "越线检测";
        case LOG_FIRE: return "火焰检测";
        case LOG_SMOKE: return "烟雾检测";
        case LOG_CROWD: return "人群聚集";
        case LOG_OTHER:
        default: return "其他";
    }
}

LogType Database::stringToLogType(const QString& str)
{
    if (str == "运动检测") return LOG_MOTION;
    if (str == "目标检测") return LOG_DETECTION;
    if (str == "区域入侵") return LOG_ZONE_ALERT;
    if (str == "越线检测") return LOG_LINE_ALERT;
    if (str == "火焰检测") return LOG_FIRE;
    if (str == "烟雾检测") return LOG_SMOKE;
    if (str == "人群聚集") return LOG_CROWD;
    return LOG_OTHER;
}