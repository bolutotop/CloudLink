#ifndef NETWORK_DISK_H
#define NETWORK_DISK_H

#include <QWidget>
#include<QStandardItemModel>
#include"protocol.h"
#include<QTimer>

namespace Ui {
class Network_disk;
}

class Network_disk : public QWidget
{
    Q_OBJECT

public:
    explicit Network_disk(QWidget *parent = nullptr);
    ~Network_disk();
    static Network_disk&getInstance();
    void flushFile();
    void flushFileView(PDU*pdu);
    QString formatFileSize(qint64 bytes);
    void showTips(const char*tips);
    void setDownloadStatus(bool status);
    bool getDownloadStatus();
    QString getSaveFilePath();
    void downloadFile();

    qint64 m_iTotal;
    qint64 m_iRecved;
private slots:

    void on_createDir_pushButton_clicked();

    void on_flush_pushButton_clicked();

    void showContextMenu(const QPoint &pos);

    void handleRowSelection(const QModelIndex &current);

    void enterDir(const QModelIndex &current);

    void on_back_pushButton_clicked();

    void on_upload_pushButton_clicked();

    void uploadFileDate();

    void on_download_pushButton_clicked();

private:
    Ui::Network_disk *ui;
    QString m_fileName;
    QString m_fileType;
    QString m_uploadFilePath;
    QTimer *m_pTimer;
    QString m_strSaveFilePath;
    bool m_bDownload;
};

#endif // NETWORK_DISK_H
