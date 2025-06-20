#include "chat_box.h"
#include "ui_chat_box.h"
#include "public/global.h"
#include "subrun/itemwdgt_datadisplay.h"
#include<QListWidgetItem>

#include"Friend_chat/private_chat.h"
#include"cloudlinkerclient.h"

Chat_box::Chat_box(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Chat_box)
{
    ui->setupUi(this);
}

Chat_box::~Chat_box()
{
    delete ui;
}

void Chat_box::sendMsg(const QString &text_content)
{
    if (text_content.isEmpty()) { qDebug() << "文本内容为空！"; return; }
    ui->txtE_TextContent->setPlainText("");

    qDebug() << "text_content: " << text_content;

    //2.添加数据项
    ui->list_DataDisplay->AddDataDisplay(static_cast<int>(ChatRole::Role_Self), text_content);
}

void Chat_box::resvMsg(PDU*pdu)
{
    qDebug()<<(char*)pdu->caMsg;
    //2.添加数据项
    ui->list_DataDisplay->AddDataDisplay(static_cast<int>(ChatRole::Role_Peer),(char*)pdu->caMsg);
}

void Chat_box::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    //遍历列表项，逐个获取项和项框，重新加载
    for (int i = 0; i < ui->list_DataDisplay->count(); ++i) {
        //1.获取项和项框
        QListWidgetItem* item = ui->list_DataDisplay->item(i);
        ItemWdgt_DataDisplay* itemwdgt_datadisplay = qobject_cast<ItemWdgt_DataDisplay*>(ui->list_DataDisplay->itemWidget(item));

        //2.重载项框的样式
        //2.1 获取项框中的原数据
        QString text_content = itemwdgt_datadisplay->GetTextContent();
        QString str_time = itemwdgt_datadisplay->GetTime();
        ChatRole role = itemwdgt_datadisplay->GetChatRole();

        //2.2 结合新数据(重新设置尺寸等)，重新设置项框样式
        itemwdgt_datadisplay->setFixedWidth(ui->list_DataDisplay->width());
        QSize itemwdgt_size = itemwdgt_datadisplay->GetTextItemSize(text_content);
        itemwdgt_datadisplay->SetInfo(text_content, str_time, itemwdgt_size, role);

        //3.设置原始项大小，添加项框
        item->setSizeHint(itemwdgt_size);
        ui->list_DataDisplay->setItemWidget(item, itemwdgt_datadisplay);
    }
}

void Chat_box::on_pbtn_Send_clicked()
{
    //1.获取内容，清空文本框
    QString text_content = ui->txtE_TextContent->toPlainText();
    sendMsg(text_content);
    PDU*pdu=mkPDU(text_content.size()+1);
    pdu->uiMsgType=ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST;
    strncpy(pdu->caData,private_chat::getInstance().getFriendName().toStdString().c_str(),private_chat::getInstance().getFriendName().size());
    strncpy(reinterpret_cast<char*>(pdu->caMsg),text_content.toStdString().c_str(),text_content.size());

    CloudLinkerClient::getInstance().getUserSocket().write(reinterpret_cast<char*>(pdu),pdu->uiPDULen);

    free(pdu);
    pdu=NULL;
}

void Chat_box::on_pbtn_Recv_clicked()
{
    //1.获取内容，清空文本框
    QString text_content = ui->txtE_TextContent->toPlainText();
}
