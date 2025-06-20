#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>
#include<protocol.h>
#include <QPointer>
#include<opendb.h>
#include"TreadPool.h"
#include<QFile>
#include<QTimer>
class MytcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    QString threadIdToStr(const std::thread::id& id);
    explicit MytcpSocket(QObject *parent = nullptr);

    static ThreadPool threadPool;
    QString getName();
    void online();
    void socketResend(const char*friendName,PDU*pdu);
signals:
    void offline(MytcpSocket*socket);

public slots:
    void recvMsg();
    void clinetOffline();
    void sendFileToClient();
private:
    QPointer<MytcpSocket> thisPtr;

    QString m_userName;
    QFile m_file;
    qint64 m_iTotal;
    qint64 m_iRecved;
    bool m_bUpload;
    QTimer *m_time;
};

#endif // MYTCPSOCKET_H
