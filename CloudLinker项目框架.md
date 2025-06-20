# CloudLinker项目框架

## 客户端与服务端的TCP连接

### cloudlinkerclient类中
```c++
    m_tcpsocket.connectToHost(QHostAddress(m_strIp),m_usport);

    connect(&m_tcpsocket,SIGNAL(connected()),this,SLOT(showConnect()));
    connect(&m_tcpsocket,SIGNAL(readyRead()),this,SLOT(recvMsg()));
    connect(&m_tcpsocket,SIGNAL(disconnected()),this,SLOT(clinetOffline()));
```
- **连接建立**：**connectToHost()** 函数初始化 TCP 连接
- **连接成功**：**connected()** 信号触发 **showConnect()** 槽
- **数据接收**：**readyRead()** 信号触发 **recvMsg()** 槽
- **断开检测**：**disconnected()** 信号触发 **clientOffline()** 槽

### mytcpserver继承QTcpServer的类中
```c++
    mytcpserver::getInstance().listen(QHostAddress(m_strIp),m_usport);//使用QTcpServer的成员函数listen函数监听端口

    void mytcpserver::incomingConnection(qintptr socketDescriptor)//当有连接时到这里
{
    MytcpSocket*pTcpSock=new MytcpSocket;
    pTcpSock->setSocketDescriptor(socketDescriptor);//接管socket
    m_tcpSocketList.append(pTcpSock);//将客户端socket放入list中 ->socket线程
    //从MytcpSocket类中offline信号发出，使用deleteSocket删除客户端socket
    connect(pTcpSock,SIGNAL(offline(MytcpSocket*)),this,SLOT(deleteSocket(MytcpSocket*)));
}
```
重写**incomingConnection()** 函数处理客户端的连接，当有客户端连接时进入此函数,使用**QList<MytcpSocket*> m_tcpSocketList**来存放客户端**socket**，进行多个客户端的管理

## protocol自定义通讯协议类

### 弹性结构体

```c++
struct PDU
{
    uint uiPDULen;   //总的协议数据单元大小
    uint uiMsgType;  //消息类型
    char caData[64];  //辅助信息
    uint uiMsgLen;   //实际消息长度
    int caMsg[];     //实际消息   
};
```
命令数据单元结构体,用于发送命令数据,比如注册请求，登录请求等

### 创建协议数据单元函数
```c++
PDU *mkPDU(uint uiMsgLen)
{
    uint uiPDULen=sizeof(PDU)+uiMsgLen;

    PDU *pdu=(PDU*)malloc(uiPDULen);
    if(pdu==nullptr)
    {
        exit(EXIT_FAILURE);
    }

    memset(pdu,0,uiPDULen);

    pdu->uiPDULen=uiPDULen;
    pdu->uiMsgLen=uiMsgLen;

    return pdu;
}
```
计算总协议数据单元大小，使用**malloc**函数分配内存空间，使用**memset**函数初始化内存，返回这个结构体指针。

### 消息类型枚举
```c++
enum ENUM_MSG_TYPE
{
    ENUM_MSG_TYPE_MIN=0,
    ENUM_MSG_TYPE_REGIST_REQUEST,  //注册请求 regist request
    ENUM_MSG_TYPE_REGIST_RESPOND,  //注册回复 regist respond
    //等......
    ENUM_MSG_TYPE_MAX=0x00ffffff,
}
```
使用枚举来限定消息类型，确保消息正确发送和处理

### 处理文件传输的结构体
```c++
struct FileInfo
{
    char caFileName[32];//文件名
    char suffix[32];  //文件后缀
    int iFileType;      //文件类型
    qint64 fileSizeInBytes;//文件大小
};
```
特殊的文件传输结构体，用于文件传输

## 消息发送与回复，客户端与服务端的交互

### 客户端发送消息(登录请求演示)
```c++
    m_strUserName=userName;
    PDU*pdu=mkPDU(0);
    pdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_REQUEST;
    strncpy(pdu->caData,userName.toStdString().c_str(),32);
    strncpy(pdu->caData+32,password.toStdString().c_str(),32);

    m_tcpsocket.write(reinterpret_cast<char*>(pdu),pdu->uiPDULen);
    free(pdu);
    pdu=NULL;
```
使用**m_tcpsocket** 的 **write**函数发送消息，使用 **mkPDU** 函数创建协议数据单元 ，**消息类型为ENUM_MSG_TYPE_LOGIN_REQUEST** ,使用**strncpy**函数将用户名和密码复制到协议数据单元中，使用**free**函数释放内存空间。

### 服务端处理消息(登录请求演示)

#### 解析协议数据单元分配正确的内存空间
```c++
    connect(this,SIGNAL(readyRead()),this,SLOT(recvMsg()));//有读IO信号

    uint uiPDULen=0;
    this->read(reinterpret_cast<char*>(&uiPDULen),sizeof(uint));//先接受总长度（主要目的是为了计算改分配的内存给pdu）
    uint uiMsgLen=uiPDULen-sizeof(PDU);//额外弹性数组长度
    PDU* pdu =mkPDU(uiMsgLen);//分配pdu的额外内存
    this->read(reinterpret_cast<char*>(pdu)+sizeof(uint),uiPDULen-sizeof(uint));//接收uiMsgType后的数据
```

