#include "mytcpsocket.h"
#include "TreadPool.h"
#include"mytcpserver.h"
#include <QThread>
#include<QStringList>
#include<QDir>
#include"Logger/logger.h"

ThreadPool MytcpSocket::threadPool(8);


QString MytcpSocket::threadIdToStr(const std::thread::id &id)
{
    std::stringstream ss;
    ss << id;
    return QString::fromStdString(ss.str());
}

MytcpSocket::MytcpSocket(QObject *parent)
    : QTcpSocket{parent}
{


    connect(this,SIGNAL(readyRead()),this,SLOT(recvMsg()));//有读IO信号

    connect(this,SIGNAL(disconnected()),this,SLOT(clinetOffline()));//断开信号发出

    m_bUpload=false;

    m_time=new QTimer;
    connect(m_time,SIGNAL(timeout()),SLOT(sendFileToClient()));
}

QString MytcpSocket::getName()
{
    return m_userName;
}

void MytcpSocket::online()
{
    QStringList ret=OpenDB::getInstance().flushFriend(m_userName.toStdString().c_str());

    PDU*respdu=mkPDU(0);
    respdu->uiMsgType=ENUM_MSG_TYPE_ONLINE_FLUSH_REQUEST;
    for(int i=0;i<ret.size();i++)
    {
        qDebug()<<ret.at(i);
        mytcpserver::getInstance().resend(ret.at(i).toStdString().c_str(),respdu);
    }
    free(respdu);
    respdu=NULL;
}

void MytcpSocket::socketResend(const char*friendName,PDU*pdu)
{
    mytcpserver::getInstance().resend(friendName,pdu);
}

