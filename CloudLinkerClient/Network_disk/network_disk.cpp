#include "network_disk.h"
#include "ui_network_disk.h"
#include"cloudlinkerclient.h"
#include"protocol.h"
#include"T_TableViewModel.h"
#include"ElaMenu.h"

#include<QFileDialog>
#include<QInputDialog>
#include<QDialogButtonBox>
#include<QFile>
#include<QFileDialog>

Network_disk::Network_disk(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Network_disk)
{
    ui->setupUi(this);

    m_pTimer=new QTimer;
    m_bDownload=false;
    // ElaScrollBar* tableViewFloatScrollBar = new ElaScrollBar(_tableView->verticalScrollBar(), _tableView);
    // tableViewFloatScrollBar->setIsAnimation(true);
    QFont tableHeaderFont = ui->tableView->horizontalHeader()->font();
    tableHeaderFont.setPixelSize(16);
    ui->tableView->horizontalHeader()->setFont(tableHeaderFont);

    ui->tableView->setModel(&T_TableViewModel::getInstance());
    // ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setIconSize(QSize(38, 38));
    ui->tableView->verticalHeader()->setHidden(true);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    // // ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    // ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    // ui->tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    // ui->tableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);



    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->horizontalHeader()->setMinimumSectionSize(55);
    ui->tableView->verticalHeader()->setMinimumSectionSize(46);

    //设置菜单
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableView, &ElaTableView::tableViewShow, this, [=]() {
        ui->tableView->setColumnWidth(0, 55);
        ui->tableView->setColumnWidth(1, 380);
        ui->tableView->setColumnWidth(2, 250);
        ui->tableView->setColumnWidth(3, 210);
    });

    ui->upload_pushButton->setToolTip("上传");
    ui->back_pushButton->setToolTip("返回");
    ui->createDir_pushButton->setToolTip("创建文件夹");
    ui->download_pushButton->setToolTip("下载");
    ui->shared_pushButton->setToolTip("分享");
    ui->flush_pushButton->setToolTip("刷新");
    //右击菜单栏连接
    connect(ui->tableView, &ElaTableView::customContextMenuRequested,
            this, &Network_disk::showContextMenu);
    //获取当前行信息连接
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &Network_disk::handleRowSelection);
    //双击
    connect(ui->tableView, &QTableView::doubleClicked, this, &Network_disk::enterDir);

    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(uploadFileDate()));
}

Network_disk::~Network_disk()
{
    delete ui;
}

Network_disk &Network_disk::getInstance()
{
    static Network_disk instance;
    return instance;
}

void Network_disk::flushFile()
{
    QString strCurPath=CloudLinkerClient::getInstance().getCurPath();
    PDU*pdu=mkPDU(strCurPath.size()+1);
    qDebug()<<strCurPath;
    pdu->uiMsgType=ENUM_MSG_TYPE_FLUSH_FILE_REQUEST;
    strncpy(reinterpret_cast<char*>(pdu->caMsg),strCurPath.toStdString().c_str(),strCurPath.size());

    CloudLinkerClient::getInstance().getUserSocket().write(reinterpret_cast<char*>(pdu),pdu->uiPDULen);//发送
    free(pdu);
    pdu=NULL;
}

void Network_disk::flushFileView(PDU *pdu)
{
    T_TableViewModel::getInstance().clearView();
    int iCount=pdu->uiMsgLen/sizeof(FileInfo);
    FileInfo*pFileInfo=NULL;
    QString fileName;//文件名
    QString dirType;//文件夹类型
    QString fileType;//文件类型
    QString nothing;//文件夹大小无
    QIcon dirIcon;//文件夹图标
    QIcon fileIcon;//文件图标

    for(int i=0;i<iCount;i++)
    {
        pFileInfo=(FileInfo*)(pdu->caMsg)+i;
        qDebug()<<pFileInfo->caFileName<<" "<<pFileInfo->iFileType<<" "<<pFileInfo->suffix;

        fileName=pFileInfo->caFileName;
        dirType="文件夹";
        fileType=QString("%1文件").arg(pFileInfo->suffix);
        nothing="-";
        dirIcon=QIcon(":/pic/dir.ico");
        fileIcon=QIcon(":/pic/reg.png");

        if(pFileInfo->iFileType==0)
        {
            T_TableViewModel::getInstance().addView(fileName,nothing,dirType,dirIcon);
        }
        else
        {
            QString fileSize=formatFileSize(pFileInfo->fileSizeInBytes);
            T_TableViewModel::getInstance().addView(fileName,fileSize,fileType,fileIcon);
        }
    }
}
QString Network_disk::formatFileSize(qint64 bytes)
{
    if (bytes < 0)
        return QString();

    double size = bytes;
    QStringList units = {"B", "KB", "MB", "GB"};
    int unitIndex = 0;

    while (size >= 1024 && unitIndex < units.size() - 1) {
        size /= 1024;
        unitIndex++;
    }

    return QString("%1 %2").arg(QString::number(size, 'f', 1)).arg(units[unitIndex]);
}

