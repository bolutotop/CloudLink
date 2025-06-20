#include "main_interface.h"

#include"Friend_chat/private_chat.h"
#include"ElaApplication.h"
#include"Network_disk/network_disk.h"
#include"AI_Model_Dialogue/ai_model_dialogue.h"
Main_interface::Main_interface(QWidget *parent)
    : ElaWindow{parent}
{
    resize(1200,800);
    // 启用导航栏
    setIsNavigationBarEnable(true);
    // setCustomWidget(ElaAppBarType::TitleBar, nullptr);

    // 设置用户信息卡片
    setUserInfoCardVisible(true);
    setUserInfoCardPixmap(QPixmap(":/res/icon/20250620183153.jpg"));

    //添加好友分组
    QString friendKey;
    QWidget *gruopChat = new QWidget();
    // 添加一个分组节点
    addExpanderNode("好友", friendKey, ElaIconType::Users);
    // 在分组下添加页面节点
    addPageNode("私聊", &private_chat::getInstance(), friendKey, ElaIconType::User);//私聊
    addPageNode("群聊", gruopChat, friendKey, ElaIconType::UserGroup);//群聊
    private_chat::getInstance().flushFriend();

    //添加网盘分组
    QString NetworkDisk;
    QWidget *transmission=new QWidget;
    // 添加一个分组节点
    addExpanderNode("网盘", NetworkDisk,ElaIconType::Folders);

    addPageNode("我的网盘", &Network_disk::getInstance(), NetworkDisk,ElaIconType::Folder);
    addPageNode("传输", transmission, NetworkDisk,ElaIconType::ArrowRightArrowLeft);


    AI_Model_Dialogue*aiChat=new AI_Model_Dialogue;

    addPageNode("AI对话",aiChat,ElaIconType::Robot);


    // // 添加页脚节点
    // QString footerKey;
    // addFooterNode("设置", footerKey, 0, ElaIconType::Gear);

    // 拦截默认关闭事件
    _closeDialog = new ElaContentDialog(this);
    connect(_closeDialog, &ElaContentDialog::rightButtonClicked, this, &Main_interface::closeWindow);
    connect(_closeDialog, &ElaContentDialog::middleButtonClicked, this, [=]() {
        _closeDialog->close();
        showMinimized();
    });
    this->setIsDefaultClosed(false);
    connect(this, &Main_interface::closeButtonClicked, this, [=]() {
        _closeDialog->exec();
    });

    //移动到中心
    moveToCenter();
}

// Main_interface &Main_interface::getInstance()
// {
//     static Main_interface instance;
//     return instance;
// }

