#include "itemwdgt_datadisplay.h"
#include <QMovie>
#include <QPainter>
#include <QDebug>
#include <QDateTime>

ItemWdgt_DataDisplay::ItemWdgt_DataDisplay(QWidget *parent)
    : QWidget(parent),
      _is_load(true)
{
    //初始化
    initItemWdgt_DataDisplay();

}

ItemWdgt_DataDisplay::~ItemWdgt_DataDisplay()
{

}

//【接口】计算并获取项框尺寸(供原始项设置)  (计算文本框的尺寸和消息气泡的尺寸)
QSize ItemWdgt_DataDisplay::GetTextItemSize(const QString &text_content)
{
    //存储信息
    _text_content = text_content;

    // 定义一些常量，用于计算气泡和文本框的尺寸
    int minHei = 30;  // 最小高度
    int iconWH = 60;  // 图标的宽度和高度
    int iconSpaceW = 20;  // 图标之间的间距
    int iconRectW = 5;  // 图标矩形宽度
    int iconTMPH = 10;  // 图标与文本框之间的间距
    int sanJiaoW = 6;  // 三角形的宽度
    int kuangTMP = 20;  // 气泡框的偏移量
    int textSpaceRect = 12;  // 文本框边距的大小

    // 计算文本框可用的宽度，减去左右的图标和空隙
    m_kuangWidth = this->width() - kuangTMP - 2*(iconWH + iconSpaceW + iconRectW);

    // 计算文本区域的宽度
    m_textWidth = m_kuangWidth - 2 * textSpaceRect;

    // 计算右侧空白区域的宽度
    m_spaceWid = this->width() - m_textWidth;

    // 设置图标的矩形区域位置和大小
    m_iconLeftRect = QRect(iconSpaceW, iconTMPH, iconWH, iconWH);  // 左侧图标的位置
    m_iconRightRect = QRect(this->width() - iconSpaceW - iconWH, iconTMPH, iconWH, iconWH);  // 右侧图标的位置

    // 获取实际文本内容的尺寸
    QSize size = getRealString(_text_content);  // 调用getRealString方法获取文本的实际尺寸

    // 计算消息气泡的高度，如果文本的高度小于最小高度，则使用最小高度
    int hei = size.height() < minHei ? minHei : size.height();

    // 计算三角形的位置，三角形通常用于气泡的尾部
    m_sanjiaoLeftRect = QRect(iconWH + iconSpaceW + iconRectW, m_lineHeight / 2, sanJiaoW, hei - m_lineHeight);
    m_sanjiaoRightRect = QRect(this->width() - iconRectW - iconWH - iconSpaceW - sanJiaoW, m_lineHeight / 2, sanJiaoW, hei - m_lineHeight);

    // 根据文本的宽度来调整气泡框的尺寸和位置
    if(size.width() < (m_textWidth + m_spaceWid)) {
        // 文本宽度小于最大宽度，设置气泡框宽度为文本宽度
        _left_bubble_rect.setRect(m_sanjiaoLeftRect.x() + m_sanjiaoLeftRect.width(), m_lineHeight / 4 * 3, size.width() - m_spaceWid + 2 * textSpaceRect, hei - m_lineHeight);
        _right_bubble_rect.setRect(this->width() - size.width() + m_spaceWid - 2 * textSpaceRect - iconWH - iconSpaceW - iconRectW - sanJiaoW, m_lineHeight / 4 * 3, size.width() - m_spaceWid + 2 * textSpaceRect, hei - m_lineHeight);
    } else {
        // 如果文本宽度超出，则固定气泡框宽度
        _left_bubble_rect.setRect(m_sanjiaoLeftRect.x() + m_sanjiaoLeftRect.width(), m_lineHeight / 4 * 3, m_kuangWidth, hei - m_lineHeight);
        _right_bubble_rect.setRect(iconWH + kuangTMP + iconSpaceW + iconRectW - sanJiaoW, m_lineHeight / 4 * 3, m_kuangWidth, hei - m_lineHeight);
    }

    // 设置文本显示区域的位置，确保文本不与边框重叠
    m_textLeftRect.setRect(_left_bubble_rect.x() + textSpaceRect, _left_bubble_rect.y() + iconTMPH, _left_bubble_rect.width() - 2 * textSpaceRect, _left_bubble_rect.height() - 2 * iconTMPH);
    m_textRightRect.setRect(_right_bubble_rect.x() + textSpaceRect, _right_bubble_rect.y() + iconTMPH, _right_bubble_rect.width() - 2 * textSpaceRect, _right_bubble_rect.height() - 2 * iconTMPH);

    // 返回最终计算的尺寸，宽度为文本宽度，高度为气泡的高度
    return QSize(size.width(), hei);
}

