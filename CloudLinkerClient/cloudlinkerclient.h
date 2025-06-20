#ifndef CLOUDLINKERCLIENT_H
#define CLOUDLINKERCLIENT_H
#include<QFile>
#include<QTcpSocket>
#include<QAction>
#include<QStringList>
#include<QParallelAnimationGroup>

#include"ElaWidget.h"
#include"main_interface.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class CloudLinkerClient;
}
QT_END_NAMESPACE

class CloudLinkerClient : public ElaWidget
{
    Q_OBJECT

public:
    CloudLinkerClient(QWidget *parent = nullptr);
    ~CloudLinkerClient();

    static CloudLinkerClient& getInstance();

    void loadConfig();
    void pageSwitch(int pageIndex);
    // void loginRequest(const QString& uesrName,const QString& password);
    QString getUserName();
    QTcpSocket& getUserSocket();
    QString getCurPath();
    void updatePath(const char* Path);

public slots:
    void showConnect();
    void recvMsg();
    void clinetOffline();

private slots:
    // void on_pushButton_clicked();

    void on_login_pushButton_clicked();

    void on_return_login_pushButton_clicked();

    void on_go_regist_pushButton_clicked();

    void on_regist_pushButton_clicked();

private:
    Ui::CloudLinkerClient *ui;

    QString m_strIp;
    quint16 m_usport;

    QTcpSocket m_tcpsocket;

    QString m_strUserName;

    Main_interface *interfaceW;

    QString m_strCurPath;

    QFile m_file;
};
#endif // CLOUDLINKERCLIENT_H
