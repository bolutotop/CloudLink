#ifndef PROTOCOL_H
#define PROTOCOL_H
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <memory>
#include<QFileInfoList>

typedef unsigned int uint;

#define REGIST_OK "regist ok"
#define REGIST_FAILED "regist failed : name existed"

#define LOGIN_OK "login ok"
#define LOGIN_FAILED "login failed:name or pwd error"

#define SEARCH_USR_NO "No Such people"
#define SEARCH_USR_ONLINE "online"
#define SEARCH_USR_OFFLINE "offline"

#define UNKNOW_ERROR "unknow error"

#define EXISTED_FRIEND "friend exist"
#define ADD_FRIEND_OFFLINE "user offline"
#define ADD_FRIEND_NO_EXIST "user not exist"


#define DEL_FRIEND_OK "delete friend ok"
#define DEL_FRIEND_NO "delete friend no"

#define DIR_NO_EXIST "cur dir not exist"

#define FILE_NAME_EXIST "file name exist"

#define CRAETE_DIR_OK "create dir ok"

#define DEL_DIR_OK "delete dir ok"
#define DEL_DIR_NO "delete dir no"

#define DEL_FILE_OK "delete file ok"
#define DEL_FILE_NO "delete file no"

#define RENAME_FILE_OK "rename file ok"
#define RENAME_FILE_NO "rename file no"

#define UPLOAD_FILE_OK "upload file ok"
#define UPLOAD_FILE_NO "upload file no"

enum ENUM_MSG_TYPE
{
    ENUM_MSG_TYPE_MIN=0,
    ENUM_MSG_TYPE_REGIST_REQUEST,  //注册请求 regist request
    ENUM_MSG_TYPE_REGIST_RESPOND,  //注册回复 regist respond

    ENUM_MSG_TYPE_LOGIN_REQUEST,  //登录请求 login request
    ENUM_MSG_TYPE_LOGIN_RESPOND,  //登录回复 login respond

    ENUM_MSG_TYPE_ALL_ONLINE_REQUEST,//显示在线用户请求 all online
    ENUM_MSG_TYPE_ALL_ONLINE_RESPOND,//显示在线用户回复

    ENUM_MSG_TYPE_SEARCH_USR_REQUEST,//搜索在线用户请求 search user
    ENUM_MSG_TYPE_SEARCH_USR_RESPOND,//搜索在线用户回复

    ENUM_MSG_TYPE_ADD_FRIEND_REQUEST,//加好友请求
    ENUM_MSG_TYPE_ADD_FRIEND_RESPOND,//加好友回复

    ENUM_MSG_TYPE_ADD_FRIEND_AGREE,//同意加好友 agree add friend
    ENUM_MSG_TYPE_ADD_FRIEND_AGREE_RESPOND,//结果
    ENUM_MSG_TYPE_ADD_FRIEND_REFUSE,//不同意加好友 refuse add friend

    ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST,//刷新好友请求
    ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND,//刷新好友回复

    ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST,//删除好友请求
    ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND,//删除好友回复

    ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST,//私聊好友请求
    ENUM_MSG_TYPE_PRIVATE_CHAT_RESPOND,//私聊好友回复

    ENUM_MSG_TYPE_GROUP_CHAT_REQUEST,//群聊好友请求
    ENUM_MSG_TYPE_GROUP_CHAT_RESPOND,//群聊好友回复

    ENUM_MSG_TYPE_CREATE_DIR_REQUEST,//创建路径请求
    ENUM_MSG_TYPE_CREATE_DIR_RESPOND,//创建路径回复

    ENUM_MSG_TYPE_FLUSH_FILE_REQUEST,//查看文件请求
    ENUM_MSG_TYPE_FLUSH_FILE_RESPOND,//查看文件回复

    ENUM_MSG_TYPE_DEL_DIR_REQUEST,//删除文件请求
    ENUM_MSG_TYPE_DEL_DIR_RESPOND,//删除文件回复

    ENUM_MSG_TYPE_RENAME_DIR_REQUEST,//重命名文件请求
    ENUM_MSG_TYPE_RENAME_DIR_RESPOND,//重命名文件请求

    ENUM_MSG_TYPE_ENTER_DIR_REQUEST,//进入文件夹请求
    ENUM_MSG_TYPE_ENTER_DIR_RESPOND,//进入文件夹回复

    ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST,//上传文件请求
    ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND,//上传文件回复

    ENUM_MSG_TYPE_ONLINE_FLUSH_REQUEST,//上线提醒

    ENUM_MSG_TYPE_UPLOAD_END_FLAG,//传输结束通知

    ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST,//下载文件请求
    ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND,//下载文件回复

    ENUM_MSG_TYPE_MAX=0x00ffffff,

};


struct FileInfo
{
    char caFileName[32];//文件名
    char suffix[32];  //文件后缀
    int iFileType;      //文件类型
    qint64 fileSizeInBytes;//文件大小
};

//协议
struct PDU
{
    uint uiPDULen;   //总的协议数据单元大小
    uint uiMsgType;  //消息类型
    char caData[64];  //辅助信息
    uint uiMsgLen;   //实际消息长度
    int caMsg[];     //实际消息   
};
PDU *mkPDU(uint uiMsgLen);
#endif // PROTOCOL_H
