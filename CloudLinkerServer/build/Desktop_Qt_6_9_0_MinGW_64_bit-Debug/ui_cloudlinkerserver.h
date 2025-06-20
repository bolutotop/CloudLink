/********************************************************************************
** Form generated from reading UI file 'cloudlinkerserver.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLOUDLINKERSERVER_H
#define UI_CLOUDLINKERSERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CloudLinkerServer
{
public:

    void setupUi(QWidget *CloudLinkerServer)
    {
        if (CloudLinkerServer->objectName().isEmpty())
            CloudLinkerServer->setObjectName("CloudLinkerServer");
        CloudLinkerServer->resize(800, 600);

        retranslateUi(CloudLinkerServer);

        QMetaObject::connectSlotsByName(CloudLinkerServer);
    } // setupUi

    void retranslateUi(QWidget *CloudLinkerServer)
    {
        CloudLinkerServer->setWindowTitle(QCoreApplication::translate("CloudLinkerServer", "CloudLinkerServer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CloudLinkerServer: public Ui_CloudLinkerServer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLOUDLINKERSERVER_H
