#include "cloudlinkerserver.h"

#include <QApplication>
#include<mytcpserver.h>
#include<opendb.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CloudLinkerServer w;
    OpenDB::getInstance().init();

    w.show();
    return a.exec();
}
