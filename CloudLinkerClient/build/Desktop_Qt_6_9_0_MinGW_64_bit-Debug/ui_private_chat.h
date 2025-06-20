/********************************************************************************
** Form generated from reading UI file 'private_chat.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRIVATE_CHAT_H
#define UI_PRIVATE_CHAT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <chat_box.h>
#include <elalineedit.h>
#include <elalistview.h>
#include <elapushbutton.h>
#include "elatext.h"
#include "scrolltext.h"

QT_BEGIN_NAMESPACE

class Ui_private_chat
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_3;
    ElaText *friend_label;
    Chat_box *widget;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_2;
    scrolltext *Tips_add;
    ElaText *label_2;
    ElaListView *friend_listView;
    QWidget *widget_5;
    QHBoxLayout *horizontalLayout_2;
    ElaLineEdit *add_friend_lineEdit;
    ElaPushButton *add_friend_pushButton;

    void setupUi(QWidget *private_chat)
    {
        if (private_chat->objectName().isEmpty())
            private_chat->setObjectName("private_chat");
        private_chat->resize(887, 617);
        horizontalLayout = new QHBoxLayout(private_chat);
        horizontalLayout->setObjectName("horizontalLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        widget_3 = new QWidget(private_chat);
        widget_3->setObjectName("widget_3");
        widget_3->setMinimumSize(QSize(0, 0));
        widget_3->setMaximumSize(QSize(16777215, 40));
        horizontalLayout_3 = new QHBoxLayout(widget_3);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        friend_label = new ElaText(widget_3);
        friend_label->setObjectName("friend_label");

        horizontalLayout_3->addWidget(friend_label);


        verticalLayout->addWidget(widget_3);

        widget = new Chat_box(private_chat);
        widget->setObjectName("widget");
        widget->setMinimumSize(QSize(0, 0));
        widget->setLayoutDirection(Qt::LayoutDirection::LeftToRight);

        verticalLayout->addWidget(widget);


        horizontalLayout->addLayout(verticalLayout);

        widget_2 = new QWidget(private_chat);
        widget_2->setObjectName("widget_2");
        widget_2->setMinimumSize(QSize(250, 0));
        widget_2->setMaximumSize(QSize(250, 16777215));
        verticalLayout_2 = new QVBoxLayout(widget_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        Tips_add = new scrolltext(widget_2);
        Tips_add->setObjectName("Tips_add");
        Tips_add->setMinimumSize(QSize(0, 50));

        verticalLayout_2->addWidget(Tips_add);

        label_2 = new ElaText(widget_2);
        label_2->setObjectName("label_2");

        verticalLayout_2->addWidget(label_2);

        friend_listView = new ElaListView(widget_2);
        friend_listView->setObjectName("friend_listView");
        friend_listView->setMinimumSize(QSize(0, 0));
        friend_listView->setLayoutDirection(Qt::LayoutDirection::LeftToRight);

        verticalLayout_2->addWidget(friend_listView);

        widget_5 = new QWidget(widget_2);
        widget_5->setObjectName("widget_5");
        widget_5->setMinimumSize(QSize(0, 50));
        horizontalLayout_2 = new QHBoxLayout(widget_5);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        add_friend_lineEdit = new ElaLineEdit(widget_5);
        add_friend_lineEdit->setObjectName("add_friend_lineEdit");

        horizontalLayout_2->addWidget(add_friend_lineEdit);

        add_friend_pushButton = new ElaPushButton(widget_5);
        add_friend_pushButton->setObjectName("add_friend_pushButton");

        horizontalLayout_2->addWidget(add_friend_pushButton);


        verticalLayout_2->addWidget(widget_5);


        horizontalLayout->addWidget(widget_2);


        retranslateUi(private_chat);

        QMetaObject::connectSlotsByName(private_chat);
    } // setupUi

    void retranslateUi(QWidget *private_chat)
    {
        private_chat->setWindowTitle(QCoreApplication::translate("private_chat", "Form", nullptr));
        friend_label->setText(QCoreApplication::translate("private_chat", "friend", nullptr));
        label_2->setText(QCoreApplication::translate("private_chat", "\345\234\250\347\272\277\345\245\275\345\217\213\345\210\227\350\241\250", nullptr));
        add_friend_pushButton->setText(QCoreApplication::translate("private_chat", "\345\212\240\345\245\275\345\217\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class private_chat: public Ui_private_chat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRIVATE_CHAT_H