//【接口】设置项框信息
void ItemWdgt_DataDisplay::SetInfo(const QString &text_content, const QString &str_time, const QSize &itemwdgt_size, const int role)
{
    //存储信息
    _text_content = text_content;
    _str_time = str_time;
    _itemwdgt_size = itemwdgt_size;
    _role = ChatRole(role);

    //获取当前时间，作为时间的绘制文本
    _str_curr_time = QDateTime::fromSecsSinceEpoch(str_time.toInt()).toString("hh:mm");

    //设置加载动画：仅在支持动画时加载，仅己方需要，对方不需要
    if (_role == ChatRole::Role_Self) LoadAnime(_is_load);

    //触发重绘
    this->update();
}

//重绘
void ItemWdgt_DataDisplay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);  // 忽略事件参数，因为在这个函数中没有使用它

    // 创建QPainter对象，初始化用于绘制
    QPainter painter(this);
    // 设置抗锯齿和图像平滑缩放
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    // 设置画笔颜色为无笔画
    painter.setPen(Qt::NoPen);
    // 设置画刷为灰色
    painter.setBrush(QBrush(Qt::gray));

    // 根据用户类型进行不同的绘制
    switch (_role) {
    case ChatRole::Role_Peer: { // 如果是对方的消息
        // 绘制左侧头像
//        painter.drawRoundedRect(m_iconLeftRect,m_iconLeftRect.width(),m_iconLeftRect.height());
        painter.drawPixmap(m_iconLeftRect, _peer_pic);

        // 绘制带边框的气泡框
        QColor col_KuangB(234, 234, 234);  // 设置边框颜色为浅灰色
        painter.setBrush(QBrush(col_KuangB));
        painter.drawRoundedRect(_left_bubble_rect.x() - 1, _left_bubble_rect.y() - 1,
            _left_bubble_rect.width() + 2, _left_bubble_rect.height() + 2, 4, 4);  // 绘制带边框的矩形

        // 绘制气泡框
        QColor col_Kuang(255, 255, 255);  // 设置气泡框颜色为白色
        painter.setBrush(QBrush(col_Kuang));
        painter.drawRoundedRect(_left_bubble_rect, 4, 4);  // 绘制气泡框

        // 绘制气泡的三角形
        QPointF points[3] = {
            QPointF(m_sanjiaoLeftRect.x(), 30),
            QPointF(m_sanjiaoLeftRect.x() + m_sanjiaoLeftRect.width(), 25),
            QPointF(m_sanjiaoLeftRect.x() + m_sanjiaoLeftRect.width(), 35),
        };
        QPen pen;  // 创建画笔
        pen.setColor(col_Kuang);  // 设置画笔颜色为气泡框的颜色
        painter.setPen(pen);  // 设置画笔
        painter.drawPolygon(points, 3);  // 绘制三角形

        // 绘制三角形的边框
        QPen penSanJiaoBian;
        penSanJiaoBian.setColor(col_KuangB);  // 设置边框颜色为浅灰色
        painter.setPen(penSanJiaoBian);
        painter.drawLine(QPointF(m_sanjiaoLeftRect.x() - 1, 30), QPointF(m_sanjiaoLeftRect.x() + m_sanjiaoLeftRect.width(), 24));  // 绘制三角形的左边
        painter.drawLine(QPointF(m_sanjiaoLeftRect.x() - 1, 30), QPointF(m_sanjiaoLeftRect.x() + m_sanjiaoLeftRect.width(), 36));  // 绘制三角形的右边

        // 绘制文本内容
        QPen penText;
        penText.setColor(QColor(51, 51, 51));  // 设置文本颜色为深灰色
        painter.setPen(penText);
        QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);  // 设置文本对齐方式：左对齐，垂直居中
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);  // 设置换行规则
        painter.setFont(this->font());  // 设置字体
        painter.drawText(m_textLeftRect, _text_content, option);  // 绘制消息文本
    }
        break;
    case ChatRole::Role_Self: { // 如果是自己的消息
        // 绘制右侧头像
//        painter.drawRoundedRect(m_iconRightRect,m_iconRightRect.width(),m_iconRightRect.height());
        painter.drawPixmap(m_iconRightRect, _self_pic);

        // 绘制带边框的气泡框
        QColor col_KuangB(234, 234, 234);  // 设置气泡框颜色为浅灰色
        painter.setBrush(QBrush(col_KuangB));
        painter.drawRoundedRect(_right_bubble_rect.x() - 1, _right_bubble_rect.y() - 1,
            _right_bubble_rect.width() + 2, _right_bubble_rect.height() + 2, 4, 4);  // 绘制带边框的矩形

        // 绘制气泡框
        QColor col_Kuang(255, 255, 255);  // 设置气泡框颜色为白色
        painter.setBrush(QBrush(col_Kuang));
        painter.drawRoundedRect(_right_bubble_rect, 4, 4);  // 绘制气泡框

        // 绘制气泡的三角形
        QPointF points[3] = {
            QPointF(m_sanjiaoRightRect.x() + m_sanjiaoRightRect.width(), 30),
            QPointF(m_sanjiaoRightRect.x(), 25),
            QPointF(m_sanjiaoRightRect.x(), 35),
        };
        QPen pen;  // 创建画笔
        pen.setColor(col_Kuang);  // 设置画笔颜色为气泡框的颜色
        painter.setPen(pen);
        painter.drawPolygon(points, 3);  // 绘制三角形

        ///【可用】己方的气泡框 可无边框 （随意）
#if 1
        // 绘制三角形的边框
        QPen penSanJiaoBian;
        penSanJiaoBian.setColor(col_KuangB);  // 设置边框颜色为气泡框的颜色
        painter.setPen(penSanJiaoBian);
        painter.drawLine(QPointF(m_sanjiaoRightRect.x() + m_sanjiaoRightRect.width() + 1, 30), QPointF(m_sanjiaoRightRect.x(), 24));  // 绘制三角形的左边
        painter.drawLine(QPointF(m_sanjiaoRightRect.x() + m_sanjiaoRightRect.width() + 1, 30), QPointF(m_sanjiaoRightRect.x(), 36));  // 绘制三角形的右边
#endif

        // 绘制文本内容
        QPen penText;
        penText.setColor(QColor(51, 51, 51));  // 设置文本颜色为深灰色
        painter.setPen(penText);
        QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);  // 设置文本对齐方式：左对齐，垂直居中
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);  // 设置换行规则
        painter.setFont(this->font());  // 设置字体
        painter.drawText(m_textRightRect, _text_content, option);  // 绘制消息文本
    }
        break;
    case ChatRole::Role_Time: { // 如果是时间
        // 设置时间文本的颜色
        QPen penText;
        penText.setColor(QColor(153, 153, 153));  // 设置时间的文本颜色为浅灰色
        painter.setPen(penText);
        QTextOption option(Qt::AlignCenter);  // 设置时间文本的对齐方式：居中
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);  // 设置换行规则

        // 设置字体样式
        QFont te_font = this->font();
        te_font.setFamily(FontFamily);  // 设置字体为微软雅黑
        te_font.setPointSize(10);  // 设置字体大小
        painter.setFont(te_font);  // 应用字体样式

        // 绘制时间文本
        painter.drawText(this->rect(), _str_curr_time, option);  // 绘制时间
    }
        break;
    default:
        break;
    }
}

