#include "list_datadisplay.h"
#include <QDateTime>
#include "itemwdgt_datadisplay.h"
#include <QDebug>
#include "Friend_chat/public/global.h"
#include <QTimer>

List_DataDisplay::List_DataDisplay(QWidget *parent)
    : QListWidget(parent)
{
    //禁用水平滚动条
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //禁用垂直滚动条
//    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

List_DataDisplay::~List_DataDisplay()
{

}

//【接口】添加数据项
void List_DataDisplay::AddDataDisplay(const int role, const QString &text_content)
{

    //1.获取当前时间戳
    QString str_curr_time = QString::number(QDateTime::currentDateTime().toSecsSinceEpoch());

    //2.添加时间项
    AddTimeItem(str_curr_time);

    //3.添加内容项
    qDebug() << "role: " << role << ", text_content: " << text_content;
    AddTextItem(str_curr_time, role, text_content);

    //4.设置当前行
//    this->setCurrentRow(this->count() - 1);

}

//添加时间项
void List_DataDisplay::AddTimeItem(const QString &str_curr_time)
{
    //1.判断是否插入时间项
    bool is_time = false;

    //1.1 无列表项时，初始插入
    if (this->count() == 0) {
        is_time = true;
    }
    //1.2 当前时间与列表的最后一项时间差超限，插入
    else {
        QListWidgetItem* last_item = this->item(this->count() - 1);

        QWidget* last_itemwdgt = this->itemWidget(last_item);

        ItemWdgt_DataDisplay* last_itemwdgt_datadisplay = qobject_cast<ItemWdgt_DataDisplay*>(last_itemwdgt);
        int last_time = last_itemwdgt_datadisplay->GetTime().toInt();
        int curr_time = str_curr_time.toInt();

        qDebug() << "时间差：" << curr_time - last_time << " 秒！";
        is_time = ((curr_time - last_time) > TimeInterval) ? true : false;
    }

    //2.插入时间项
    if (!is_time) { qDebug() << "无需插入时间项"; return; }

    //2.1 创建项框，设置大小和数据
    std::shared_ptr<ItemWdgt_DataDisplay> itemwdgt_datadisplay = std::make_shared<ItemWdgt_DataDisplay>();
    QSize itemwdgt_size = QSize(this->width(), TimeItem_Height);

    itemwdgt_datadisplay->resize(itemwdgt_size);
    itemwdgt_datadisplay->SetInfo(str_curr_time, str_curr_time, itemwdgt_size, static_cast<int>(ChatRole::Role_Time));

    //2.2 添加项框
    AddOneListItem(itemwdgt_datadisplay, itemwdgt_size);
}

//添加内容项
void List_DataDisplay::AddTextItem(const QString &str_curr_time, const int role, const QString &text_content)
{

    //1.创建项框，设置大小和数据
    std::shared_ptr<ItemWdgt_DataDisplay> itemwdgt_datadisplay = std::make_shared<ItemWdgt_DataDisplay>();
    itemwdgt_datadisplay->setFixedWidth(this->width());
    QSize itemwdgt_size = itemwdgt_datadisplay->GetTextItemSize(text_content);

    itemwdgt_datadisplay->SetInfo(text_content, str_curr_time, itemwdgt_size, ChatRole(role));

    //2.添加项框
    AddOneListItem(itemwdgt_datadisplay, itemwdgt_size);

    //[violet-test]【使用弱指针！！！】延时测试
    std::weak_ptr<ItemWdgt_DataDisplay> weak_itemwdgt = itemwdgt_datadisplay;
    QTimer::singleShot(500, this, [weak_itemwdgt]() {
        std::shared_ptr<ItemWdgt_DataDisplay> itemwdgt_datadisplay = weak_itemwdgt.lock();
        if (itemwdgt_datadisplay) {
            itemwdgt_datadisplay->LoadAnime(false);
        }
    });
#if 0
    ///【禁用】在窗口关闭时，itemwdgt_datadisplay已被销毁，导致崩溃
    ///[violet-test]延时测试
    QTimer::singleShot(2000, this, [itemwdgt_datadisplay]() {
        itemwdgt_datadisplay->LoadAnime(false);
    });
#endif
}

//添加一个列表项
void List_DataDisplay::AddOneListItem(std::shared_ptr<ItemWdgt_DataDisplay> itemwdgt_datadisplay, const QSize& itemwdgt_size)
{
    //1.创建原始项，设置大小
    QListWidgetItem* item = new QListWidgetItem();
    item->setSizeHint(itemwdgt_size);

    //2.添加原始项，设置项框
    this->addItem(item);
    this->setItemWidget(item, itemwdgt_datadisplay.get());

    //3.加入管理
    _itemwdgt_datadisplay_vec.push_back(itemwdgt_datadisplay);

    // 自动滚动到列表的最后一项
    this->scrollToItem(item, QAbstractItemView::EnsureVisible);
}
