#ifndef CHAT_BOX_H
#define CHAT_BOX_H

#include <QWidget>
#include"protocol.h"
namespace Ui {
class Chat_box;
}

class Chat_box : public QWidget
{
    Q_OBJECT

public:
    explicit Chat_box(QWidget *parent = nullptr);
    ~Chat_box();
    void sendMsg(const QString&text_content);
    void resvMsg(PDU*pdu);
protected:
    void resizeEvent(QResizeEvent *event) override;
private slots:
    void on_pbtn_Send_clicked();

    void on_pbtn_Recv_clicked();
private:
    Ui::Chat_box *ui;
};

#endif // CHAT_BOX_H
