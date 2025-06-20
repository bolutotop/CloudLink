#include "cloudlinkerclient.h"
#include <QApplication>
#include <QFile>
//加载qss样式表
void LoadQss(QApplication& app);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //加载qss样式表
    LoadQss(a);

    CloudLinkerClient::getInstance().show();
    return a.exec();
}
//加载qss样式表
void LoadQss(QApplication &app) {
    //加载文件，只读方式打开文件，读取内容，设置样式，关闭文件
    QFile qss_file(":/res/style/stylesheet.qss");
    if (qss_file.open(QIODevice::ReadOnly)) {
        qDebug("Open stylesheet.qss success.");
        QString style = QLatin1String(qss_file.readAll());
        app.setStyleSheet(style);
        qss_file.close();
    }
    else qDebug("Open stylesheet.qss failed!");
}
