/****************************************************************************
** Meta object code from reading C++ file 'componentselector.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/componentselector/componentselector.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'componentselector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ComponentSelector_t {
    QByteArrayData data[8];
    char stringdata0[101];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ComponentSelector_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ComponentSelector_t qt_meta_stringdata_ComponentSelector = {
    {
QT_MOC_LITERAL(0, 0, 17), // "ComponentSelector"
QT_MOC_LITERAL(1, 18, 15), // "slotItemClicked"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(4, 52, 4), // "item"
QT_MOC_LITERAL(5, 57, 6), // "column"
QT_MOC_LITERAL(6, 64, 15), // "slotContextMenu"
QT_MOC_LITERAL(7, 80, 20) // "slotManageComponents"

    },
    "ComponentSelector\0slotItemClicked\0\0"
    "QTreeWidgetItem*\0item\0column\0"
    "slotContextMenu\0slotManageComponents"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ComponentSelector[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x08 /* Private */,
       6,    1,   34,    2, 0x08 /* Private */,
       7,    0,   37,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    5,
    QMetaType::Void, QMetaType::QPoint,    2,
    QMetaType::Void,

       0        // eod
};

void ComponentSelector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ComponentSelector *_t = static_cast<ComponentSelector *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotItemClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->slotContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 2: _t->slotManageComponents(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ComponentSelector::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_ComponentSelector.data,
      qt_meta_data_ComponentSelector,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ComponentSelector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ComponentSelector::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ComponentSelector.stringdata0))
        return static_cast<void*>(this);
    return QTreeWidget::qt_metacast(_clname);
}

int ComponentSelector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
