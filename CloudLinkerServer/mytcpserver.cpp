#include "mytcpserver.h"
#include<QDebug>
#include<QThread>
#include"mytcpsocket.h"
mytcpserver::mytcpserver(QObject *parent)
    : QTcpServer{parent}
{

}

mytcpserver &mytcpserver::getInstance()
{
    static mytcpserver instance;
    return instance;
}

void mytcpserver::incomingConnection(qintptr socketDescriptor)//当有连接时到这里
{
    qDebug()<<"new client connected";
    MytcpSocket*pTcpSock=new MytcpSocket;
    pTcpSock->setSocketDescriptor(socketDescriptor);//接管socket
    m_tcpSocketList.append(pTcpSock);//将客户端socket放入list中 ->socket线程
    //从MytcpSocket类中offline信号发出，使用deleteSocket删除客户端socket
    connect(pTcpSock,SIGNAL(offline(MytcpSocket*)),this,SLOT(deleteSocket(MytcpSocket*)));

}

void mytcpserver::resend(const char *pername, PDU *pdu)
{
    if(pername==NULL||pdu==NULL)
    {
        return;
    }
    QString strName=pername;
    for(int i=0;i<m_tcpSocketList.size();i++)
    {
        if(strName==m_tcpSocketList.at(i)->getName())
        {
            m_tcpSocketList.at(i)->write((char*)pdu,pdu->uiPDULen);
            break;
        }
    }
}

void mytcpserver::deleteSocket(MytcpSocket *socket)//当客户端断开连接时删除list中的客户端socket
{
    qDebug()<<"delete socket";
    auto it=m_tcpSocketList.begin();
    for(;it!=m_tcpSocketList.end();it++)
    {
        if(socket==*it)
        {
            delete *it;
            m_tcpSocketList.erase(it);
            break;
        }
    }
}
