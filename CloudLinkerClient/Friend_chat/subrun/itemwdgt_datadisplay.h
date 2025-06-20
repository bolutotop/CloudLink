#ifndef ITEMWDGT_DATADISPLAY_H
#define ITEMWDGT_DATADISPLAY_H

#include <QWidget>
#include "Friend_chat/public/global.h"
#include <QLabel>

class ItemWdgt_DataDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit ItemWdgt_DataDisplay(QWidget *parent = nullptr);
    ~ItemWdgt_DataDisplay();

    //【接口】计算并获取项框尺寸(供原始项设置) (计算文本框的尺寸和消息气泡的尺寸)
    QSize GetTextItemSize(const QString& text_content);

    //【接口】设置项框信息
    void SetInfo(const QString& text_content, const QString&  str_time, const QSize& itemwdgt_size, const int role);

    //获取文本内容
    QString GetTextContent() const { return _text_content; }
    //获取时间点
    QString GetTime() const { return _str_time; }
    //获取角色类型
    ChatRole GetChatRole() const { return _role; }

protected:
    //重绘
    void paintEvent(QPaintEvent *event) override;

private:
    QString _text_content;
    QString _str_time; //存放本次消息真正的时间点
    ChatRole _role;

    QSize _itemwdgt_size; //无用
    QString _str_curr_time; //仅用于重绘

    QPixmap _self_pic; //头像
    QPixmap _peer_pic;

    bool _is_load; //是否支持加载动画。初始`true`
    QMovie* _load_anime; //加载动画
    QLabel* _lb_load_anime; //放置加载动画的标签

    QRect _right_bubble_rect; //气泡框 右侧-己方
    QRect _left_bubble_rect;

    int m_kuangWidth;
    int m_textWidth;
    int m_spaceWid;
    int m_lineHeight;

    QRect m_iconLeftRect;
    QRect m_iconRightRect;
    QRect m_sanjiaoLeftRect;
    QRect m_sanjiaoRightRect;
//    QRect m_kuangLeftRect; //_left_bubble_rect
//    QRect m_kuangRightRect; //_right_bubble_rect
    QRect m_textLeftRect;
    QRect m_textRightRect;

    //初始化
    void initItemWdgt_DataDisplay();

public:
    //控制加载动画
    void LoadAnime(const bool is_load);
private:
    // 获取文本的实际宽度和高度，并进行换行处理
    QSize getRealString(const QString& text_content);


signals:

};

#endif // ITEMWDGT_DATADISPLAY_H
