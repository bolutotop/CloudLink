#include "cloudlinkerclient.h"
#include "ui_cloudlinkerclient.h"

#include<QMessageBox>
#include"Logger/logger.h"
#include"ElaApplication.h"
#include"Friend_chat/private_chat.h"
#include"Network_disk/network_disk.h"
#include"protocol.h"
#include"qtmaterialautocomplete.h"


CloudLinkerClient::CloudLinkerClient(QWidget *parent)
    : ElaWidget(parent)
    , ui(new Ui::CloudLinkerClient)
{
    ui->setupUi(this);
    ElaApplication::getInstance()->init();

    loadConfig();
    m_tcpsocket.connectToHost(QHostAddress(m_strIp),m_usport);

    connect(&m_tcpsocket,SIGNAL(connected()),this,SLOT(showConnect()));
    connect(&m_tcpsocket,SIGNAL(readyRead()),this,SLOT(recvMsg()));
    connect(&m_tcpsocket,SIGNAL(disconnected()),this,SLOT(clinetOffline()));

    //样式设计
    int r=10;
    setAttribute(Qt::WA_StyledBackground, true); // 启用样式表背景
    setStyleSheet(QString("QWidget{background-color: white;border-bottom-left-radius: %1px;border-bottom-right-radius: %1px;}").arg(r));
    ui->login_pushButton->setBackgroundColor("#3fbcbc");
    ui->login_pushButton->setCheckable(false);
    ui->login_pushButton->setHaloVisible(false);

    ui->go_regist_pushButton->setBackgroundColor("#3fbcbc");
    ui->go_regist_pushButton->setCheckable(false);
    ui->go_regist_pushButton->setHaloVisible(false);

    ui->passward_lineEdit->setLabel("password");
    ui->passward_lineEdit->setLabelColor("#3fbcbc");
    ui->passward_lineEdit->setInkColor("#3fbcbc");
    ui->passward_lineEdit->setShowLabel(true);

    ui->user_lineEdit->setLabel("username");
    ui->user_lineEdit->setLabelColor("#3fbcbc");
    ui->user_lineEdit->setInkColor("#3fbcbc");
    ui->user_lineEdit->setShowLabel(true);

    ui->rigist_uesrname_lineEdit->setLabel("username");
    ui->rigist_uesrname_lineEdit->setLabelColor("#3fbcbc");
    ui->rigist_uesrname_lineEdit->setInkColor("#3fbcbc");
    ui->rigist_uesrname_lineEdit->setShowLabel(true);

    ui->regist_passward_lineEdit->setLabel("password");
    ui->regist_passward_lineEdit->setLabelColor("#3fbcbc");
    ui->regist_passward_lineEdit->setInkColor("#3fbcbc");
    ui->regist_passward_lineEdit->setShowLabel(true);

    ui->regist_passward_ag_lineEdit->setLabel("Please enter your password again");
    ui->regist_passward_ag_lineEdit->setLabelColor("#3fbcbc");
    ui->regist_passward_ag_lineEdit->setInkColor("#3fbcbc");
    ui->regist_passward_ag_lineEdit->setShowLabel(true);

    ui->regist_pushButton->setBackgroundColor("#3fbcbc");
    ui->regist_pushButton->setCheckable(false);
    ui->regist_pushButton->setHaloVisible(false);

    ui->return_login_pushButton->setBackgroundColor("#3fbcbc");
    ui->return_login_pushButton->setCheckable(false);
    ui->return_login_pushButton->setHaloVisible(false);

    //给密码输入栏右侧添加图标(只有选中情况下才会显示眼睛图标)
    QAction*action=new QAction(this);
    action->setIcon(QIcon(":/pic/eyes-open.png"));
    connect(ui->passward_lineEdit,&QtMaterialAutoComplete::textEdited,this,[=]{
        static bool first=true;
        if(first)
        {
            ui->passward_lineEdit->addAction(action,QLineEdit::TrailingPosition);
            first=false;
        }
    });

    int stat=0;
    connect(action,&QAction::triggered,this,[=]mutable{
        if(stat==0)
        {
            action->setIcon(QIcon(":/pic/eyes-open.png"));
            ui->passward_lineEdit->addAction(action,QLineEdit::TrailingPosition);
            stat=1;
            ui->passward_lineEdit->setEchoMode(QLineEdit::Normal);
        }
        else
        {
            action->setIcon(QIcon(":/pic/preview-close.png"));
            ui->passward_lineEdit->addAction(action,QLineEdit::TrailingPosition);
            stat=0;
            ui->passward_lineEdit->setEchoMode(QLineEdit::Password);
        }
    });

    moveToCenter();
    setWindowButtonFlag(ElaAppBarType::ButtonType::MaximizeButtonHint,false);
    setWindowButtonFlag(ElaAppBarType::ButtonType::MinimizeButtonHint,false);
    setWindowButtonFlag(ElaAppBarType::ButtonType::ThemeChangeButtonHint,false);
    setWindowButtonFlag(ElaAppBarType::ButtonType::NavigationButtonHint,false);
    setWindowButtonFlag(ElaAppBarType::ButtonType::RouteBackButtonHint,false);
    setWindowButtonFlag(ElaAppBarType::ButtonType::StayTopButtonHint,false);

    // setWindowButtonFlag(ElaAppBarType::ButtonType::CloseButtonHint,false);
}

