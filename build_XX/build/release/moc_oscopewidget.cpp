/****************************************************************************
** Meta object code from reading C++ file 'oscopewidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/oscopewidget/oscopewidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'oscopewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OscopeWidget_t {
    QByteArrayData data[12];
    char stringdata0[105];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OscopeWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OscopeWidget_t qt_meta_stringdata_OscopeWidget = {
    {
QT_MOC_LITERAL(0, 0, 12), // "OscopeWidget"
QT_MOC_LITERAL(1, 13, 13), // "HscaleChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 6), // "Hscale"
QT_MOC_LITERAL(4, 35, 13), // "VscaleChanged"
QT_MOC_LITERAL(5, 49, 6), // "Vscale"
QT_MOC_LITERAL(6, 56, 11), // "HposChanged"
QT_MOC_LITERAL(7, 68, 4), // "hPos"
QT_MOC_LITERAL(8, 73, 11), // "VposChanged"
QT_MOC_LITERAL(9, 85, 4), // "Vpos"
QT_MOC_LITERAL(10, 90, 11), // "autoChanged"
QT_MOC_LITERAL(11, 102, 2) // "au"

    },
    "OscopeWidget\0HscaleChanged\0\0Hscale\0"
    "VscaleChanged\0Vscale\0HposChanged\0hPos\0"
    "VposChanged\0Vpos\0autoChanged\0au"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OscopeWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x0a /* Public */,
       4,    1,   42,    2, 0x0a /* Public */,
       6,    1,   45,    2, 0x0a /* Public */,
       8,    1,   48,    2, 0x0a /* Public */,
      10,    1,   51,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,   11,

       0        // eod
};

void OscopeWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        OscopeWidget *_t = static_cast<OscopeWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->HscaleChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->VscaleChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->HposChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->VposChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->autoChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject OscopeWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_OscopeWidget.data,
      qt_meta_data_OscopeWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *OscopeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OscopeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OscopeWidget.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "eElement"))
        return static_cast< eElement*>(this);
    return QWidget::qt_metacast(_clname);
}

int OscopeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
