/****************************************************************************
** Meta object code from reading C++ file 'SerialPortWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Communication/SerialPort/SerialPortWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SerialPortWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SerialPortWidget_t {
    QByteArrayData data[11];
    char stringdata0[196];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialPortWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialPortWidget_t qt_meta_stringdata_SerialPortWidget = {
    {
QT_MOC_LITERAL(0, 0, 16), // "SerialPortWidget"
QT_MOC_LITERAL(1, 17, 19), // "on_send_btn_Clicked"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 19), // "on_open_btn_Clicked"
QT_MOC_LITERAL(4, 58, 22), // "on_refresh_btn_Clicked"
QT_MOC_LITERAL(5, 81, 25), // "on_clear_recv_btn_Clicked"
QT_MOC_LITERAL(6, 107, 25), // "on_clear_send_btn_Clicked"
QT_MOC_LITERAL(7, 133, 14), // "on_ReveiveText"
QT_MOC_LITERAL(8, 148, 4), // "text"
QT_MOC_LITERAL(9, 153, 11), // "on_SendText"
QT_MOC_LITERAL(10, 165, 30) // "on_clear_statistic_btn_Clicked"

    },
    "SerialPortWidget\0on_send_btn_Clicked\0"
    "\0on_open_btn_Clicked\0on_refresh_btn_Clicked\0"
    "on_clear_recv_btn_Clicked\0"
    "on_clear_send_btn_Clicked\0on_ReveiveText\0"
    "text\0on_SendText\0on_clear_statistic_btn_Clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialPortWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    1,   59,    2, 0x08 /* Private */,
       9,    1,   62,    2, 0x08 /* Private */,
      10,    0,   65,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    8,
    QMetaType::Void, QMetaType::QByteArray,    8,
    QMetaType::Void,

       0        // eod
};

void SerialPortWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SerialPortWidget *_t = static_cast<SerialPortWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_send_btn_Clicked(); break;
        case 1: _t->on_open_btn_Clicked(); break;
        case 2: _t->on_refresh_btn_Clicked(); break;
        case 3: _t->on_clear_recv_btn_Clicked(); break;
        case 4: _t->on_clear_send_btn_Clicked(); break;
        case 5: _t->on_ReveiveText((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 6: _t->on_SendText((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 7: _t->on_clear_statistic_btn_Clicked(); break;
        default: ;
        }
    }
}

const QMetaObject SerialPortWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SerialPortWidget.data,
      qt_meta_data_SerialPortWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SerialPortWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialPortWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SerialPortWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SerialPortWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
