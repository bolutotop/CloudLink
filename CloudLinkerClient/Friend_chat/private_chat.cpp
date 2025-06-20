#include "private_chat.h"
#include "ui_private_chat.h"
#include "chat_box.h"

#include"cloudlinkerclient.h"
#include"protocol.h"

#include<QStringListModel>
#include<QModelIndex>
#include<QMenu>
#include<QAction>
// #include
private_chat::private_chat(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::private_chat)
{
    ui->setupUi(this);
    ui->friend_listView->setStyleSheet(
        "QListView {"
        "   font-size: 14pt;"    // 字体大小
        "   font-weight: bold;"  // 字体粗细
        "}"
        );

    connect(ui->friend_listView, &QListView::clicked,
            this, &private_chat::onFriendListViewClicked);

    ui->friend_listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->friend_listView, &QListView::customContextMenuRequested,
            this, &private_chat::showFriendListContextMenu);
}

private_chat::~private_chat()
{
    delete ui;
}

private_chat &private_chat::getInstance()
{
    static private_chat instance;
    return instance;
}

void private_chat::flushFriend()
{
    QString strName=CloudLinkerClient::getInstance().getUserName();
    qDebug()<<"刷新好友出发点,客户端为:"<<strName;
    PDU*pdu=mkPDU(0);
    pdu->uiMsgType=ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST;
    strncpy(pdu->caData,strName.toStdString().c_str(),strName.size());

    CloudLinkerClient::getInstance().getUserSocket().write((char*)pdu,pdu->uiPDULen);//发送

    free(pdu);
    pdu=NULL;
}

void private_chat::updateFriend(PDU *pdu)
{
    if(pdu == nullptr) {
        return;
    }

    // 获取当前模型（避免重复创建）
    QStringListModel *model = qobject_cast<QStringListModel*>(ui->friend_listView->model());

    // 如果尚未创建模型，则新建
    if(model == nullptr) {
        model = new QStringListModel(this); // 指定父对象自动回收
        ui->friend_listView->setModel(model);
    }

    const uint count = pdu->uiMsgLen / 32;
    QStringList friendList;

    for(uint i = 0; i < count; i++) {
        // 更安全的数据拷贝方式
        char buffer[33] = {0}; // 确保末尾有终止符
        memcpy(buffer, (char*)(pdu->caMsg) + i * 32, 32);

        // 处理字符编码（确保正确显示中文等特殊字符）
        // 根据实际编码选择其一：
        // QString name = QString::fromUtf8(buffer);   // UTF-8 编码
        QString name = QString::fromLocal8Bit(buffer); // 本地编码
        qDebug()<<"打印好友 : "<<name;
        friendList.append(name.trimmed()); // 移除可能的空格
    }

    // 更新模型数据
    model->setStringList(friendList);

    // 强制刷新视图（某些情况下需要）
    ui->friend_listView->update();
}

QString& private_chat::getFriendName()
{
    return friendName;
}

void private_chat::resvMsg(PDU *pdu)
{
    ui->widget->resvMsg(pdu);
}

void private_chat::showTipsAddFriend(const char *Tips)
{
    ui->Tips_add->textlable->setText(Tips);
    ui->Tips_add->animationStart();
}

void private_chat::removeFriendFromView(const QString &friendName)
{
    QStringListModel *model = qobject_cast<QStringListModel*>(ui->friend_listView->model());
    if (!model) return;

    QStringList friends = model->stringList();
    friends.removeAll(friendName);
    model->setStringList(friends);
}

void private_chat::onFriendListViewClicked(const QModelIndex &index)
{
    if(!index.isValid()) return;

    // 获取选中的好友名称
    QString selectedFriend = index.data().toString();
    qDebug() << "选中的好友:" << selectedFriend;
    ui->friend_label->setText(selectedFriend);
    friendName=selectedFriend;

}

void private_chat::on_add_friend_pushButton_clicked()
{
    QString friendName=ui->add_friend_lineEdit->text();
    QString userName=CloudLinkerClient::getInstance().getUserName();
    qDebug()<<userName<<" 客户端发出添加好友请求 加的好友为: "<<friendName;
    PDU*pdu=mkPDU(0);
    pdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
    strncpy(pdu->caData,userName.toStdString().c_str(),userName.size());
    strncpy(pdu->caData+32,friendName.toStdString().c_str(),friendName.size());

    CloudLinkerClient::getInstance().getUserSocket().write(reinterpret_cast<char*>(pdu),pdu->uiPDULen);
    free(pdu);
    pdu=NULL;
}

void private_chat::showFriendListContextMenu(const QPoint &pos)
{
    QModelIndex index = ui->friend_listView->indexAt(pos);
    if (!index.isValid()) return;  // 确保点击在有效项上

    QMenu contextMenu(this);
    QAction *deleteAction = contextMenu.addAction("删除好友");

    // 连接删除动作
    connect(deleteAction, &QAction::triggered, this, [this, index]() {
        deleteSelectedFriend(index);
    });

    contextMenu.exec(ui->friend_listView->viewport()->mapToGlobal(pos));
}

void private_chat::deleteSelectedFriend(const QModelIndex &index)
{
    QStringListModel *model = qobject_cast<QStringListModel*>(ui->friend_listView->model());
    if (!model) return;

    QString friendName = model->data(index, Qt::DisplayRole).toString();

    // 创建PDU并填充数据
    PDU *pdu = mkPDU(0);  // 根据实际结构创建
    pdu->uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST;
    QString userName = CloudLinkerClient::getInstance().getUserName();

    // 填充删除双方的用户名
    strncpy(pdu->caData, userName.toStdString().c_str(), 32);
    strncpy(pdu->caData + 32, friendName.toStdString().c_str(), 32);

    // 发送给服务器（假设有tcpSocket对象）
    CloudLinkerClient::getInstance().getUserSocket().write((char*)pdu, pdu->uiPDULen);
    free(pdu);
}

