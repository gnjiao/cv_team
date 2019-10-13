/****************************************************************************
** Meta object code from reading C++ file 'SerialPortIODevice.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Communication/SerialPort/SerialPortIODevice.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SerialPortIODevice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SerialPortIODevice_t {
    QByteArrayData data[18];
    char stringdata0[187];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialPortIODevice_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialPortIODevice_t qt_meta_stringdata_SerialPortIODevice = {
    {
QT_MOC_LITERAL(0, 0, 18), // "SerialPortIODevice"
QT_MOC_LITERAL(1, 19, 27), // "SerialPortCloseSuccessfully"
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 26), // "SerialPortOpenSuccessfully"
QT_MOC_LITERAL(4, 75, 15), // "Need2OutputInfo"
QT_MOC_LITERAL(5, 91, 4), // "info"
QT_MOC_LITERAL(6, 96, 5), // "color"
QT_MOC_LITERAL(7, 102, 6), // "prefix"
QT_MOC_LITERAL(8, 109, 5), // "close"
QT_MOC_LITERAL(9, 115, 4), // "open"
QT_MOC_LITERAL(10, 120, 8), // "portName"
QT_MOC_LITERAL(11, 129, 8), // "baudRate"
QT_MOC_LITERAL(12, 138, 8), // "dataBits"
QT_MOC_LITERAL(13, 147, 8), // "stopBits"
QT_MOC_LITERAL(14, 156, 6), // "parity"
QT_MOC_LITERAL(15, 163, 10), // "writeBytes"
QT_MOC_LITERAL(16, 174, 4), // "data"
QT_MOC_LITERAL(17, 179, 7) // "ReadAll"

    },
    "SerialPortIODevice\0SerialPortCloseSuccessfully\0"
    "\0SerialPortOpenSuccessfully\0Need2OutputInfo\0"
    "info\0color\0prefix\0close\0open\0portName\0"
    "baudRate\0dataBits\0stopBits\0parity\0"
    "writeBytes\0data\0ReadAll"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialPortIODevice[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    0,   60,    2, 0x06 /* Public */,
       4,    3,   61,    2, 0x06 /* Public */,
       4,    2,   68,    2, 0x26 /* Public | MethodCloned */,
       4,    1,   73,    2, 0x26 /* Public | MethodCloned */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   76,    2, 0x0a /* Public */,
       9,    5,   77,    2, 0x0a /* Public */,
      15,    1,   88,    2, 0x0a /* Public */,
      17,    0,   91,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool,    5,    6,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    5,    6,
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   10,   11,   12,   13,   14,
    QMetaType::Void, QMetaType::QByteArray,   16,
    QMetaType::Void,

       0        // eod
};

void SerialPortIODevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SerialPortIODevice *_t = static_cast<SerialPortIODevice *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SerialPortCloseSuccessfully(); break;
        case 1: _t->SerialPortOpenSuccessfully(); break;
        case 2: _t->Need2OutputInfo((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 3: _t->Need2OutputInfo((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->Need2OutputInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->close(); break;
        case 6: _t->open((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 7: _t->writeBytes((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 8: _t->ReadAll(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (SerialPortIODevice::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortIODevice::SerialPortCloseSuccessfully)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SerialPortIODevice::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortIODevice::SerialPortOpenSuccessfully)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (SerialPortIODevice::*_t)(QString , QString , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortIODevice::Need2OutputInfo)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject SerialPortIODevice::staticMetaObject = {
    { &SPIODevice::staticMetaObject, qt_meta_stringdata_SerialPortIODevice.data,
      qt_meta_data_SerialPortIODevice,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SerialPortIODevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialPortIODevice::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SerialPortIODevice.stringdata0))
        return static_cast<void*>(this);
    return SPIODevice::qt_metacast(_clname);
}

int SerialPortIODevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SPIODevice::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void SerialPortIODevice::SerialPortCloseSuccessfully()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SerialPortIODevice::SerialPortOpenSuccessfully()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SerialPortIODevice::Need2OutputInfo(QString _t1, QString _t2, bool _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
