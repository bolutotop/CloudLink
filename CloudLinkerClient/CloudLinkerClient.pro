QT       += core gui network
QT += core gui widgets statemachine
QT += statemachine
QT += core5compat  # 添加对 Qt5 兼容模块的支持
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# include($$PWD/FrameWgt/FrameWgt.pri)
#METERIAL-SDK
LIBS+=$$PWD/libs/libcomponents.a
LIBS+=$$PWD/libs/libElaWidgetTools.a

INCLUDEPATH +=$$PWD/libs/Include
INCLUDEPATH += $$PWD/Friend_chat
DEPENDPATH += $$PWD/Friend_chat

SOURCES += \
    AI_Model_Dialogue/ai_model_dialogue.cpp \
    Friend_chat/chat_box.cpp \
    Friend_chat/private_chat.cpp \
    Friend_chat/public/global.cpp \
    Friend_chat/subrun/itemwdgt_datadisplay.cpp \
    Friend_chat/subrun/list_datadisplay.cpp \
    Logger/logger.cpp \
    Network_disk/T_TableViewModel.cpp \
    Network_disk/network_disk.cpp \
    cloudlinkerclient.cpp \
    main.cpp \
    main_interface.cpp \
    protocol.cpp \
    scrolltext.cpp

HEADERS += \
    AI_Model_Dialogue/ai_model_dialogue.h \
    Friend_chat/chat_box.h \
    Friend_chat/private_chat.h \
    Friend_chat/public/global.h \
    Friend_chat/subrun/itemwdgt_datadisplay.h \
    Friend_chat/subrun/list_datadisplay.h \
    Logger/logger.h \
    Network_disk/T_TableViewModel.h \
    Network_disk/network_disk.h \
    cloudlinkerclient.h \
    main_interface.h \
    protocol.h \
    scrolltext.h

FORMS += \
    AI_Model_Dialogue/ai_model_dialogue.ui \
    Friend_chat/chat_box.ui \
    Friend_chat/private_chat.ui \
    Network_disk/network_disk.ui \
    cloudlinkerclient.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Friend_chat/res.qrc \
    config.qrc \
    pic.qrc