```
内存示意图
 0      4      8                72      76       结束
┌──────┬──────┬────────────────┬───────┬────────┐
│PDULen│MsgType│    caData      │MsgLen │ caMsg  │
└──────┴──────┴────────────────┴───────┴────────┘
◄─4B─►◄─4B─►◄───────64B───────►◄─4B─►◄─变长区──►
```

先读取头部信息 总协议数据单元的大小，然后根据总协议数据单元的大小分配内存，然后读取剩余数据，最后根据消息类型处理消息。

#### 线程池处理消息
```c++
QPointer<MytcpSocket> safeThis(this);//使用线程安全的this指针

QMetaObject::invokeMethod(this, [safeThis, pdu]() {
    MytcpSocket::threadPool.enques([pdu, safeThis]() mutable {
        switch (pdu->uiMsgType) 
        {
            case ENUM_MSG_TYPE_LOGIN_REQUEST:
            {
                char userName[32]={'\0'};
                char password[32]={'\0'};
                strncpy(userName,pdu->caData,32);
                strncpy(password,pdu->caData+32,32);

                PDU* respdu = mkPDU(0);
                respdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_RESPOND;//处理登录回复

                QMetaObject::invokeMethod(safeThis, [safeThis, respdu,userName,password]() {
                    //数据库操作....
                    safeThis->write(reinterpret_cast<char*>(respdu), respdu->uiPDULen);
                    free(respdu);
                });
            }
            //case :........
        }
    });
});
```
先转到当前客户端socket线程,使用线程池处理消息，根据消息类型处理消息，转到当前客户端socket线程用安全指针发送回复消息。客户端的处理回复的方式相同。

## 服务端数据库操作

### 数据库操作类(继承QObject)

#### 线程安全处理数据库
```c++
private:
    OpenDB(QObject* parent = nullptr); // 禁止外部创建
    OpenDB(const OpenDB&) = delete;
    OpenDB& operator=(const OpenDB&) = delete;

    static QMutex m_mutex;

    QSqlDatabase m_db;
``` 
单例模式，使用**QMutex**互斥锁保证线程安全。
#### 初始化init()
```c++
    m_db.setHostName("localhost");
    m_db.setDatabaseName("C:\\Users\\55047\\Desktop\\myproject\\CloudLinker\\CloudLinkerServer\\cloud.db");
    if(m_db.open())
    {
        QSqlQuery query;
        query.exec("select*from usrInfo");
        while(query.next())
        {
            QString data=QString("%1,%2,%3").
                           arg(query.value(0).toString()).
                           arg(query.value(1).toString()).
                           arg(query.value(2).toString());
            qDebug()<<data;
        }
    }
    else
    {
        QMessageBox::critical(NULL,"打开数据库","打开数据库失败");
    }

    //析构调用 m_db.close();确保资源释放
```
**setHostName**设置数据库的主机名，**setDatabaseName**设置数据库的文件名，**open**打开数据库。

#### 数据库操作登录为例
```c++
bool OpenDB::login(const char *username, const char *password)
{
    QMutexLocker lock(&m_mutex);
    if(username==NULL||password==NULL)
    {
        return false;
    }
    QString data =QString("select*from usrInfo where name=\'%1\'and pwd=\'%2\'and online =0").arg(username).arg(password);
    QSqlQuery query;
    query.exec(data);
    if(query.next())
    {
        QString data =QString("update usrInfo set online=1 where name=\'%1\'and pwd=\'%2\'").arg(username).arg(password);
        QSqlQuery query;
        query.exec(data);
        return true;
    }
    else
    {
        return false;
    }
}
```
使用**QMutexLocker lock(&m_mutex)** 确保线程安全,**exec**执行命令，**next**判断是否还有下一条数据。

## 文件传输

### 客户端发出文件传输请求(以上传文件为例)
```c++
m_pTimer->start(1000);
connect(m_pTimer,SIGNAL(timeout()),this,SLOT(uploadFileDate()));

void Network_disk::uploadFileDate()
{
    m_pTimer->stop();
    QFile file(m_uploadFilePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    char *pbuffer=new char[4096];
    qint64 ret=0;
    while(1)
    {
        ret=file.read(pbuffer,4096);
        if(ret>0&&ret<=4096)
        {
            CloudLinkerClient::getInstance().getUserSocket().write(pbuffer,ret);
        }
        else if(0==ret)
        {
            break;
        }
    }
    file.close();
    delete []pbuffer;
    pbuffer=NULL;
}
```
使用定时器发送文件数据，避免文件数据流与命令数据流混在一起。

### 服务端接收文件数据
```c++
void MytcpSocket::recvMsg()//处理信号
{
    if(!m_bUpload)
    {
        //处理命令
    }
    else
    {
        //处理文件
            {
        QPointer<MytcpSocket> safeThis(this);
        QByteArray buffer = readAll();
        QMetaObject::invokeMethod(this, [safeThis, buffer]() {
            if (safeThis.isNull()) return;
            safeThis->m_file.write(buffer);
            safeThis->m_iRecved += buffer.size();
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
}
```
先是到处理命令，设置标识符，等客户端计时器，之后进入**else**处理文件传输。

