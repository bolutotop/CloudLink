#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include<QList>
#include"mytcpsocket.h"
class mytcpserver : public QTcpServer
{
    Q_OBJECT
public:
    explicit mytcpserver(QObject *parent = nullptr);

    static mytcpserver& getInstance();

    void incomingConnection(qintptr socketDescriptor);//当有连接时到此

    void resend(const char* pername,PDU* pdu);//转发

public slots:

    void deleteSocket(MytcpSocket*socket);

private:

    QList<MytcpSocket*> m_tcpSocketList;
};

#endif // MYTCPSERVER_H
