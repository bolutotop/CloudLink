#ifndef CLOUDLINKERSERVER_H
#define CLOUDLINKERSERVER_H

#include <QWidget>
#include<QFile>
#include<QString>
#include<mytcpserver.h>


QT_BEGIN_NAMESPACE
namespace Ui {
class CloudLinkerServer;
}
QT_END_NAMESPACE

class CloudLinkerServer : public QWidget
{
    Q_OBJECT

public:
    CloudLinkerServer(QWidget *parent = nullptr);
    ~CloudLinkerServer();
    void loadConfig();


private:
    Ui::CloudLinkerServer *ui;

    QString m_strIp;
    quint16 m_usport;
};
#endif // CLOUDLINKERSERVER_H