CloudLinkerClient::~CloudLinkerClient()
{
    delete ui;
}

CloudLinkerClient &CloudLinkerClient::getInstance()
{
    static CloudLinkerClient instance;
    return instance;
}

void CloudLinkerClient::loadConfig()
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
        LOG(QString("初始化IP:%1,PORT:%2").arg(m_strIp).arg(m_usport));
    }
    else
    {
        QMessageBox::critical(this,"open config","open config ops!!");
    }
}

void CloudLinkerClient::pageSwitch(int pageIndex)
{
    int curIndex = ui->stackedWidget->currentIndex();
    {
        if (pageIndex < 0 || pageIndex >= ui->stackedWidget->count()) {
            return;
        }
        QWidget *currentPage = ui->stackedWidget->currentWidget();
        QWidget *targetPage = ui->stackedWidget->widget(pageIndex);
        targetPage->setGeometry(0,0,currentPage->width(),currentPage->height());

        if (!currentPage || !targetPage || currentPage == targetPage) {
            return;
        }

        int currentPageX = currentPage->x();
        int targetPageX = targetPage->x();
        int currentPageWidth = currentPage->width();
        // 创建当前页面向左滑出的动画
        QPropertyAnimation *currentPageAnimation = new QPropertyAnimation(currentPage, "geometry");
        currentPageAnimation->setDuration(400);
        currentPageAnimation->setEasingCurve(QEasingCurve::InOutQuad);
        // 创建目标页面从右至左滑进的动画
        QPropertyAnimation *targetPageAnimation = new QPropertyAnimation(targetPage, "geometry");
        targetPageAnimation->setDuration(400);
        targetPageAnimation->setEasingCurve(QEasingCurve::InOutQuad);
        if(pageIndex == 0)
        {
            currentPageAnimation->setStartValue(QRect(currentPageX, currentPage->y(), currentPageWidth, currentPage->height()));
            currentPageAnimation->setEndValue(QRect(currentPageX - currentPageWidth, currentPage->y(), currentPageWidth, currentPage->height()));

            targetPageAnimation->setStartValue(QRect(targetPageX + currentPageWidth, targetPage->y(), targetPage->width(), targetPage->height()));
            targetPageAnimation->setEndValue(QRect(targetPageX, targetPage->y(), targetPage->width(), targetPage->height()));
        }
        else
        {
            currentPageAnimation->setStartValue(QRect(currentPageX, currentPage->y(), currentPageWidth, currentPage->height()));
            currentPageAnimation->setEndValue(QRect(currentPageX + currentPageWidth, currentPage->y(), currentPageWidth, currentPage->height()));

            targetPageAnimation->setStartValue(QRect(targetPageX - currentPageWidth, targetPage->y(), targetPage->width(), targetPage->height()));
            targetPageAnimation->setEndValue(QRect(targetPageX, targetPage->y(), targetPage->width(), targetPage->height()));
        }
        // 创建动画组，添加动画，并启动动画组
        QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup();
        animationGroup->addAnimation(currentPageAnimation);
        animationGroup->addAnimation(targetPageAnimation);
        ui->stackedWidget->widget(pageIndex)->setVisible(true);
        ui->stackedWidget->widget(pageIndex)->update();
        ui->stackedWidget->widget(pageIndex)->show();
        animationGroup->start(QAbstractAnimation::DeleteWhenStopped);

        animationGroup->setProperty(
            "widget", QVariant::fromValue(ui->stackedWidget->widget(curIndex)));

        connect(animationGroup, &QParallelAnimationGroup::finished, [=]() {
            //切换页面
            ui->stackedWidget->widget(pageIndex)->hide();
            ui->stackedWidget->setCurrentIndex(pageIndex);
            ui->stackedWidget->widget(pageIndex)->show();
        });
    }
}

