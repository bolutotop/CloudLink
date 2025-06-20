/********************************************************************************
** Form generated from reading UI file 'network_disk.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NETWORK_DISK_H
#define UI_NETWORK_DISK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "elaiconbutton.h"
#include "elatableview.h"
#include "elatext.h"
#include "scrolltext.h"

QT_BEGIN_NAMESPACE

class Ui_Network_disk
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_3;
    ElaIconButton *back_pushButton;
    ElaIconButton *createDir_pushButton;
    ElaIconButton *upload_pushButton;
    ElaIconButton *download_pushButton;
    ElaIconButton *shared_pushButton;
    QSpacerItem *horizontalSpacer;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout;
    ElaIconButton *flush_pushButton;
    ElaText *label;
    QWidget *widget_2;
    QGridLayout *gridLayout;
    scrolltext *Tips_widget;
    ElaTableView *tableView;

    void setupUi(QWidget *Network_disk)
    {
        if (Network_disk->objectName().isEmpty())
            Network_disk->setObjectName("Network_disk");
        Network_disk->resize(1045, 679);
        verticalLayout = new QVBoxLayout(Network_disk);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        widget = new QWidget(Network_disk);
        widget->setObjectName("widget");
        horizontalLayout_4 = new QHBoxLayout(widget);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        widget_4 = new QWidget(widget);
        widget_4->setObjectName("widget_4");
        horizontalLayout_3 = new QHBoxLayout(widget_4);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        back_pushButton = new ElaIconButton(ElaIconType::ArrowLeftLong);
        back_pushButton->setObjectName("back_pushButton");
        back_pushButton->setMinimumSize(QSize(40, 0));

        horizontalLayout_3->addWidget(back_pushButton);

        createDir_pushButton = new ElaIconButton(ElaIconType::FolderPlus);
        createDir_pushButton->setObjectName("createDir_pushButton");
        createDir_pushButton->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_3->addWidget(createDir_pushButton);

        upload_pushButton = new ElaIconButton(ElaIconType::FolderArrowUp);
        upload_pushButton->setObjectName("upload_pushButton");
        upload_pushButton->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_3->addWidget(upload_pushButton);

        download_pushButton = new ElaIconButton(ElaIconType::FolderArrowDown);
        download_pushButton->setObjectName("download_pushButton");
        download_pushButton->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_3->addWidget(download_pushButton);

        shared_pushButton = new ElaIconButton(ElaIconType::ShareNodes);
        shared_pushButton->setObjectName("shared_pushButton");
        shared_pushButton->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_3->addWidget(shared_pushButton);

        horizontalSpacer = new QSpacerItem(526, 17, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        verticalLayout_2->addWidget(widget_4);

        widget_3 = new QWidget(widget);
        widget_3->setObjectName("widget_3");
        horizontalLayout = new QHBoxLayout(widget_3);
        horizontalLayout->setObjectName("horizontalLayout");
        flush_pushButton = new ElaIconButton(ElaIconType::ArrowRotateRight);
        flush_pushButton->setObjectName("flush_pushButton");
        flush_pushButton->setMaximumSize(QSize(40, 16777215));

        horizontalLayout->addWidget(flush_pushButton);

        label = new ElaText(widget_3);
        label->setObjectName("label");
        label->setMinimumSize(QSize(180, 0));

        horizontalLayout->addWidget(label);


        verticalLayout_2->addWidget(widget_3);


        horizontalLayout_4->addLayout(verticalLayout_2);

        widget_2 = new QWidget(widget);
        widget_2->setObjectName("widget_2");
        gridLayout = new QGridLayout(widget_2);
        gridLayout->setObjectName("gridLayout");
        Tips_widget = new scrolltext(widget_2);
        Tips_widget->setObjectName("Tips_widget");
        Tips_widget->setMinimumSize(QSize(250, 60));

        gridLayout->addWidget(Tips_widget, 0, 0, 1, 1);


        horizontalLayout_4->addWidget(widget_2);


        horizontalLayout_2->addWidget(widget);


        verticalLayout->addLayout(horizontalLayout_2);

        tableView = new ElaTableView(Network_disk);
        tableView->setObjectName("tableView");

        verticalLayout->addWidget(tableView);


        retranslateUi(Network_disk);

        QMetaObject::connectSlotsByName(Network_disk);
    } // setupUi

    void retranslateUi(QWidget *Network_disk)
    {
        Network_disk->setWindowTitle(QCoreApplication::translate("Network_disk", "Form", nullptr));
        back_pushButton->setText(QCoreApplication::translate("Network_disk", "\350\277\224\345\233\236", nullptr));
        createDir_pushButton->setText(QCoreApplication::translate("Network_disk", "\346\226\260\345\273\272\346\226\207\344\273\266\345\244\271", nullptr));
        upload_pushButton->setText(QCoreApplication::translate("Network_disk", "\344\270\212\344\274\240", nullptr));
        download_pushButton->setText(QCoreApplication::translate("Network_disk", "\344\270\213\350\275\275", nullptr));
        shared_pushButton->setText(QCoreApplication::translate("Network_disk", "\345\210\206\344\272\253\346\226\207\344\273\266", nullptr));
        flush_pushButton->setText(QCoreApplication::translate("Network_disk", "\345\210\267\346\226\260", nullptr));
        label->setText(QCoreApplication::translate("Network_disk", "\346\210\221\347\232\204\347\275\221\347\233\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Network_disk: public Ui_Network_disk {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NETWORK_DISK_H
