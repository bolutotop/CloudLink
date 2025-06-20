#include "opendb.h"


QMutex OpenDB::m_mutex;

OpenDB::OpenDB(QObject *parent)
    : QObject{parent}
{
    m_db=QSqlDatabase::addDatabase("QSQLITE");
}

OpenDB::~OpenDB()
{
    m_db.close();
}

OpenDB &OpenDB::getInstance()
{
    static OpenDB instance;
    return instance;
}

void OpenDB::init()
{
    m_db.setHostName("localhost");
    m_db.setDatabaseName("C:\\Users\\55047\\Desktop\\myproject\\CloudLinker\\CloudLinkerServer\\cloud.db");
    if(m_db.open())
    {
        QSqlQuery query;
        query.exec("select*from usrInfo");
        while(query.next())
        {
            QString data=QString("%1,%2,%3").
                           arg(query.value(0).toString()).
                           arg(query.value(1).toString()).
                           arg(query.value(2).toString());
            qDebug()<<data;
        }
    }
    else
    {
        QMessageBox::critical(NULL,"打开数据库","打开数据库失败");
    }
}

bool OpenDB::regist(const char *username, const char *password)
{
    QMutexLocker lock(&m_mutex);

    if(username==NULL||password==NULL)
    {
        return false;
    }
    QString data=QString("insert into usrInfo(name,pwd) values(\'%1\',\'%2\')").arg(username).arg(password);
    QSqlQuery query;
    return query.exec(data);
}

bool OpenDB::login(const char *username, const char *password)
{
    QMutexLocker lock(&m_mutex);
    if(username==NULL||password==NULL)
    {
        return false;
    }
    QString data =QString("select*from usrInfo where name=\'%1\'and pwd=\'%2\'and online =0").arg(username).arg(password);
    QSqlQuery query;

    query.exec(data);
    if(query.next())
    {
        QString data =QString("update usrInfo set online=1 where name=\'%1\'and pwd=\'%2\'").arg(username).arg(password);
        QSqlQuery query;
        query.exec(data);
        return true;
    }
    else
    {
        return false;
    }
}

void OpenDB::disconnectUser(const char *userName)
{
    QMutexLocker lock(&m_mutex);
    if(userName==NULL)
    {
        return;
    }
    QString data =QString("update usrInfo set online=0 where name=\'%1\'").arg(userName);
    QSqlQuery query;
    query.exec(data);
    qDebug()<<userName<<"online=0";
}

QStringList OpenDB::flushFriend(const char*userName)
{
    QMutexLocker lock(&m_mutex);
    QStringList strFriendList;
    strFriendList.clear();
    if(userName==NULL)
    {
        return strFriendList;
    }
    QString data =QString("select name from usrInfo where online=1 and id in(select friendId from friend where "
                           "id=(select id from usrInfo where name=\'%1\'))").arg(userName);
    QSqlQuery query;
    query.exec(data);
    while(query.next())
    {
        strFriendList.append(query.value(0).toString());
        qDebug()<<query.value(0).toString();
    }
    data =QString("select name from usrInfo where online=1 and id in(select id from friend where "
                   "friendId=(select id from usrInfo where name=\'%1\'))").arg(userName);
    query.exec(data);
    while(query.next())
    {
        strFriendList.append(query.value(0).toString());
        qDebug()<<query.value(0).toString();
    }
    return strFriendList;
}

int OpenDB::addFriend(const char *userName, const char *friendName)
{
    QMutexLocker lock(&m_mutex);
    if(userName==NULL||friendName==NULL)
    {
        return -1;
    }
    QString data =QString("select *from friend "
                           "where"
                           "(id=(select id from usrInfo where name=\'%1\')"
                           "and "
                           "friendId=(select id from usrInfo where name=\'%2\'))"
                           "or"
                           "(id=(select id from usrInfo where name=\'%3\')"
                           "and "
                           "friendId=(select id from usrInfo where name=\'%4\'))")
                       .arg(friendName)
                       .arg(userName)
                       .arg(userName)
                       .arg(friendName);
    qDebug()<<data;
    QSqlQuery query;
    query.exec(data);
    if(query.next())
    {
        return 0;//已经是好友
    }
    else
    {
        data =QString("select  online from usrInfo where name=\'%1\'").arg(friendName);
        QSqlQuery query;
        query.exec(data);
        if(query.next())
        {
            int ret= query.value(0).toInt();
            if(ret==1)
            {
                return 1;//在线
            }
            else if(ret==0)
            {
                return 2;//不在线
            }
        }
        else
        {
            return 3;//不存在
        }
    }
}

void OpenDB::addFriendDeel(const char *userName, const char *friendName)
{
    QMutexLocker lock(&m_mutex);
    QString data =QString("insert into friend(id,friendId) values("
                           "(select id from usrInfo where name=\'%1\'),"
                           "(select id from usrInfo where name=\'%2\'))").arg(userName).arg(friendName);
    QSqlQuery query;
    query.exec(data);
}

bool OpenDB::deleteFriend(const char *userName, const char *friendName)
{
    if(userName==NULL||friendName==NULL)
    {
        return false;
    }
    QString data =QString("delete from friend where id="
                           "(select id from usrInfo where name=\'%1\')"
                           "and "
                           "friendId="
                           "(select id from usrInfo where name=\'%2\')").
                   arg(userName).
                   arg(friendName);
    QSqlQuery query;
    query.exec(data);

    data =QString("delete from friend where id="
                   "(select id from usrInfo where name=\'%1\')"
                   "and "
                   "friendId="
                   "(select id from usrInfo where name=\'%2\')").
           arg(friendName).
           arg(userName);
    query.exec(data);

    return true;
}