void Network_disk::showTips(const char *tips)
{
    ui->Tips_widget->textlable->setText(tips);
    ui->Tips_widget->animationStart();
}

void Network_disk::setDownloadStatus(bool status)
{
    m_bDownload=status;
}

bool Network_disk::getDownloadStatus()
{
    return m_bDownload;
}

QString Network_disk::getSaveFilePath()
{
    return m_strSaveFilePath;
}

void Network_disk::downloadFile()
{
    QString fileName=m_fileName;
    QString CurPath=CloudLinkerClient::getInstance().getCurPath();
    PDU*pdu=mkPDU(CurPath.size()+1);
    pdu->uiMsgType=ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST;
    strcpy(pdu->caData,fileName.toStdString().c_str());
    memcpy(pdu->caMsg,CurPath.toStdString().c_str(),CurPath.size());

    QString strSaveFilePath=QFileDialog::getSaveFileName();
    if(!strSaveFilePath.isEmpty())
    {
        m_strSaveFilePath=strSaveFilePath;
    }
    else
    {
        strSaveFilePath.clear();
        return;
    }

    CloudLinkerClient::getInstance().getUserSocket().write(reinterpret_cast<char*>(pdu),pdu->uiPDULen);//发送
    free(pdu);
    pdu=NULL;
}

void Network_disk::on_createDir_pushButton_clicked()
{
    QInputDialog dialog(this);
    // 设置无边框窗口标志
    dialog.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    dialog.setWindowTitle(tr("创建文件夹"));
    dialog.setInputMode(QInputDialog::InputMode::TextInput);
    dialog.setTextEchoMode(QLineEdit::Normal);
    dialog.setLabelText(tr("请输入文件夹名称:"));
    dialog.setOkButtonText(QObject::tr("确定"));
    dialog.setCancelButtonText(QObject::tr("取消"));
    dialog.setFixedSize(350, 250);
    dialog.setTextValue("");

    // 无边框样式
    QString style = "QInputDialog{"
                    "    background-color:rgb(255,255,255);"
                    "    border: 2px solid rgb(26,179,148);" // 添加自定义边框
                    "    border-radius: 8px;"                // 圆角效果
                    "}"
                    "QPushButton{"
                    "    background:rgb(26,179,148);"
                    "    color:rgb(255,255,255);"
                    "    border-radius:3px;"
                    "    min-height:30px;"
                    "    min-width:60px;"
                    "    font:13px \"Microsoft YaHei\";"
                    "}"
                    "QPushButton:hover{"
                    "    background:rgb(24,166,137);"
                    "}"
                    "QPushButton:pressed{"
                    "    background:rgb(32,75,148);"
                    "}"
                    "QLineEdit{"
                    "    border:2px solid rgb(229,230,231);"
                    "    padding:4px;"
                    "    padding-left:10px;"
                    "    border-radius:3px;"
                    "    color:rgb(105,105,105);"
                    "    font:13px \"Microsoft YaHei\";"
                    "}"
                    "QLineEdit:focus{"
                    "    border:2px solid rgb(26,179,148);"
                    "}"
                    "QLineEdit:disabled{"
                    "    background-color:rgb(238,238,238);"
                    "}"
                    "QLabel{"
                    "    color:rgb(85,85,85);"
                    "    font:12px \"Microsoft YaHei\";"
                    "    font-weight:bold;"
                    "}"
                    "QInputDialog > QLabel#qt_msgbox_label{"
                    "    qproperty-alignment: AlignCenter;"  // 标题居中
                    "}";
    dialog.setStyleSheet(style);

    if (dialog.exec() == QDialog::Accepted) {
        QString dirName = dialog.textValue().trimmed();
        if (!dirName.isEmpty()) {

            QString userName=CloudLinkerClient::getInstance().getUserName();
            QString curPath=CloudLinkerClient::getInstance().getCurPath();
            PDU*pdu=mkPDU(curPath.size()+1);
            pdu->uiMsgType=ENUM_MSG_TYPE_CREATE_DIR_REQUEST;//创建文件夹请求

            strncpy(pdu->caData,userName.toStdString().c_str(),userName.size());//1,自己的名字 前32
            strncpy(pdu->caData+32,dirName.toStdString().c_str(),dirName.size());//2，新建的文件夹 后32
            strncpy(reinterpret_cast<char*>(pdu->caMsg),curPath.toStdString().c_str(),curPath.size()); //放在弹性数组里

            CloudLinkerClient::getInstance().getUserSocket().write(reinterpret_cast<char*>(pdu),pdu->uiPDULen);//发送
            free(pdu);
            pdu=NULL;
        }
    }

}

