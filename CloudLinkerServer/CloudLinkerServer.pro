QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Logger/logger.cpp \
    main.cpp \
    cloudlinkerserver.cpp \
    mytcpserver.cpp \
    mytcpsocket.cpp \
    opendb.cpp \
    protocol.cpp

HEADERS += \
    Logger/logger.h \
    TreadPool.h \
    cloudlinkerserver.h \
    mytcpserver.h \
    mytcpsocket.h \
    opendb.h \
    protocol.h

FORMS += \
    cloudlinkerserver.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    config.qrc
