#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QDir>
#include <QCoreApplication>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDebug>
#include <sys/file.h>
#include <unistd.h>
#include<QMutexLocker>
#define LOG(msg) Logger::log(msg, __FILE__, __LINE__)
class Logger
{
public:
    // 获取单例实例
    static Logger& instance() {
        static Logger logger;
        return logger;
    }

    // 记录日志的静态方法
    static void log(const QString& message,
                    const char* file = __FILE__,
                    int line = __LINE__);

private:
    Logger();  // 私有构造函数
    ~Logger(); // 析构函数

    // 禁用复制和赋值
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    QFile logFile;
    static QMutex logMutex;
};

#endif // LOGGER_H