QString CloudLinkerClient::getUserName()
{
    return m_strUserName;
}

QTcpSocket& CloudLinkerClient::getUserSocket()
{
    return m_tcpsocket;
}

QString CloudLinkerClient::getCurPath()
{
    return m_strCurPath;
}

void CloudLinkerClient::updatePath(const char *Path)
{
    m_strCurPath=Path;
}


// void CloudLinkerClinet::loginRequest(const QString &uesrName, const QString &password)
// {
//     PDU*pdu=mkPDU(0);
//     pdu->uiMsgType=ENUM_MSG_TYPE_MIN;

//     strncpy(reinterpret_cast<char*>(pdu->caMsg),uesrName.toStdString().c_str(),uesrName.size());

//     m_tcpsocket.write(reinterpret_cast<char*>(pdu),pdu->uiPDULen);
//     free(pdu);
//     pdu=NULL;
// }

void CloudLinkerClient::showConnect()
{
    ui->label_3->setText("已连接");
    LOG("客户端连接成功");
}

void CloudLinkerClient::recvMsg()
{
    if(!Network_disk::getInstance().getDownloadStatus())
    {
        qDebug()<<m_tcpsocket.bytesAvailable();
        LOG(QString("%1客户端接受消息:消息长度为:%2字节").arg(m_strUserName).arg(m_tcpsocket.bytesAvailable()));
        uint uiPDULen=0;
        m_tcpsocket.read((char*)&uiPDULen,sizeof(uint));
        uint uiMsgLen=uiPDULen-sizeof(PDU);
        PDU* pdu =mkPDU(uiMsgLen);
        m_tcpsocket.read((char*)pdu+sizeof(uint),uiPDULen-sizeof(uint));

        switch (pdu->uiMsgType) {
        // case ENUM_MSG_TYPE_MIN:    测试
        // {
        //     char cadata[32]={'\0'};
        //     strncpy(cadata,pdu->caData,32);
        //     QMessageBox::information(this,"成功",cadata);
        //     break;
        // }
        case ENUM_MSG_TYPE_REGIST_RESPOND://注册回复
        {
            char cadata[32]={'\0'};
            strncpy(cadata,pdu->caData,32);
            ui->Tips_regist->textlable->setText(cadata);
            ui->Tips_regist->animationStart();
            break;
        }
        case ENUM_MSG_TYPE_LOGIN_RESPOND://登录回复
        {
            qDebug()<<pdu->caData;
            if(strcmp(pdu->caData,LOGIN_OK)==0)
            {
                this->hide();
                m_strCurPath=QString("./%1").arg(m_strUserName);
                interfaceW=new Main_interface;
                interfaceW->setUserInfoCardTitle(m_strUserName);
                interfaceW->setUserInfoCardSubTitle("在线");
                interfaceW->show();
                LOG(QString("客户端: %1登录成功，路径为:%2").arg(m_strUserName).arg(m_strCurPath));
                break;
            }
            else if(strcmp(pdu->caData,LOGIN_FAILED)==0)
            {
                ui->Tips->textlable->setText(pdu->caData);
                ui->Tips->animationStart();
                LOG(QString("客户端: %1登录失败，密码错误").arg(m_strUserName).arg(m_strCurPath));
            }
            break;
        }
        case ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND://刷新好友列表回复
        {
            private_chat::getInstance().updateFriend(pdu);
            break;
        }
        case ENUM_MSG_TYPE_ONLINE_FLUSH_REQUEST://刷新好友通知
        {
            qDebug()<<"刷新好友处理通知,调用出发点flushFriend";
            private_chat::getInstance().flushFriend();
            break;
        }
        case ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST://发消息回复请求
        {
            private_chat::getInstance().resvMsg(pdu);
            break;
        }
        case ENUM_MSG_TYPE_ADD_FRIEND_RESPOND://加好友请求回复
        {
            qDebug()<<"加好友请求的回复 : "<<pdu->caData;
            private_chat::getInstance().showTipsAddFriend(pdu->caData);
            private_chat::getInstance().flushFriend();
            break;
        }
        case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST://加好友请求
        {
            char friendName[32]={'\0'};//被添加方的用户名
            char userName[32]={'\0'};//自己的用户名
            strncpy(userName,pdu->caData,32);
            strncpy(friendName,pdu->caData+32,32);
            int ret=QMessageBox::information(this,"添加好友",QString(" %1  want to add you friend?").arg(userName)
                                               ,QMessageBox::Yes,QMessageBox::No);
            PDU*respdu=mkPDU(0);
            memcpy(respdu->caData,pdu->caData,32);//目标名
            memcpy(respdu->caData+32,pdu->caData+32,32);//自己名
            if(ret==QMessageBox::Yes)
            {
                qDebug()<<"客户端:"<<userName<<"客户端"<<friendName<<"同意添加好友请求发送";
                respdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_AGREE;//同意

                m_tcpsocket.write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu=NULL;
            }
            else if(ret==QMessageBox::No)
            {
                respdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_REFUSE;//拒绝

                m_tcpsocket.write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu=NULL;
            }
            break;
        }
        case ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND://删除好友回复
        {
            char friendName[32]={'\0'};
            strncpy(friendName,pdu->caData+32,32);
            private_chat::getInstance().showTipsAddFriend(pdu->caData);
            private_chat::getInstance().removeFriendFromView(friendName);
            private_chat::getInstance().flushFriend();
            break;
        }
        case ENUM_MSG_TYPE_CREATE_DIR_RESPOND://创建文件夹回复
        {
            qDebug()<<pdu->caData;
            Network_disk::getInstance().showTips(pdu->caData);
            Network_disk::getInstance().flushFile();
            break;
        }
        case ENUM_MSG_TYPE_FLUSH_FILE_RESPOND://刷新文件回复
        {
            Network_disk::getInstance().flushFileView(pdu);
            break;
        }
        case ENUM_MSG_TYPE_DEL_DIR_RESPOND://删除文件回复
        {
            Network_disk::getInstance().showTips(pdu->caData);
            Network_disk::getInstance().flushFile();
            break;
        }
        case ENUM_MSG_TYPE_RENAME_DIR_RESPOND://重命名回复
        {
            Network_disk::getInstance().showTips(pdu->caData);
            Network_disk::getInstance().flushFile();
            break;
        }
        case ENUM_MSG_TYPE_ENTER_DIR_RESPOND://进入文件夹回复
        {
            // Network_disk::getInstance().showTips(pdu->caData);
            Network_disk::getInstance().flushFileView(pdu);
            break;
        }
        case ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND://上传回复
        {
            qDebug()<<pdu->caData;
            Network_disk::getInstance().showTips(pdu->caData);
            Network_disk::getInstance().flushFile();
            break;
        }
        case ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND://下载回复
        {
            qDebug()<<pdu->caData;
            char caFileName[32]={'\0'};
            sscanf(pdu->caData,"%s %lld",caFileName,&Network_disk::getInstance().m_iTotal);
            if(strlen(caFileName)>0&&Network_disk::getInstance().m_iTotal>0)
            {
                Network_disk::getInstance().setDownloadStatus(true);
                 m_file.setFileName(Network_disk::getInstance().getSaveFilePath());
                if(!m_file.open(QIODevice::WriteOnly))
                {
                    //错误
                }
            }
            break;
        }
        default:
            break;
        }
        free(pdu);
        pdu=NULL;
    }
    else
    {
        QByteArray buffer=m_tcpsocket.readAll();
        m_file.write(buffer);
        Network_disk::getInstance().m_iRecved+=buffer.size();
        qDebug()<<Network_disk::getInstance().m_iRecved<<Network_disk::getInstance().m_iTotal;
        if(Network_disk::getInstance().m_iTotal==Network_disk::getInstance().m_iRecved)
        {
            m_file.close();
            Network_disk::getInstance().m_iRecved=0;
            Network_disk::getInstance().m_iTotal=0;
            Network_disk::getInstance().setDownloadStatus(false);
            Network_disk::getInstance().showTips("下载成功");
        }
        else if(Network_disk::getInstance().m_iTotal<Network_disk::getInstance().m_iRecved)
        {
            Network_disk::getInstance().showTips("download false");
        }
    }
}

