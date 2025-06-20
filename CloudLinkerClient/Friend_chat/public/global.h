#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

//聊天角色
enum ChatRole {
    Role_Time = 0,
    Role_Self = 1,
    Role_Peer = 2,
};

//时间间隔。单位：秒。
const int TimeInterval = 60;

//时间项的高
const int TimeItem_Height = 40;

const QString FontFamily = "新宋体";
const int FontSize = 13;

const int IconWidth = 60;
const int IconHeight = 60;

const QString IconPath_Self = ":/res/icon/20250620183153.jpg";
const QString IconPath_Peer = ":/res/icon/avatar_peer.jpg";
const QString IconPath_LoadAnime = ":/res/icon/load_anime.gif";

const int LoadAnime_Width = 16;
const int LoadAnime_Height = 16;

//间隙距离
const int Interval_Anime_Bubble = 10; //动画框和气泡框之间



#endif // GLOBAL_H