void Network_disk::on_flush_pushButton_clicked()
{
    flushFile();
}

void Network_disk::showContextMenu(const QPoint &pos)
{
    QModelIndex index = ui->tableView->indexAt(pos);
    if (!index.isValid()) return;  // 确保点击位置有效
    // 确保选中当前右键点击的行
    ui->tableView->selectRow(index.row());


    ElaMenu menu(this);
    QAction *deleteAction = menu.addAction("删除");
    QAction *renameAction = menu.addAction("重命名");
    if(m_fileType!="文件夹")
    {
        QAction *downloadAction = menu.addAction("下载");
        connect(downloadAction, &QAction::triggered, [this]() {
            qDebug()<<m_fileName<<m_fileType;
                downloadFile();
        });
    }
    // 连接删除动作
    connect(deleteAction, &QAction::triggered, [this]() { 
        qDebug()<<"当前删除的文件为 :"<<m_fileName;
        QString fileName=m_fileName;
        QString strCurPath =CloudLinkerClient::getInstance().getCurPath();
        PDU*pdu=mkPDU(strCurPath.size()+1);
        pdu->uiMsgType=ENUM_MSG_TYPE_DEL_DIR_REQUEST;

        strncpy(pdu->caData,fileName.toStdString().c_str(),fileName.size());
        strncpy(reinterpret_cast<char*>(pdu->caMsg),strCurPath.toStdString().c_str(),strCurPath.size());

        CloudLinkerClient::getInstance().getUserSocket().write(reinterpret_cast<char*>(pdu),pdu->uiPDULen);//发送
        free(pdu);
        pdu=NULL;
    });
    //连接重命名动作
    connect(renameAction, &QAction::triggered, [this]() {
        QInputDialog dialog(this);
        // 设置无边框窗口标志
        dialog.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
        dialog.setWindowTitle(tr("创建文件夹"));
        dialog.setInputMode(QInputDialog::InputMode::TextInput);
        dialog.setTextEchoMode(QLineEdit::Normal);
        dialog.setLabelText(tr("请输入文件夹名称:"));
        dialog.setOkButtonText(QObject::tr("确定"));
        dialog.setCancelButtonText(QObject::tr("取消"));
        dialog.setFixedSize(350, 250);
        dialog.setTextValue("");

        // 无边框样式
        QString style = "QInputDialog{"
                        "    background-color:rgb(255,255,255);"
                        "    border: 2px solid rgb(26,179,148);" // 添加自定义边框
                        "    border-radius: 8px;"                // 圆角效果
                        "}"
                        "QPushButton{"
                        "    background:rgb(26,179,148);"
                        "    color:rgb(255,255,255);"
                        "    border-radius:3px;"
                        "    min-height:30px;"
                        "    min-width:60px;"
                        "    font:13px \"Microsoft YaHei\";"
                        "}"
                        "QPushButton:hover{"
                        "    background:rgb(24,166,137);"
                        "}"
                        "QPushButton:pressed{"
                        "    background:rgb(32,75,148);"
                        "}"
                        "QLineEdit{"
                        "    border:2px solid rgb(229,230,231);"
                        "    padding:4px;"
                        "    padding-left:10px;"
                        "    border-radius:3px;"
                        "    color:rgb(105,105,105);"
                        "    font:13px \"Microsoft YaHei\";"
                        "}"
                        "QLineEdit:focus{"
                        "    border:2px solid rgb(26,179,148);"
                        "}"
                        "QLineEdit:disabled{"
                        "    background-color:rgb(238,238,238);"
                        "}"
                        "QLabel{"
                        "    color:rgb(85,85,85);"
                        "    font:12px \"Microsoft YaHei\";"
                        "    font-weight:bold;"
                        "}"
                        "QInputDialog > QLabel#qt_msgbox_label{"
                        "    qproperty-alignment: AlignCenter;"  // 标题居中
                        "}";
        dialog.setStyleSheet(style);

        if (dialog.exec() == QDialog::Accepted) {
            QString newName = dialog.textValue().trimmed();
            if (!newName.isEmpty()) {

                qDebug()<<"当前重命名的文件为 :"<<m_fileName<<"新名字为"<<newName;
                QString fileName=m_fileName;
                QString strCurPath =CloudLinkerClient::getInstance().getCurPath();
                PDU*pdu=mkPDU(strCurPath.size()+1);
                pdu->uiMsgType=ENUM_MSG_TYPE_RENAME_DIR_REQUEST;//修改

                strncpy(pdu->caData,fileName.toStdString().c_str(),fileName.size());//前32原文件名
                strncpy(pdu->caData+32,newName.toStdString().c_str(),newName.size());//后32为新文件名

                strncpy(reinterpret_cast<char*>(pdu->caMsg),strCurPath.toStdString().c_str(),strCurPath.size());

                CloudLinkerClient::getInstance().getUserSocket().write(reinterpret_cast<char*>(pdu),pdu->uiPDULen);//发送
                free(pdu);
                pdu=NULL;
            }
        }
    });
    menu.exec(ui->tableView->viewport()->mapToGlobal(pos));//菜单事件循环
}