void MytcpSocket::recvMsg()//处理信号
{
    if(!m_bUpload)
    {
        qDebug()<<this->bytesAvailable();
        LOG(QString("命令数据:%1字节").arg(this->bytesAvailable()));
        uint uiPDULen=0;
        this->read(reinterpret_cast<char*>(&uiPDULen),sizeof(uint));//先接受总长度（主要目的是为了计算改分配的内存给pdu）
        qDebug()<<uiPDULen;
        uint uiMsgLen=uiPDULen-sizeof(PDU);//额外弹性数组长度
        qDebug()<<uiMsgLen;
        PDU* pdu =mkPDU(uiMsgLen);//分配pdu的额外内存
        this->read(reinterpret_cast<char*>(pdu)+sizeof(uint),uiPDULen-sizeof(uint));//接收uiMsgType后的数据

        QPointer<MytcpSocket> safeThis(this);//使用线程安全的this指针

        QMetaObject::invokeMethod(this, [safeThis, pdu]() {//切换成this(socket)对象的线程
            if (safeThis.isNull()) {
                qDebug() << "Socket destroyed before processing";
                free(pdu);
                return;
            }
            MytcpSocket::threadPool.enques([pdu, safeThis]() mutable {
                if (safeThis.isNull()) {
                    qDebug() << "Socket destroyed before processing";
                    free(pdu);
                    return;
                }
                switch (pdu->uiMsgType) {
                case ENUM_MSG_TYPE_MIN: // 测试
                {
                    // QThread::sleep(5);
                    qDebug() << (char*)pdu->caMsg;
                    PDU* respdu = mkPDU(0);
                    respdu->uiMsgType = ENUM_MSG_TYPE_MIN;
                    QString strmsg = "666";
                    strncpy(respdu->caData, strmsg.toStdString().c_str(), strmsg.size());
                    // 使用线程安全方式发送响应
                    QMetaObject::invokeMethod(safeThis, [safeThis, respdu]() {//切换成safeThis(socket)对象的线程
                        if (safeThis && safeThis->state() == QAbstractSocket::ConnectedState) {
                            safeThis->write(reinterpret_cast<char*>(respdu), respdu->uiPDULen);
                            free(respdu);
                        }
                    });
                    respdu=NULL;
                    break;
                }
                case ENUM_MSG_TYPE_REGIST_REQUEST://注册请求
                {
                    char userName[32]={'\0'};
                    char password[32]={'\0'};

                    strncpy(userName,pdu->caData,32);
                    strncpy(password,pdu->caData+32,32);
                    PDU* respdu = mkPDU(0);
                    respdu->uiMsgType=ENUM_MSG_TYPE_REGIST_RESPOND;//注册回复

                    QMetaObject::invokeMethod(safeThis, [safeThis, respdu,userName,password]() {//切换成safeThis(socket)对象的线程
                        if (safeThis && safeThis->state() == QAbstractSocket::ConnectedState) {
                            if(OpenDB::getInstance().regist(userName,password))
                            {
                                strcpy(respdu->caData,"REGIST OK");
                                safeThis->m_userName=userName;
                                QDir dir;
                                dir.mkdir(QString("./%1").arg(userName));
                            }
                            else
                            {
                                strcpy(respdu->caData,"REGIST NO : username existed");
                            }
                            safeThis->write(reinterpret_cast<char*>(respdu), respdu->uiPDULen);
                            free(respdu);
                        }
                    });
                    respdu=NULL;
                    break;
                }
                case ENUM_MSG_TYPE_LOGIN_REQUEST://处理登录请求
                {
                    //当前
                    LOG(QString("命令数据为处理%1登录请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));

                    char userName[32]={'\0'};
                    char password[32]={'\0'};
                    strncpy(userName,pdu->caData,32);
                    strncpy(password,pdu->caData+32,32);
                    PDU* respdu = mkPDU(0);
                    respdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_RESPOND;//处理登录回复

                    QMetaObject::invokeMethod(safeThis, [safeThis, respdu,userName,password]() {//切换成safeThis(socket)对象的线程
                        if (safeThis && safeThis->state() == QAbstractSocket::ConnectedState) {
                            if(OpenDB::getInstance().login(userName,password))  //对数据库操作
                            {
                                strcpy(respdu->caData,LOGIN_OK);
                                safeThis->m_userName=userName;//用安全指针访问私有成员
                                safeThis->online();
                                LOG(QString("命令数据为处理%1登录成功回复,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                            }
                            else
                            {
                                strcpy(respdu->caData,LOGIN_FAILED);
                                LOG(QString("命令数据为处理%1登录失败,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                            }
                            safeThis->write(reinterpret_cast<char*>(respdu), respdu->uiPDULen);
                            free(respdu);
                        }
                    });
                    respdu=NULL;
                    break;
                }
                case ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST://处理刷新好友请求
                {
                    LOG(QString("命令数据为处理%1刷新请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                    char userName[32]={'\0'};
                    strncpy(userName,pdu->caData,32);
                    QMetaObject::invokeMethod(safeThis, [safeThis,userName]() {//切换成safeThis(socket)对象的线程
                        if (safeThis && safeThis->state() == QAbstractSocket::ConnectedState) {

                            QStringList ret=OpenDB::getInstance().flushFriend(userName);
                            uint uiMsgLen=ret.size()*32+1;
                            PDU* respdu = mkPDU(uiMsgLen);

                            respdu->uiMsgType=ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND;//刷新好友回复
                            for(int i=0;i<ret.size();i++)
                            {
                                memcpy((char*)(respdu->caMsg)+i*32
                                       ,ret.at(i).toStdString().c_str()
                                       ,ret.at(i).size());
                            }
                            LOG(QString("命令数据为处理%1刷新好友,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                            safeThis->write(reinterpret_cast<char*>(respdu), respdu->uiPDULen);
                            free(respdu); // 确保内存释放
                            respdu=NULL;
                        }
                    });
                    break;
                }
                case ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST:
                {
                    char friendName[32]={'\0'};
                    strcpy(friendName,pdu->caData);
                    QMetaObject::invokeMethod(safeThis, [safeThis,pdu,friendName]() {//切换成safeThis(socket)对象的线程
                        if (safeThis && safeThis->state() == QAbstractSocket::ConnectedState) {
                            LOG(QString("命令数据为处理%1转发私聊,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                            safeThis->socketResend(friendName,pdu);
                        }
                    });
                    break;
                }
                case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:
                {
                    LOG(QString("命令数据为处理%1加好友请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                    char userName[32]={'\0'};
                    char friendName[32]={'\0'};
                    strncpy(userName,pdu->caData,32);
                    strncpy(friendName,pdu->caData+32,32);
                    PDU*respdu=mkPDU(0);
                    strncpy(respdu->caData,userName,32);
                    strncpy(respdu->caData+32,friendName,32);
                    QMetaObject::invokeMethod(safeThis, [safeThis,respdu,friendName,userName,pdu]() {//切换成safeThis(socket)对象的线程
                        if (safeThis && safeThis->state() == QAbstractSocket::ConnectedState) {
                            int ret =OpenDB::getInstance().addFriend(userName,friendName);
                            qDebug()<<ret<<userName<<" "<<friendName;
                            if(ret==-1)//未知错误
                            {
                                respdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
                                strcpy(respdu->caData,UNKNOW_ERROR);
                                safeThis->write(reinterpret_cast<char*>(respdu), respdu->uiPDULen);
                                free(respdu); // 确保内存释放
                            }
                            else if(ret==0)//已经是好友
                            {
                                respdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
                                strcpy(respdu->caData,EXISTED_FRIEND);
                                safeThis->write(reinterpret_cast<char*>(respdu), respdu->uiPDULen);
                                free(respdu); // 确保内存释放
                            }
                            else if(ret==1)//在线 请求添加好友
                            {
                                respdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
                                LOG(QString("命令数据为处理%1转发加好友请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                                safeThis->socketResend(friendName,respdu);
                                free(respdu); // 确保内存释放
                            }
                            else if(ret==2)//不在线 不可请求
                            {
                                respdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
                                strcpy(respdu->caData,ADD_FRIEND_OFFLINE);
                                safeThis->write(reinterpret_cast<char*>(respdu), respdu->uiPDULen);
                                free(respdu); // 确保内存释放
                            }
                            else//不存在此人
                            {
                                respdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
                                strcpy(respdu->caData,ADD_FRIEND_NO_EXIST);
                                safeThis->write(reinterpret_cast<char*>(respdu), respdu->uiPDULen);
                                free(respdu);
                            }
                            LOG(QString("命令数据为返回%1加好友结果,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                        }

                    });

                    respdu=NULL;
                    break;
                }
                case ENUM_MSG_TYPE_ADD_FRIEND_AGREE://同意添加好友
                {
                    LOG(QString("命令数据为处理%1同意添加好友请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                    char userName[32]={'\0'};
                    char friendName[32]={'\0'};
                    strncpy(userName,pdu->caData,32);
                    strncpy(friendName,pdu->caData+32,32);

                    qDebug()<<userName<<" "<<friendName;
                    PDU*respdu=mkPDU(0);
                    respdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;

                    QString res=QString("Add friend successfully");
                    strcpy(respdu->caData,res.toStdString().c_str());
                    QMetaObject::invokeMethod(safeThis, [safeThis,userName,respdu,friendName]() {//切换成safeThis(socket)对象的线程
                        if (safeThis && safeThis->state() == QAbstractSocket::ConnectedState) {
                            LOG(QString("命令数据为处理%1发送加好友回复,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                            OpenDB::getInstance().addFriendDeel(userName,friendName);
                            safeThis->socketResend(userName,respdu);
                            safeThis->socketResend(friendName,respdu);
                            free(respdu);
                        }
                    });
                    respdu=NULL;
                    break;
                }
                case ENUM_MSG_TYPE_ADD_FRIEND_REFUSE://拒绝添加好友
                {
                    LOG(QString("命令数据为处理%1拒绝添加好友请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                    char userName[32]={'\0'};
                    char friendName[32]={'\0'};

                    strncpy(userName,pdu->caData,32);
                    strncpy(friendName,pdu->caData+32,32);

                    qDebug()<<userName<<" "<<friendName;
                    PDU*respdu=mkPDU(0);
                    respdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
                    QString res=QString("%1 refuse add friend").arg(friendName);
                    strcpy(respdu->caData,res.toStdString().c_str());
                    QMetaObject::invokeMethod(safeThis, [safeThis,respdu]() {//切换成safeThis(socket)对象的线程
                        if (safeThis && safeThis->state() == QAbstractSocket::ConnectedState) {
                            LOG(QString("命令数据为处理%1发送拒绝添加好友回复,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                            safeThis->write(reinterpret_cast<char*>(respdu), respdu->uiPDULen);
                            free(respdu);
                        }
                    });
                    respdu=NULL;
                    break;
                }
                case ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST://删除好友
                {
                    LOG(QString("命令数据为处理%1删除好友请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                    char userName[32]={'\0'};
                    char friendName[32]={'\0'};

                    strncpy(userName,pdu->caData,32);
                    strncpy(friendName,pdu->caData+32,32);
                    qDebug()<<userName<<" "<<friendName;
                    PDU*respdu=mkPDU(0);
                    respdu->uiMsgType=ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND;//回复
                    QMetaObject::invokeMethod(safeThis, [safeThis,respdu,userName,friendName]() {//切换成safeThis(socket)对象的线程
                        if (safeThis && safeThis->state() == QAbstractSocket::ConnectedState) {
                            if(OpenDB::getInstance().deleteFriend(userName,friendName))
                            {
                                strncpy(respdu->caData+32,friendName,32);
                                strcpy(respdu->caData,DEL_FRIEND_OK);
                                safeThis->socketResend(friendName,respdu);
                                safeThis->write(reinterpret_cast<char*>(respdu), respdu->uiPDULen);
                                LOG(QString("命令数据为处理%1回复删除成功请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                            }
                            else
                            {
                                strcpy(respdu->caData,DEL_FRIEND_NO);
                                safeThis->write(reinterpret_cast<char*>(respdu), respdu->uiPDULen);
                                LOG(QString("命令数据为处理%1回复删除好友失败请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                            }
                            free(respdu);
                        }
                    });
                    respdu=NULL;
                    break;
                }
                case ENUM_MSG_TYPE_CREATE_DIR_REQUEST://创建文件夹
                {
                    LOG(QString("命令数据为处理%1创建文件夹请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                    qDebug()<<"创建文件夹";
                    QDir dir;
                    QString strCurPath=QString("%1").arg((char*)(pdu->caMsg));
                    bool ret=dir.exists(strCurPath);
                    PDU*respdu=NULL;

                    if(ret)//创建的文件名存在
                    {
                        qDebug()<<strCurPath<<"路径存在";
                        char caNewDir[32]={'\0'};
                        memcpy(caNewDir,pdu->caData+32,32);
                        QString strNewPath=strCurPath+"/"+caNewDir;
                        qDebug()<<strNewPath;
                        ret=dir.exists(strNewPath);
                        if(ret)//文件夹名已存在
                        {
                            respdu=mkPDU(0);
                            respdu->uiMsgType=ENUM_MSG_TYPE_CREATE_DIR_RESPOND;
                            strcpy(respdu->caData,DIR_NO_EXIST);
                        }
                        else
                        {
                            dir.mkdir(strNewPath);
                            respdu=mkPDU(0);
                            respdu->uiMsgType=ENUM_MSG_TYPE_CREATE_DIR_RESPOND;
                            strcpy(respdu->caData,CRAETE_DIR_OK);
                        }
                    }
                    else//创建的文件名不存在
                    {
                        respdu=mkPDU(0);
                        respdu->uiMsgType=ENUM_MSG_TYPE_CREATE_DIR_RESPOND;
                        strcpy(respdu->caData,FILE_NAME_EXIST);
                    }
                    QMetaObject::invokeMethod(safeThis, [safeThis,respdu]() {//切换成safeThis(socket)对象的线程
                        if (safeThis && safeThis->state() == QAbstractSocket::ConnectedState) {
                            safeThis->write(reinterpret_cast<char*>(respdu), respdu->uiPDULen);
                            LOG(QString("命令数据为处理%1回复处理创建文件夹请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                            free(respdu);
                        }
                    });
                    respdu=NULL;
                    break;
                }
                case ENUM_MSG_TYPE_FLUSH_FILE_REQUEST:
                {
                    LOG(QString("命令数据为处理%1刷新文件请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                    char *pCurPath=new char[pdu->uiMsgLen];
                    memcpy(pCurPath,pdu->caMsg,pdu->uiMsgLen);
                    qDebug()<<*pCurPath;
                    QDir dir(pCurPath);
                    QFileInfoList fileInfoList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System,
                                                                   QDir::DirsFirst);
                    int iFileCount=fileInfoList.size();
                    PDU*respdu=mkPDU(sizeof(FileInfo)*iFileCount);
                    FileInfo *pFileInfo=NULL;
                    QString strFileName;
                    QString suffix;
                    respdu->uiMsgType=ENUM_MSG_TYPE_FLUSH_FILE_RESPOND;

                    for(int i=0;i<iFileCount;i++)
                    {
                        pFileInfo=(FileInfo*)(respdu->caMsg)+i;
                        strFileName=fileInfoList[i].fileName();
                        suffix =fileInfoList[i].suffix().toLower();
                        qDebug()<<suffix;
                        memcpy(pFileInfo->suffix,suffix.toStdString().c_str(),suffix.size());
                        memcpy(pFileInfo->caFileName,strFileName.toStdString().c_str(),strFileName.size());

                        if(fileInfoList[i].isDir())//文件夹
                        {
                            pFileInfo->iFileType=0;
                        }
                        else if(fileInfoList[i].isFile())//文件
                        {
                            pFileInfo->iFileType=1;
                        }
                        // qDebug()<<fileInfoList[i].fileName()
                        //          <<fileInfoList[i].size()
                        //     <<"文件夹:"
                        //          <<fileInfoList[i].isDir()
                        //     <<"常规文件:"
                        //          <<fileInfoList[i].isFile();
                        pFileInfo->fileSizeInBytes=fileInfoList[i].size();
                    }
                    QMetaObject::invokeMethod(safeThis, [safeThis,respdu]() {//切换成safeThis(socket)对象的线程
                        if (safeThis && safeThis->state() == QAbstractSocket::ConnectedState) {
                            safeThis->write(reinterpret_cast<char*>(respdu), respdu->uiPDULen);
                            LOG(QString("命令数据为处理%1回复刷新文件夹请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                            free(respdu);
                        }
                    });
                    respdu=NULL;
                    delete []pCurPath;
                    pCurPath=NULL;
                    break;
                }
                case ENUM_MSG_TYPE_DEL_DIR_REQUEST:
                {
                    LOG(QString("命令数据为处理%1删除文件夹请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                    char caName[32]={'\0'};
                    strcpy(caName,pdu->caData);
                    char*pPath=new char[pdu->uiMsgLen];
                    memcpy(pPath,pdu->caMsg,pdu->uiMsgLen);
                    QString  strPath=QString("%1/%2").arg(pPath).arg(caName);
                    qDebug()<<strPath;

                    QFileInfo fileInfo(strPath);
                    bool ret=false;
                    PDU*respdu=NULL;
                    if(fileInfo.isDir())
                    {
                        QDir dir;
                        dir.setPath(strPath);
                        ret=dir.removeRecursively();
                        if(!ret)//删除文件夹失败时
                        {
                            respdu=mkPDU(0);
                            respdu->uiMsgType=ENUM_MSG_TYPE_DEL_DIR_RESPOND;
                            memcpy(respdu->caData,DEL_DIR_NO,strlen(DEL_DIR_NO));
                        }
                        else
                        {
                            respdu=mkPDU(0);
                            respdu->uiMsgType=ENUM_MSG_TYPE_DEL_DIR_RESPOND;
                            memcpy(respdu->caData,DEL_DIR_OK,strlen(DEL_DIR_OK));
                        }
                    }
                    else if(fileInfo.isFile())//常规文件
                    {
                        ret=QFile::remove(strPath); // 删除文件;
                        if(!ret)//删除文件失败时
                        {
                            respdu=mkPDU(0);
                            respdu->uiMsgType=ENUM_MSG_TYPE_DEL_DIR_RESPOND;
                            memcpy(respdu->caData,DEL_FILE_NO,strlen(DEL_FILE_NO));
                            break;
                        }
                        else
                        {
                            respdu=mkPDU(0);
                            respdu->uiMsgType=ENUM_MSG_TYPE_DEL_DIR_RESPOND;
                            memcpy(respdu->caData,DEL_DIR_OK,strlen(DEL_FILE_OK));
                        }
                    }
                    QMetaObject::invokeMethod(safeThis, [safeThis,respdu]() {//切换成safeThis(socket)对象的线程
                        if (safeThis && safeThis->state() == QAbstractSocket::ConnectedState) {
                            safeThis->write(reinterpret_cast<char*>(respdu), respdu->uiPDULen);
                            LOG(QString("命令数据为处理%1回复删除文件夹请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                            free(respdu);
                        }
                    });
                    respdu=NULL;
                    break;
                }
                case ENUM_MSG_TYPE_RENAME_DIR_REQUEST:
                {
                    LOG(QString("命令数据为处理%1重命名请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                    char oldName[32]={'\0'};
                    char newName[32]={'\0'};
                    strcpy(newName,pdu->caData+32);
                    strcpy(oldName,pdu->caData);
                    char*pPath=new char[pdu->uiMsgLen];
                    memcpy(pPath,pdu->caMsg,pdu->uiMsgLen);
                    QString  strPath=QString("%1/%2").arg(pPath).arg(oldName);
                    QString  newPath=QString("%1/%2").arg(pPath).arg(newName);
                    qDebug()<<strPath<<" "<<newPath;

                    PDU*respdu=NULL;
                    QDir dir;
                    if(dir.rename(strPath,newPath))
                    {
                        respdu=mkPDU(0);
                        respdu->uiMsgType=ENUM_MSG_TYPE_RENAME_DIR_RESPOND;
                        memcpy(respdu->caData,RENAME_FILE_OK,strlen(RENAME_FILE_OK));
                    }
                    else
                    {
                        respdu=mkPDU(0);
                        respdu->uiMsgType=ENUM_MSG_TYPE_RENAME_DIR_RESPOND;
                        memcpy(respdu->caData,RENAME_FILE_OK,strlen(RENAME_FILE_OK));
                    }
                    QMetaObject::invokeMethod(safeThis, [safeThis,respdu]() {//切换成safeThis(socket)对象的线程
                        if (safeThis && safeThis->state() == QAbstractSocket::ConnectedState) {
                            LOG(QString("命令数据为处理%1回复重命名请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                            safeThis->write(reinterpret_cast<char*>(respdu), respdu->uiPDULen);
                            free(respdu);
                        }
                    });
                    respdu=NULL;
                    break;
                }
                case ENUM_MSG_TYPE_ENTER_DIR_REQUEST://进入文件夹请求
                {
                    LOG(QString("命令数据为处理%1进入文件夹请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                    char *pCurPath=new char[pdu->uiMsgLen];
                    memcpy(pCurPath,pdu->caMsg,pdu->uiMsgLen);
                    QString newDir=QString("%1/%2").arg(pCurPath).arg(pdu->caData);

                    qDebug()<<newDir;

                    QDir dir(newDir);
                    QFileInfoList fileInfoList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System,
                                                                   QDir::DirsFirst);
                    int iFileCount=fileInfoList.size();
                    PDU*respdu=mkPDU(sizeof(FileInfo)*iFileCount);
                    FileInfo *pFileInfo=NULL;
                    QString strFileName;
                    QString suffix;
                    respdu->uiMsgType=ENUM_MSG_TYPE_ENTER_DIR_RESPOND;//回复

                    for(int i=0;i<iFileCount;i++)
                    {
                        pFileInfo=(FileInfo*)(respdu->caMsg)+i;
                        strFileName=fileInfoList[i].fileName();
                        suffix =fileInfoList[i].suffix().toLower();
                        qDebug()<<suffix;
                        memcpy(pFileInfo->suffix,suffix.toStdString().c_str(),suffix.size());
                        memcpy(pFileInfo->caFileName,strFileName.toStdString().c_str(),strFileName.size());

                        if(fileInfoList[i].isDir())//文件夹
                        {
                            pFileInfo->iFileType=0;
                        }
                        else if(fileInfoList[i].isFile())//文件
                        {
                            pFileInfo->iFileType=1;
                        }
                        // qDebug()<<fileInfoList[i].fileName()
                        //          <<fileInfoList[i].size()
                        //     <<"文件夹:"
                        //          <<fileInfoList[i].isDir()
                        //     <<"常规文件:"
                        //          <<fileInfoList[i].isFile();
                        pFileInfo->fileSizeInBytes=fileInfoList[i].size();
                    }
                    QMetaObject::invokeMethod(safeThis, [safeThis,respdu]() {//切换成safeThis(socket)对象的线程
                        if (safeThis && safeThis->state() == QAbstractSocket::ConnectedState) {
                            LOG(QString("命令数据为处理%1回复进入文件夹请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                            safeThis->write(reinterpret_cast<char*>(respdu), respdu->uiPDULen);
                            free(respdu);
                        }
                    });
                    respdu=NULL;

                    delete []pCurPath;
                    pCurPath=NULL;
                    break;
                }
                case ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST:
                {
                    LOG(QString("命令数据为处理%1上传文件夹请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                    char uploadFileName[32]={'\0'};
                    qint64 fileSize=0;
                    sscanf(pdu->caData,"%s %lld",uploadFileName,&fileSize);
                    char *CurPath=new char[pdu->uiMsgLen];
                    memcpy(CurPath,pdu->caMsg,pdu->uiMsgLen);

                    QString uploadPath=QString("%1/%2").arg(CurPath).arg(uploadFileName);
                    qDebug()<<uploadPath;

                    delete []CurPath;
                    CurPath=NULL;

                    safeThis->m_file.setFileName(uploadPath);

                    if(safeThis->m_file.open(QIODevice::WriteOnly))
                    {
                        safeThis->m_bUpload=true;
                        safeThis->m_iTotal=fileSize;
                        safeThis->m_iRecved=0;
                    }
                    break;
                }
                case ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST:
                {
                    LOG(QString("命令数据为处理%1下载文件请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                    char downloadFileName[32]={'\0'};
                    strcpy(downloadFileName,pdu->caData);
                    char *CurPath=new char[pdu->uiMsgLen];
                    memcpy(CurPath,pdu->caMsg,pdu->uiMsgLen);

                    QString downloadPath=QString("%1/%2").arg(CurPath).arg(downloadFileName);
                    qDebug()<<downloadPath;

                    delete []CurPath;
                    CurPath=NULL;

                    QFileInfo fileInfo(downloadPath);
                    qint64 fileSize =fileInfo.size();
                    PDU*respdu=mkPDU(0);
                    respdu->uiMsgType=ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND;
                    sprintf(respdu->caData,"%s %lld",downloadFileName,fileSize);
                    QMetaObject::invokeMethod(safeThis, [safeThis,respdu,downloadPath]() {//切换成safeThis(socket)对象的线程
                        if (safeThis && safeThis->state() == QAbstractSocket::ConnectedState) {
                            safeThis->m_file.setFileName(downloadPath);
                            safeThis->m_file.open(QIODevice::ReadOnly);
                            safeThis->write(reinterpret_cast<char*>(respdu), respdu->uiPDULen);
                            LOG(QString("命令数据为处理%1回复下载文件请求,当前服务线程ID为:%2").arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
                            safeThis->m_time->start(1000);
                            free(respdu);
                        }
                    });
                    respdu=NULL;
                    break;
                }
                default:
                    break;
                }
                free(pdu);
                pdu=NULL;
            });
        }, Qt::QueuedConnection); // 使用队列连接确保线程安全
    }
    else {
        QPointer<MytcpSocket> safeThis(this);
        QByteArray buffer = readAll();
        QMetaObject::invokeMethod(this, [safeThis, buffer]() {
            if (safeThis.isNull()) return;
            safeThis->m_file.write(buffer);
            safeThis->m_iRecved += buffer.size();
            LOG(QString("%2上传文件,总%3,已上传%4,当前服务线程ID为:%5")
                    .arg(safeThis->m_userName)
                    .arg(safeThis->m_iTotal)
                    .arg(safeThis->m_iRecved)
                    .arg(safeThis->threadIdToStr(std::this_thread::get_id())));

            if(safeThis->m_iRecved >= safeThis->m_iTotal) {
                safeThis->m_file.close();
                safeThis->m_bUpload = false;

                PDU* respdu = mkPDU(0);
                respdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND;
                strcpy(respdu->caData,
                       (safeThis->m_iRecved == safeThis->m_iTotal) ?
                           UPLOAD_FILE_OK : UPLOAD_FILE_NO);

                safeThis->write(reinterpret_cast<char*>(respdu), respdu->uiPDULen);
                free(respdu);
                respdu=NULL;
            }
        }, Qt::QueuedConnection);
    }
}

void MytcpSocket::clinetOffline()
{
    qDebug()<<"clinet disconnect";
    OpenDB::getInstance().disconnectUser(m_userName.toStdString().c_str());//让online归零为离线状态
    online();
    emit offline(this);//发出断开连接信号-> server的deleteSocket中
}

void MytcpSocket::sendFileToClient()
{
    QPointer<MytcpSocket> safeThis(this);
    char *pbuffer=new char[4096];
    QMetaObject::invokeMethod(this, [safeThis,pbuffer]() {
        safeThis->m_time->stop();
        qint64 ret=0;
        while(1)
        {
            ret=safeThis->m_file.read(pbuffer,4096);
            LOG(QString("文件数据流:%1字节,%2下载文件,当前服务线程ID为:%3").arg(ret).arg(safeThis->m_userName).arg(safeThis->threadIdToStr(std::this_thread::get_id())));
            if(ret>0&&ret<=4096)
            {
                safeThis->write(pbuffer,ret);
            }
            else if(ret==0)
            {
                safeThis->m_file.close();
                break;
            }
            else if(ret<0)
            {
                qDebug()<<"发送文件失败";
                safeThis->m_file.close();
                break;
            }
        }
        safeThis->m_file.close();
        delete []pbuffer;
    });
    pbuffer=NULL;
}

