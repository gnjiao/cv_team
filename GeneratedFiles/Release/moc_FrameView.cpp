/****************************************************************************
** Meta object code from reading C++ file 'FrameView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DataModel/FrameView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrameView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GView_t {
    QByteArrayData data[1];
    char stringdata0[6];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GView_t qt_meta_stringdata_GView = {
    {
QT_MOC_LITERAL(0, 0, 5) // "GView"

    },
    "GView"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void GView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject GView::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_GView.data,
      qt_meta_data_GView,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *GView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GView.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int GView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_FrameView_t {
    QByteArrayData data[10];
    char stringdata0[111];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FrameView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FrameView_t qt_meta_stringdata_FrameView = {
    {
QT_MOC_LITERAL(0, 0, 9), // "FrameView"
QT_MOC_LITERAL(1, 10, 15), // "to_imageChanged"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 6), // "zoomIn"
QT_MOC_LITERAL(4, 34, 5), // "level"
QT_MOC_LITERAL(5, 40, 7), // "zoomOut"
QT_MOC_LITERAL(6, 48, 11), // "setupMatrix"
QT_MOC_LITERAL(7, 60, 24), // "on_adaptToWindow_clicked"
QT_MOC_LITERAL(8, 85, 7), // "isAdapt"
QT_MOC_LITERAL(9, 93, 17) // "togglePointerMode"

    },
    "FrameView\0to_imageChanged\0\0zoomIn\0"
    "level\0zoomOut\0setupMatrix\0"
    "on_adaptToWindow_clicked\0isAdapt\0"
    "togglePointerMode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FrameView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   55,    2, 0x0a /* Public */,
       3,    0,   58,    2, 0x2a /* Public | MethodCloned */,
       5,    1,   59,    2, 0x0a /* Public */,
       5,    0,   62,    2, 0x2a /* Public | MethodCloned */,
       6,    0,   63,    2, 0x08 /* Private */,
       7,    1,   64,    2, 0x08 /* Private */,
       9,    0,   67,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void,

       0        // eod
};

void FrameView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FrameView *_t = static_cast<FrameView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->to_imageChanged(); break;
        case 1: _t->zoomIn((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->zoomIn(); break;
        case 3: _t->zoomOut((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->zoomOut(); break;
        case 5: _t->setupMatrix(); break;
        case 6: _t->on_adaptToWindow_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->togglePointerMode(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (FrameView::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FrameView::to_imageChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject FrameView::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_FrameView.data,
      qt_meta_data_FrameView,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FrameView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FrameView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FrameView.stringdata0))
        return static_cast<void*>(this);
    return QFrame::qt_metacast(_clname);
}

int FrameView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void FrameView::to_imageChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
