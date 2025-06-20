/********************************************************************************
** Form generated from reading UI file 'ai_model_dialogue.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AI_MODEL_DIALOGUE_H
#define UI_AI_MODEL_DIALOGUE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <elapushbutton.h>
#include "elaplaintextedit.h"

QT_BEGIN_NAMESPACE

class Ui_AI_Model_Dialogue
{
public:
    QVBoxLayout *verticalLayout;
    ElaPlainTextEdit *output_textEdit;
    ElaPlainTextEdit *input_textEdit;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    ElaPushButton *send_pushButton;

    void setupUi(QWidget *AI_Model_Dialogue)
    {
        if (AI_Model_Dialogue->objectName().isEmpty())
            AI_Model_Dialogue->setObjectName("AI_Model_Dialogue");
        AI_Model_Dialogue->resize(862, 612);
        verticalLayout = new QVBoxLayout(AI_Model_Dialogue);
        verticalLayout->setObjectName("verticalLayout");
        output_textEdit = new ElaPlainTextEdit(AI_Model_Dialogue);
        output_textEdit->setObjectName("output_textEdit");

        verticalLayout->addWidget(output_textEdit);

        input_textEdit = new ElaPlainTextEdit(AI_Model_Dialogue);
        input_textEdit->setObjectName("input_textEdit");
        input_textEdit->setMaximumSize(QSize(16777215, 160));

        verticalLayout->addWidget(input_textEdit);

        widget = new QWidget(AI_Model_Dialogue);
        widget->setObjectName("widget");
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(737, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        send_pushButton = new ElaPushButton(widget);
        send_pushButton->setObjectName("send_pushButton");

        horizontalLayout->addWidget(send_pushButton);


        verticalLayout->addWidget(widget);


        retranslateUi(AI_Model_Dialogue);

        QMetaObject::connectSlotsByName(AI_Model_Dialogue);
    } // setupUi

    void retranslateUi(QWidget *AI_Model_Dialogue)
    {
        AI_Model_Dialogue->setWindowTitle(QCoreApplication::translate("AI_Model_Dialogue", "Form", nullptr));
        send_pushButton->setText(QCoreApplication::translate("AI_Model_Dialogue", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AI_Model_Dialogue: public Ui_AI_Model_Dialogue {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AI_MODEL_DIALOGUE_H
