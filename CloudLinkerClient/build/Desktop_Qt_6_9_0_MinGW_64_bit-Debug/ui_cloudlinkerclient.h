/********************************************************************************
** Form generated from reading UI file 'cloudlinkerclient.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLOUDLINKERCLIENT_H
#define UI_CLOUDLINKERCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>
#include <qtmaterialraisedbutton.h>
#include <qtmaterialtextfield.h>
#include "scrolltext.h"

QT_BEGIN_NAMESPACE

class Ui_CloudLinkerClient
{
public:
    QGridLayout *gridLayout_3;
    QStackedWidget *stackedWidget;
    QWidget *regist_page;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_5;
    QLabel *label_2;
    QtMaterialTextField *rigist_uesrname_lineEdit;
    QtMaterialTextField *regist_passward_lineEdit;
    QtMaterialTextField *regist_passward_ag_lineEdit;
    QWidget *widget;
    QGridLayout *gridLayout_4;
    scrolltext *Tips_regist;
    QHBoxLayout *horizontalLayout_3;
    QtMaterialRaisedButton *return_login_pushButton;
    QtMaterialRaisedButton *regist_pushButton;
    QWidget *login_page;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QtMaterialTextField *passward_lineEdit;
    QHBoxLayout *horizontalLayout;
    QtMaterialRaisedButton *go_regist_pushButton;
    QtMaterialRaisedButton *login_pushButton;
    QtMaterialTextField *user_lineEdit;
    QWidget *container;
    QHBoxLayout *horizontalLayout_2;
    scrolltext *Tips;
    QLabel *label;
    QLabel *label_3;

    void setupUi(QWidget *CloudLinkerClient)
    {
        if (CloudLinkerClient->objectName().isEmpty())
            CloudLinkerClient->setObjectName("CloudLinkerClient");
        CloudLinkerClient->resize(585, 405);
        gridLayout_3 = new QGridLayout(CloudLinkerClient);
        gridLayout_3->setObjectName("gridLayout_3");
        stackedWidget = new QStackedWidget(CloudLinkerClient);
        stackedWidget->setObjectName("stackedWidget");
        regist_page = new QWidget();
        regist_page->setObjectName("regist_page");
        gridLayout_6 = new QGridLayout(regist_page);
        gridLayout_6->setObjectName("gridLayout_6");
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName("gridLayout_5");
        label_2 = new QLabel(regist_page);
        label_2->setObjectName("label_2");
        QFont font;
        font.setPointSize(24);
        font.setBold(true);
        label_2->setFont(font);
        label_2->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: #3fbcbc;\n"
"    font-weight: bold;\n"
"}"));
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_5->addWidget(label_2, 0, 0, 1, 1);

        rigist_uesrname_lineEdit = new QtMaterialTextField(regist_page);
        rigist_uesrname_lineEdit->setObjectName("rigist_uesrname_lineEdit");
        QFont font1;
        font1.setPointSize(14);
        rigist_uesrname_lineEdit->setFont(font1);

        gridLayout_5->addWidget(rigist_uesrname_lineEdit, 1, 0, 1, 1);

        regist_passward_lineEdit = new QtMaterialTextField(regist_page);
        regist_passward_lineEdit->setObjectName("regist_passward_lineEdit");
        regist_passward_lineEdit->setFont(font1);
        regist_passward_lineEdit->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayout_5->addWidget(regist_passward_lineEdit, 2, 0, 1, 1);

        regist_passward_ag_lineEdit = new QtMaterialTextField(regist_page);
        regist_passward_ag_lineEdit->setObjectName("regist_passward_ag_lineEdit");
        regist_passward_ag_lineEdit->setFont(font1);
        regist_passward_ag_lineEdit->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayout_5->addWidget(regist_passward_ag_lineEdit, 3, 0, 1, 1);

        widget = new QWidget(regist_page);
        widget->setObjectName("widget");
        gridLayout_4 = new QGridLayout(widget);
        gridLayout_4->setSpacing(0);
        gridLayout_4->setObjectName("gridLayout_4");
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        Tips_regist = new scrolltext(widget);
        Tips_regist->setObjectName("Tips_regist");

        gridLayout_4->addWidget(Tips_regist, 0, 0, 1, 1);


        gridLayout_5->addWidget(widget, 4, 0, 1, 1);


        gridLayout_6->addLayout(gridLayout_5, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        return_login_pushButton = new QtMaterialRaisedButton(regist_page);
        return_login_pushButton->setObjectName("return_login_pushButton");
        QFont font2;
        font2.setPointSize(12);
        return_login_pushButton->setFont(font2);

        horizontalLayout_3->addWidget(return_login_pushButton);

        regist_pushButton = new QtMaterialRaisedButton(regist_page);
        regist_pushButton->setObjectName("regist_pushButton");
        regist_pushButton->setFont(font2);

        horizontalLayout_3->addWidget(regist_pushButton);


        gridLayout_6->addLayout(horizontalLayout_3, 1, 0, 1, 1);

        stackedWidget->addWidget(regist_page);
        login_page = new QWidget();
        login_page->setObjectName("login_page");
        gridLayout_2 = new QGridLayout(login_page);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        passward_lineEdit = new QtMaterialTextField(login_page);
        passward_lineEdit->setObjectName("passward_lineEdit");
        passward_lineEdit->setStyleSheet(QString::fromUtf8("font-size: 14pt"));
        passward_lineEdit->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayout->addWidget(passward_lineEdit, 4, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        go_regist_pushButton = new QtMaterialRaisedButton(login_page);
        go_regist_pushButton->setObjectName("go_regist_pushButton");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(go_regist_pushButton->sizePolicy().hasHeightForWidth());
        go_regist_pushButton->setSizePolicy(sizePolicy);
        go_regist_pushButton->setFont(font2);

        horizontalLayout->addWidget(go_regist_pushButton);

        login_pushButton = new QtMaterialRaisedButton(login_page);
        login_pushButton->setObjectName("login_pushButton");
        sizePolicy.setHeightForWidth(login_pushButton->sizePolicy().hasHeightForWidth());
        login_pushButton->setSizePolicy(sizePolicy);
        login_pushButton->setFont(font2);
        login_pushButton->setIconSize(QSize(16, 16));
        login_pushButton->setCheckable(false);
        login_pushButton->setAutoRepeatDelay(300);

        horizontalLayout->addWidget(login_pushButton);


        gridLayout->addLayout(horizontalLayout, 7, 0, 1, 1);

        user_lineEdit = new QtMaterialTextField(login_page);
        user_lineEdit->setObjectName("user_lineEdit");
        user_lineEdit->setStyleSheet(QString::fromUtf8("font-size: 14pt"));
        user_lineEdit->setEchoMode(QLineEdit::EchoMode::Normal);

        gridLayout->addWidget(user_lineEdit, 3, 0, 1, 1);

        container = new QWidget(login_page);
        container->setObjectName("container");
        horizontalLayout_2 = new QHBoxLayout(container);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        Tips = new scrolltext(container);
        Tips->setObjectName("Tips");

        horizontalLayout_2->addWidget(Tips);


        gridLayout->addWidget(container, 5, 0, 1, 1);

        label = new QLabel(login_page);
        label->setObjectName("label");
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: #3fbcbc;\n"
"    font-weight: bold;\n"
"}"));
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label, 1, 0, 1, 1);

        label_3 = new QLabel(login_page);
        label_3->setObjectName("label_3");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);
        label_3->setMinimumSize(QSize(0, 0));
        QFont font3;
        font3.setPointSize(8);
        label_3->setFont(font3);
        label_3->setContextMenuPolicy(Qt::ContextMenuPolicy::DefaultContextMenu);
        label_3->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: #D3D3D3;\n"
"}"));
        label_3->setTextFormat(Qt::TextFormat::AutoText);
        label_3->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label_3, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        stackedWidget->addWidget(login_page);

        gridLayout_3->addWidget(stackedWidget, 1, 0, 1, 1);


        retranslateUi(CloudLinkerClient);

        stackedWidget->setCurrentIndex(1);
        login_pushButton->setDefault(false);


        QMetaObject::connectSlotsByName(CloudLinkerClient);
    } // setupUi

    void retranslateUi(QWidget *CloudLinkerClient)
    {
        CloudLinkerClient->setWindowTitle(QCoreApplication::translate("CloudLinkerClient", "CloudLinkerClient", nullptr));
        label_2->setText(QCoreApplication::translate("CloudLinkerClient", "REGIST", nullptr));
        return_login_pushButton->setText(QCoreApplication::translate("CloudLinkerClient", "\350\277\224\345\233\236", nullptr));
        regist_pushButton->setText(QCoreApplication::translate("CloudLinkerClient", "\346\263\250\345\206\214", nullptr));
        go_regist_pushButton->setText(QCoreApplication::translate("CloudLinkerClient", "\345\216\273\346\263\250\345\206\214", nullptr));
        login_pushButton->setText(QCoreApplication::translate("CloudLinkerClient", "\347\231\273\345\275\225", nullptr));
        label->setText(QCoreApplication::translate("CloudLinkerClient", "LOGIN", nullptr));
        label_3->setText(QCoreApplication::translate("CloudLinkerClient", "\346\234\252\350\277\236\346\216\245", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CloudLinkerClient: public Ui_CloudLinkerClient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLOUDLINKERCLIENT_H
