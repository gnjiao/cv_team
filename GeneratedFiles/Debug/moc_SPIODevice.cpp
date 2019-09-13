/****************************************************************************
** Meta object code from reading C++ file 'SPIODevice.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Communication/SerialPort/SPIODevice.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SPIODevice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SPIODevice_t {
    QByteArrayData data[27];
    char stringdata0[274];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SPIODevice_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SPIODevice_t qt_meta_stringdata_SPIODevice = {
    {
QT_MOC_LITERAL(0, 0, 10), // "SPIODevice"
QT_MOC_LITERAL(1, 11, 22), // "deviceOpenSuccessfully"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 23), // "deviceCloseSuccessfully"
QT_MOC_LITERAL(4, 59, 9), // "bytesRead"
QT_MOC_LITERAL(5, 69, 4), // "data"
QT_MOC_LITERAL(6, 74, 12), // "bytesWritten"
QT_MOC_LITERAL(7, 87, 8), // "errorStr"
QT_MOC_LITERAL(8, 96, 7), // "infoStr"
QT_MOC_LITERAL(9, 104, 14), // "informationStr"
QT_MOC_LITERAL(10, 119, 12), // "peerHostInfo"
QT_MOC_LITERAL(11, 132, 4), // "host"
QT_MOC_LITERAL(12, 137, 4), // "port"
QT_MOC_LITERAL(13, 142, 10), // "writeBytes"
QT_MOC_LITERAL(14, 153, 4), // "open"
QT_MOC_LITERAL(15, 158, 13), // "serverAddress"
QT_MOC_LITERAL(16, 172, 10), // "serverPort"
QT_MOC_LITERAL(17, 183, 11), // "hostAddress"
QT_MOC_LITERAL(18, 195, 8), // "hostPort"
QT_MOC_LITERAL(19, 204, 11), // "peerAddress"
QT_MOC_LITERAL(20, 216, 8), // "peerPort"
QT_MOC_LITERAL(21, 225, 8), // "portName"
QT_MOC_LITERAL(22, 234, 8), // "baudRate"
QT_MOC_LITERAL(23, 243, 8), // "dataBits"
QT_MOC_LITERAL(24, 252, 8), // "stopBits"
QT_MOC_LITERAL(25, 261, 6), // "parity"
QT_MOC_LITERAL(26, 268, 5) // "close"

    },
    "SPIODevice\0deviceOpenSuccessfully\0\0"
    "deviceCloseSuccessfully\0bytesRead\0"
    "data\0bytesWritten\0errorStr\0infoStr\0"
    "informationStr\0peerHostInfo\0host\0port\0"
    "writeBytes\0open\0serverAddress\0serverPort\0"
    "hostAddress\0hostPort\0peerAddress\0"
    "peerPort\0portName\0baudRate\0dataBits\0"
    "stopBits\0parity\0close"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SPIODevice[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    0,   75,    2, 0x06 /* Public */,
       4,    1,   76,    2, 0x06 /* Public */,
       6,    1,   79,    2, 0x06 /* Public */,
       7,    1,   82,    2, 0x06 /* Public */,
       8,    1,   85,    2, 0x06 /* Public */,
      10,    2,   88,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    1,   93,    2, 0x0a /* Public */,
      14,    2,   96,    2, 0x0a /* Public */,
      14,    4,  101,    2, 0x0a /* Public */,
      14,    5,  110,    2, 0x0a /* Public */,
      26,    0,  121,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    5,
    QMetaType::Void, QMetaType::QByteArray,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::UShort,   11,   12,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   15,   16,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,   20,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   21,   22,   23,   24,   25,
    QMetaType::Void,

       0        // eod
};

void SPIODevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SPIODevice *_t = static_cast<SPIODevice *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->deviceOpenSuccessfully(); break;
        case 1: _t->deviceCloseSuccessfully(); break;
        case 2: _t->bytesRead((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 3: _t->bytesWritten((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 4: _t->errorStr((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->infoStr((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->peerHostInfo((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        case 7: _t->writeBytes((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 8: _t->open((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 9: _t->open((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 10: _t->open((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 11: _t->close(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (SPIODevice::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SPIODevice::deviceOpenSuccessfully)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SPIODevice::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SPIODevice::deviceCloseSuccessfully)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (SPIODevice::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SPIODevice::bytesRead)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (SPIODevice::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SPIODevice::bytesWritten)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (SPIODevice::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SPIODevice::errorStr)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (SPIODevice::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SPIODevice::infoStr)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (SPIODevice::*_t)(QString , quint16 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SPIODevice::peerHostInfo)) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject SPIODevice::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_SPIODevice.data,
      qt_meta_data_SPIODevice,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SPIODevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SPIODevice::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SPIODevice.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int SPIODevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void SPIODevice::deviceOpenSuccessfully()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SPIODevice::deviceCloseSuccessfully()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SPIODevice::bytesRead(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SPIODevice::bytesWritten(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SPIODevice::errorStr(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void SPIODevice::infoStr(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void SPIODevice::peerHostInfo(QString _t1, quint16 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
