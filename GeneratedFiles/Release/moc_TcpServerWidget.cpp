/****************************************************************************
** Meta object code from reading C++ file 'TcpServerWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Communication/TcpServer/TcpServerWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TcpServerWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TcpServerWidget_t {
    QByteArrayData data[18];
    char stringdata0[299];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TcpServerWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TcpServerWidget_t qt_meta_stringdata_TcpServerWidget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "TcpServerWidget"
QT_MOC_LITERAL(1, 16, 20), // "on_close_btn_Clicked"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 21), // "on_listen_btn_Clicked"
QT_MOC_LITERAL(4, 60, 14), // "changedClients"
QT_MOC_LITERAL(5, 75, 18), // "QList<QTcpSocket*>"
QT_MOC_LITERAL(6, 94, 7), // "clients"
QT_MOC_LITERAL(7, 102, 16), // "setCurrentClient"
QT_MOC_LITERAL(8, 119, 4), // "text"
QT_MOC_LITERAL(9, 124, 13), // "setServerPort"
QT_MOC_LITERAL(10, 138, 4), // "port"
QT_MOC_LITERAL(11, 143, 25), // "on_deviceOpenSuccessfully"
QT_MOC_LITERAL(12, 169, 19), // "on_send_btn_Clicked"
QT_MOC_LITERAL(13, 189, 25), // "on_clear_recv_btn_Clicked"
QT_MOC_LITERAL(14, 215, 25), // "on_clear_send_btn_Clicked"
QT_MOC_LITERAL(15, 241, 14), // "on_ReceiveText"
QT_MOC_LITERAL(16, 256, 11), // "on_SendText"
QT_MOC_LITERAL(17, 268, 30) // "on_clear_statistic_btn_Clicked"

    },
    "TcpServerWidget\0on_close_btn_Clicked\0"
    "\0on_listen_btn_Clicked\0changedClients\0"
    "QList<QTcpSocket*>\0clients\0setCurrentClient\0"
    "text\0setServerPort\0port\0"
    "on_deviceOpenSuccessfully\0on_send_btn_Clicked\0"
    "on_clear_recv_btn_Clicked\0"
    "on_clear_send_btn_Clicked\0on_ReceiveText\0"
    "on_SendText\0on_clear_statistic_btn_Clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TcpServerWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x08 /* Private */,
       3,    0,   75,    2, 0x08 /* Private */,
       4,    1,   76,    2, 0x08 /* Private */,
       7,    1,   79,    2, 0x08 /* Private */,
       9,    1,   82,    2, 0x08 /* Private */,
      11,    0,   85,    2, 0x08 /* Private */,
      12,    0,   86,    2, 0x08 /* Private */,
      13,    0,   87,    2, 0x08 /* Private */,
      14,    0,   88,    2, 0x08 /* Private */,
      15,    1,   89,    2, 0x08 /* Private */,
      16,    1,   92,    2, 0x08 /* Private */,
      17,    0,   95,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    8,
    QMetaType::Void, QMetaType::QByteArray,    8,
    QMetaType::Void,

       0        // eod
};

void TcpServerWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TcpServerWidget *_t = static_cast<TcpServerWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_close_btn_Clicked(); break;
        case 1: _t->on_listen_btn_Clicked(); break;
        case 2: _t->changedClients((*reinterpret_cast< QList<QTcpSocket*>(*)>(_a[1]))); break;
        case 3: _t->setCurrentClient((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->setServerPort((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->on_deviceOpenSuccessfully(); break;
        case 6: _t->on_send_btn_Clicked(); break;
        case 7: _t->on_clear_recv_btn_Clicked(); break;
        case 8: _t->on_clear_send_btn_Clicked(); break;
        case 9: _t->on_ReceiveText((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 10: _t->on_SendText((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 11: _t->on_clear_statistic_btn_Clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QTcpSocket*> >(); break;
            }
            break;
        }
    }
}

const QMetaObject TcpServerWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TcpServerWidget.data,
      qt_meta_data_TcpServerWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TcpServerWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TcpServerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TcpServerWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TcpServerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