void CloudLinkerClient::clinetOffline()
{
    qDebug()<<"disconnected";
}
// void CloudLinkerClinet::on_pushButton_clicked()
// {
//     QString strtest=ui->lineEdit->text();
//     qDebug()<<strtest;
//     PDU*pdu=mkPDU(strtest.size()+1);
//     pdu->uiMsgType=ENUM_MSG_TYPE_MIN;
//     strncpy(reinterpret_cast<char*>(pdu->caMsg),strtest.toStdString().c_str(),strtest.size());
//     m_tcpsocket.write(reinterpret_cast<char*>(pdu),pdu->uiPDULen);
//     free(pdu);
//     pdu=NULL;
// }


void CloudLinkerClient::on_login_pushButton_clicked()
{
    QString userName=ui->user_lineEdit->text();
    QString password=ui->passward_lineEdit->text();
    if(userName.isEmpty()&&password.isEmpty())
    {
        ui->Tips->textlable->setText("请输入用户名和密码");
        ui->Tips->animationStart();
    }
    else if(userName.isEmpty())
    {
        ui->Tips->textlable->setText("请输入用户名");
        ui->Tips->animationStart();
    }
    else if(password.isEmpty())
    {
        ui->Tips->textlable->setText("请输入密码");
        ui->Tips->animationStart();
    }
    else//执行登录逻辑
    {
        m_strUserName=userName;
        PDU*pdu=mkPDU(0);
        pdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_REQUEST;
        strncpy(pdu->caData,userName.toStdString().c_str(),32);
        strncpy(pdu->caData+32,password.toStdString().c_str(),32);

        m_tcpsocket.write(reinterpret_cast<char*>(pdu),pdu->uiPDULen);
        free(pdu);
        pdu=NULL;
    }
}


