#include "cloudlinkerserver.h"
#include "ui_cloudlinkerserver.h"
#include<QMessageBox>
#include"Logger/logger.h"

CloudLinkerServer::CloudLinkerServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CloudLinkerServer)
{
    ui->setupUi(this);

    loadConfig();
    mytcpserver::getInstance().listen(QHostAddress(m_strIp),m_usport);
}

CloudLinkerServer::~CloudLinkerServer()
{
    delete ui;
}

void CloudLinkerServer::loadConfig()
{
    QFile file(":/client.config");
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray badata=file.readAll();
        QString strdata=badata.toStdString().c_str();

        file.close();
        strdata.replace("\r\n"," ");
        QStringList strlist= strdata.split(" ");
        m_strIp=strlist.at(0);
        m_usport=strlist.at(1).toUShort();
        LOG(QString("服务端初始化IP:%1,PORT:%2").arg(m_strIp).arg(m_usport));
    }
    else
    {
        QMessageBox::critical(this,"open config","open config ops!!");
    }
}