void Network_disk::handleRowSelection(const QModelIndex &current)
{
    if (current.isValid()) {
        int selectedRow = current.row();//当前行

        // 通过模型单例获取文件名
        QString fileName = T_TableViewModel::getInstance().getFileName(selectedRow);
        QString fileType = T_TableViewModel::getInstance().getFiletype(selectedRow);
        m_fileName=fileName;
        m_fileType=fileType;
    }
}

void Network_disk::enterDir(const QModelIndex &current)
{

    if(m_fileType!="文件夹")
    {
        downloadFile();
        return;
    }
    QString fileName=m_fileName;
    QString strCurPath =CloudLinkerClient::getInstance().getCurPath();
    QString eryPath=QString("%1/%2").arg(strCurPath).arg(fileName);
    qDebug()<<"当前进入的文件夹路径为 :"<<eryPath;
    CloudLinkerClient::getInstance().updatePath(eryPath.toStdString().c_str());
    PDU*pdu=mkPDU(strCurPath.size()+1);
    pdu->uiMsgType=ENUM_MSG_TYPE_ENTER_DIR_REQUEST;//进入文件夹请求

    strncpy(pdu->caData,fileName.toStdString().c_str(),fileName.size());
    strncpy(reinterpret_cast<char*>(pdu->caMsg),strCurPath.toStdString().c_str(),strCurPath.size());

    CloudLinkerClient::getInstance().getUserSocket().write(reinterpret_cast<char*>(pdu),pdu->uiPDULen);//发送
    free(pdu);
    pdu=NULL;
}


void Network_disk::on_back_pushButton_clicked()
{
    QString strCurPath =CloudLinkerClient::getInstance().getCurPath();//当前目录
         //返回的上一级目录
    QString userName=CloudLinkerClient::getInstance().getUserName();
    QString rootPath=QString("./%1").arg(userName);
    if(strCurPath==rootPath)
    {
        return;
    }

    int lastSlash = strCurPath.lastIndexOf('/');
    QString curPath=strCurPath.left(lastSlash);

    qDebug()<<"当前返回的文件夹路径为 :"<<curPath;
    CloudLinkerClient::getInstance().updatePath(curPath.toStdString().c_str());
    PDU*pdu=mkPDU(curPath.size()+1);
    pdu->uiMsgType=ENUM_MSG_TYPE_FLUSH_FILE_REQUEST;

    strncpy(reinterpret_cast<char*>(pdu->caMsg),curPath.toStdString().c_str(),curPath.size());

    CloudLinkerClient::getInstance().getUserSocket().write(reinterpret_cast<char*>(pdu),pdu->uiPDULen);//发送
    free(pdu);
    pdu=NULL;
}


void Network_disk::on_upload_pushButton_clicked()
{
    QString strCurPath =CloudLinkerClient::getInstance().getCurPath();//当前目录
    m_uploadFilePath=QFileDialog::getOpenFileName();//打开文件操作
    if(!m_uploadFilePath.isEmpty())
    {
        //获取文件名字
        int index=m_uploadFilePath.lastIndexOf('/');
        QString strFileName=m_uploadFilePath.right(m_uploadFilePath.size()-index-1);

        qDebug()<<m_uploadFilePath<<strCurPath<<strFileName;

        //获取文件大小
        QFile file(m_uploadFilePath);
        qint64 fileSize=file.size();


        PDU*pdu=mkPDU(strCurPath.size()+1);
        pdu->uiMsgType=ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST;

        memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.size());

        sprintf(pdu->caData,"%s %lld",strFileName.toStdString().c_str(),fileSize);//拼接字符串 (要赋的值，"占位符",填补占位符)

        CloudLinkerClient::getInstance().getUserSocket().write(reinterpret_cast<char*>(pdu),pdu->uiPDULen);//发送
        free(pdu);
        pdu=NULL;

        m_pTimer->start(1000);
    }
}

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


void Network_disk::on_download_pushButton_clicked()
{
    qDebug()<<m_fileName<<m_fileType;
    QString nothing="文件夹";
    if(m_fileType==nothing)
    {
        showTips("请选择文件");
    }
    else if(m_fileName.isEmpty())
    {
        showTips("请选择文件");
    }
    else
    {
        downloadFile();
    }
}

