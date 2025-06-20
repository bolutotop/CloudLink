/****************************************************************************
** Meta object code from reading C++ file 'cloudlinkerclient.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../cloudlinkerclient.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cloudlinkerclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN17CloudLinkerClientE_t {};
} // unnamed namespace

template <> constexpr inline auto CloudLinkerClient::qt_create_metaobjectdata<qt_meta_tag_ZN17CloudLinkerClientE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "CloudLinkerClient",
        "showConnect",
        "",
        "recvMsg",
        "clinetOffline",
        "on_login_pushButton_clicked",
        "on_return_login_pushButton_clicked",
        "on_go_regist_pushButton_clicked",
        "on_regist_pushButton_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'showConnect'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'recvMsg'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'clinetOffline'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'on_login_pushButton_clicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_return_login_pushButton_clicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_go_regist_pushButton_clicked'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_regist_pushButton_clicked'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<CloudLinkerClient, qt_meta_tag_ZN17CloudLinkerClientE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject CloudLinkerClient::staticMetaObject = { {
    QMetaObject::SuperData::link<ElaWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17CloudLinkerClientE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17CloudLinkerClientE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17CloudLinkerClientE_t>.metaTypes,
    nullptr
} };

void CloudLinkerClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CloudLinkerClient *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->showConnect(); break;
        case 1: _t->recvMsg(); break;
        case 2: _t->clinetOffline(); break;
        case 3: _t->on_login_pushButton_clicked(); break;
        case 4: _t->on_return_login_pushButton_clicked(); break;
        case 5: _t->on_go_regist_pushButton_clicked(); break;
        case 6: _t->on_regist_pushButton_clicked(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *CloudLinkerClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CloudLinkerClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17CloudLinkerClientE_t>.strings))
        return static_cast<void*>(this);
    return ElaWidget::qt_metacast(_clname);
}

int CloudLinkerClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ElaWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
