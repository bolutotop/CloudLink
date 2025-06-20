/********************************************************************************
** Form generated from reading UI file 'chat_box.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHAT_BOX_H
#define UI_CHAT_BOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <elapushbutton.h>
#include <subrun/list_datadisplay.h>
#include "elaplaintextedit.h"

QT_BEGIN_NAMESPACE

class Ui_Chat_box
{
public:
    QGridLayout *gridLayout;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    List_DataDisplay *list_DataDisplay;
    ElaPlainTextEdit *txtE_TextContent;
    QHBoxLayout *horizontalLayout;
    QPushButton *pbtn_Recv;
    QSpacerItem *horizontalSpacer;
    ElaPushButton *pbtn_Send;

    void setupUi(QWidget *Chat_box)
    {
        if (Chat_box->objectName().isEmpty())
            Chat_box->setObjectName("Chat_box");
        Chat_box->resize(1002, 766);
        gridLayout = new QGridLayout(Chat_box);
        gridLayout->setObjectName("gridLayout");
        widget = new QWidget(Chat_box);
        widget->setObjectName("widget");
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName("verticalLayout");
        list_DataDisplay = new List_DataDisplay(widget);
        list_DataDisplay->setObjectName("list_DataDisplay");
        list_DataDisplay->setMinimumSize(QSize(0, 0));
        list_DataDisplay->setStyleSheet(QString::fromUtf8(""));
        list_DataDisplay->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

        verticalLayout->addWidget(list_DataDisplay);

        txtE_TextContent = new ElaPlainTextEdit(widget);
        txtE_TextContent->setObjectName("txtE_TextContent");
        txtE_TextContent->setMaximumSize(QSize(16777215, 150));

        verticalLayout->addWidget(txtE_TextContent);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        pbtn_Recv = new QPushButton(widget);
        pbtn_Recv->setObjectName("pbtn_Recv");
        pbtn_Recv->setMinimumSize(QSize(100, 35));
        pbtn_Recv->setMaximumSize(QSize(100, 35));
        QFont font;
        font.setPointSize(14);
        pbtn_Recv->setFont(font);

        horizontalLayout->addWidget(pbtn_Recv);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pbtn_Send = new ElaPushButton(widget);
        pbtn_Send->setObjectName("pbtn_Send");
        pbtn_Send->setMinimumSize(QSize(100, 35));
        pbtn_Send->setMaximumSize(QSize(100, 35));
        pbtn_Send->setFont(font);

        horizontalLayout->addWidget(pbtn_Send);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addWidget(widget, 0, 0, 1, 1);


        retranslateUi(Chat_box);

        QMetaObject::connectSlotsByName(Chat_box);
    } // setupUi

    void retranslateUi(QWidget *Chat_box)
    {
        Chat_box->setWindowTitle(QCoreApplication::translate("Chat_box", "Form", nullptr));
        pbtn_Recv->setText(QCoreApplication::translate("Chat_box", "\346\216\245\346\224\266", nullptr));
        pbtn_Send->setText(QCoreApplication::translate("Chat_box", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Chat_box: public Ui_Chat_box {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHAT_BOX_H