void CloudLinkerClient::on_return_login_pushButton_clicked()
{

    pageSwitch(1);
}


void CloudLinkerClient::on_go_regist_pushButton_clicked()
{
    qDebug()<<ui->stackedWidget->currentIndex();

    pageSwitch(0);
}


void CloudLinkerClient::on_regist_pushButton_clicked()
{
    QString userName=ui->rigist_uesrname_lineEdit->text();
    QString password=ui->regist_passward_lineEdit->text();
    QString password_ag=ui->regist_passward_ag_lineEdit->text();

    if(userName.isEmpty()&&password.isEmpty())
    {
        ui->Tips_regist->textlable->setText("请输入用户名和密码");
        ui->Tips_regist->animationStart();
    }
    else if(userName.isEmpty())
    {
        ui->Tips_regist->textlable->setText("请输入用户名");
        ui->Tips_regist->animationStart();
    }
    else if(password.isEmpty())
    {
        ui->Tips_regist->textlable->setText("请输入密码");
        ui->Tips_regist->animationStart();
    }
    else if(password!=password_ag)
    {
        ui->Tips_regist->textlable->setText("两次输入的密码不一致");
        ui->Tips_regist->animationStart();
    }
    else//执行注册逻辑
    {
        PDU*pdu=mkPDU(0);
        pdu->uiMsgType=ENUM_MSG_TYPE_REGIST_REQUEST;
        strncpy(pdu->caData,userName.toStdString().c_str(),32);
        strncpy(pdu->caData+32,password.toStdString().c_str(),32);

        m_tcpsocket.write(reinterpret_cast<char*>(pdu),pdu->uiPDULen);
        free(pdu);
        pdu=NULL;
    }
}

