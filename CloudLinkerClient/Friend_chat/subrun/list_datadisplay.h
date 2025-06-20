#ifndef LIST_DATADISPLAY_H
#define LIST_DATADISPLAY_H

#include <QListWidget>

class ItemWdgt_DataDisplay;

class List_DataDisplay : public QListWidget
{
    Q_OBJECT
public:
    List_DataDisplay(QWidget *parent = nullptr);
    ~List_DataDisplay();

    //【接口】添加数据项
    void AddDataDisplay(const int role, const QString& text_content);

private:
    std::vector<std::shared_ptr<ItemWdgt_DataDisplay>> _itemwdgt_datadisplay_vec;

    //添加时间项
    void AddTimeItem(const QString& str_curr_time);
    //添加内容项
    void AddTextItem(const QString& str_curr_time, const int role, const QString& text_content);
    //添加一个列表项
    void AddOneListItem(std::shared_ptr<ItemWdgt_DataDisplay> itemwdgt_datadisplay, const QSize& itemwdgt_size);

};

#endif // LIST_DATADISPLAY_H
