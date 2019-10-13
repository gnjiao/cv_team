/****************************************************************************
** Meta object code from reading C++ file 'BaseWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DataModel/BaseWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BaseWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BaseWidget_t {
    QByteArrayData data[9];
    char stringdata0[162];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BaseWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BaseWidget_t qt_meta_stringdata_BaseWidget = {
    {
QT_MOC_LITERAL(0, 0, 10), // "BaseWidget"
QT_MOC_LITERAL(1, 11, 28), // "on_action_openFile_triggered"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 24), // "on_action_play_triggered"
QT_MOC_LITERAL(4, 66, 22), // "on_action_edit_toggled"
QT_MOC_LITERAL(5, 89, 1), // "b"
QT_MOC_LITERAL(6, 91, 30), // "on_action_editCancel_triggered"
QT_MOC_LITERAL(7, 122, 17), // "on_OK_btn_clicked"
QT_MOC_LITERAL(8, 140, 21) // "on_Cancel_btn_clicked"

    },
    "BaseWidget\0on_action_openFile_triggered\0"
    "\0on_action_play_triggered\0"
    "on_action_edit_toggled\0b\0"
    "on_action_editCancel_triggered\0"
    "on_OK_btn_clicked\0on_Cancel_btn_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BaseWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x08 /* Private */,
       3,    0,   45,    2, 0x08 /* Private */,
       4,    1,   46,    2, 0x08 /* Private */,
       6,    0,   49,    2, 0x08 /* Private */,
       7,    0,   50,    2, 0x08 /* Private */,
       8,    0,   51,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void BaseWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BaseWidget *_t = static_cast<BaseWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_action_openFile_triggered(); break;
        case 1: _t->on_action_play_triggered(); break;
        case 2: _t->on_action_edit_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->on_action_editCancel_triggered(); break;
        case 4: _t->on_OK_btn_clicked(); break;
        case 5: _t->on_Cancel_btn_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject BaseWidget::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_BaseWidget.data,
      qt_meta_data_BaseWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *BaseWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BaseWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BaseWidget.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int BaseWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
