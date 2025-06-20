#ifndef MAIN_INTERFACE_H
#define MAIN_INTERFACE_H

#include <QMainWindow>
#include"ElaWindow.h"
#include"ElaContentDialog.h"



class Main_interface : public ElaWindow
{
    Q_OBJECT
public:
    explicit Main_interface(QWidget *parent = nullptr);
    // static Main_interface&getInstance();
signals:


private:
    ElaContentDialog* _closeDialog{nullptr};
};

#endif // MAIN_INTERFACE_H
