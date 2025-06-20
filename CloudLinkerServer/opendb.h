#ifndef OPENDB_H
#define OPENDB_H

#include <QObject>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QMessageBox>
#include <QThreadStorage>
#include <QMutex>
#include<QList>
class OpenDB : public QObject
{
    Q_OBJECT
public:
    // explicit OpenDB(QObject *parent = nullptr);
    ~OpenDB();
    static OpenDB& getInstance();
    void init();
    bool regist(const char*username,const char*password);
    bool login(const char*username,const char*password);
    void disconnectUser(const char*userName);

    QStringList flushFriend(const char*userName);

    int addFriend(const char*userName,const char*friendName);
    void addFriendDeel(const char*userName,const char*friendName);

    bool deleteFriend(const char*userName,const char*friendName);

signals:

private:
    OpenDB(QObject* parent = nullptr); // 禁止外部创建
    OpenDB(const OpenDB&) = delete;
    OpenDB& operator=(const OpenDB&) = delete;

    static QMutex m_mutex;

    QSqlDatabase m_db;
};

#endif // OPENDB_H
