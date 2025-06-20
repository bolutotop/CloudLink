#ifndef PRIVATE_CHAT_H
#define PRIVATE_CHAT_H

#include <QWidget>
#include"protocol.h"
#include"ElaContentDialog.h"
namespace Ui {
class private_chat;
}

class private_chat : public QWidget
{
    Q_OBJECT

public:
    explicit private_chat(QWidget *parent = nullptr);
    ~private_chat();
    static private_chat&getInstance();
    void flushFriend();
    void updateFriend(PDU*pdu);
    QString& getFriendName();
    void resvMsg(PDU*pdu);
    void showTipsAddFriend(const char*Tips);
    void removeFriendFromView(const QString &friendName);
public slots:
    void onFriendListViewClicked(const QModelIndex &index);
private slots:
    void on_add_friend_pushButton_clicked();
    void showFriendListContextMenu(const QPoint &pos);
    void deleteSelectedFriend(const QModelIndex &index);
private:
    Ui::private_chat *ui;
    QString friendName;
};

#endif // PRIVATE_CHAT_H