//初始化
void ItemWdgt_DataDisplay::initItemWdgt_DataDisplay()
{
    //设置字体
    QFont font = this->font();
    font.setFamily(FontFamily); font.setPointSize(FontSize);
    this->setFont(font);

//    font.setWordSpacing(0);
//    font.setLetterSpacing(QFont::PercentageSpacing,0);
//    font.setLetterSpacing(QFont::PercentageSpacing, 100);          //300%,100为默认  //设置字间距%
//    font.setLetterSpacing(QFont::AbsoluteSpacing, 0);             //设置字间距为3像素 //设置字间距像素值


    //初始化图片    
    _self_pic = QPixmap (IconPath_Self).scaled(QSize(IconWidth, IconHeight), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    _peer_pic = QPixmap (IconPath_Peer).scaled(QSize(IconWidth, IconHeight), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    //初始化动画
    _load_anime = new QMovie(this);
    _load_anime->setFileName(IconPath_LoadAnime);
    _lb_load_anime = new QLabel(this);
    _lb_load_anime->setMovie(_load_anime);
    _lb_load_anime->resize(LoadAnime_Width, LoadAnime_Height);
    _lb_load_anime->setAttribute(Qt::WA_TranslucentBackground, true);
    _lb_load_anime->setAutoFillBackground(false);
}

//控制加载动画
void ItemWdgt_DataDisplay::LoadAnime(const bool is_load)
{
    //更新加载标志(其实只有为 false 时才需要更新，若上层不设置 false，将一直为 true，无更新必要)
     _is_load = is_load;

    //关闭
    if (!is_load) { _lb_load_anime->hide(); return; }

    //开启：先移动到对应位置(右侧气泡框的左侧)
    _lb_load_anime->move(_right_bubble_rect.x() - _lb_load_anime->width() - Interval_Anime_Bubble,
                         _right_bubble_rect.y() + _right_bubble_rect.height() / 2 - _lb_load_anime->height() / 2);
    _load_anime->start();
}

// 获取文本的实际宽度和高度，并进行换行处理
QSize ItemWdgt_DataDisplay::getRealString(const QString &text_content)
{
    // 使用QFontMetricsF来计算文本的字体度量信息
    QFontMetricsF fm(this->font());

    // 获取字体的行高
    m_lineHeight = fm.lineSpacing();

    // 定义变量来存储最大宽度和行数
    int nMaxWidth = 0;
    int nCount = 0;

    // 循环遍历每一行，计算每行的宽度，找到最长的一行
    QStringList list = text_content.split("\n");  // 将文本按换行符分割成多行
    foreach(QString text, list) {
        int textWidth = fm.horizontalAdvance(text);  // 获取每行文本的宽度
        nMaxWidth = qMax(nMaxWidth, textWidth);  // 更新最大宽度
        nCount++;  // 增加行数
    }

    // 如果文本的最大宽度超出了预设的最大宽度，则需要进行换行处理
    if (nMaxWidth > m_textWidth) {
        QString temp;
        foreach(QString text, list) {
            QString tempStr;
            int len = text.length();
            int strWidth = 0;

            // 逐个字符地计算每行的宽度，确保文本不超出最大宽度
            for (int i = 0; i < len; i++) {
                QChar ch = text.at(i);
                strWidth += fm.horizontalAdvance(ch);
                if (strWidth >= m_textWidth) {
                    temp += tempStr + "\n";
                    tempStr = "";
                    strWidth = 0;
                }
                tempStr += ch;
            }
            temp += tempStr + "\n";
        }

        list = temp.split("\n");
        nCount = list.count();
        foreach(QString text, list) {
            int textWidth = fm.horizontalAdvance(text);
            nMaxWidth = qMax(nMaxWidth, textWidth);
        }
    }

    // 返回计算得到的尺寸
    return QSize(nMaxWidth + m_spaceWid, (nCount + 1) * m_lineHeight + 2 * m_lineHeight);
}
