// Logger.cpp
#include "logger.h"
#include <QDir>
#include <QCoreApplication>
QMutex Logger::logMutex;
Logger::Logger() {
    // 获取应用执行目录
    QString customLogDir = "C:/Users/55047/Desktop/myproject/CloudLinker/logs";

    // 创建日志文件名（包含日期）
    QString fileName = QDateTime::currentDateTime()
                           .toString("yyyyMMdd") + "_applog.txt";

    // 设置完整文件路径
    QString filePath = customLogDir + "/server/" + fileName;

    // 确保日志目录存在
    QDir().mkpath(customLogDir + "/server");

    // 打开日志文件（追加模式）
    logFile.setFileName(filePath);
    logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
}

Logger::~Logger() {
    if(logFile.isOpen()) {
        logFile.close();
    }
}

void Logger::log(const QString& message, const char* file, int line) {
    QMutexLocker locker(&logMutex);
    if(!Logger::instance().logFile.isOpen())
    {
        return;
    }

    // 提取文件名（去除路径）
    QString fileName = QFileInfo(file).fileName();


    // 格式化时间戳
    QString timestamp = QDateTime::currentDateTime()
                            .toString("[服务端yyyy-MM-dd hh:mm:ss.zzz]");

    // 构造日志行
    QString logLine = QString("%1 [%2:%3] %4\n")
                          .arg(timestamp)
                          .arg(fileName)
                          .arg(line)
                          .arg(message);

    // 写入文件
    QTextStream stream(&Logger::instance().logFile);
    stream << logLine;
    stream.flush();  // 立即刷新缓冲区
}
