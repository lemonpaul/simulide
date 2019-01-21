/****************************************************************************
** Meta object code from reading C++ file 'capacitorbase.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/circuitwidget/components/capacitorbase.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'capacitorbase.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CapacitorBase_t {
    QByteArrayData data[4];
    char stringdata0[40];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CapacitorBase_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CapacitorBase_t qt_meta_stringdata_CapacitorBase = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CapacitorBase"
QT_MOC_LITERAL(1, 14, 11), // "Capacitance"
QT_MOC_LITERAL(2, 26, 4), // "Unit"
QT_MOC_LITERAL(3, 31, 8) // "Show_Cap"

    },
    "CapacitorBase\0Capacitance\0Unit\0Show_Cap"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CapacitorBase[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       3,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::Double, 0x00195003,
       2, QMetaType::QString, 0x00195103,
       3, QMetaType::Bool, 0x00195003,

       0        // eod
};

void CapacitorBase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        CapacitorBase *_t = static_cast<CapacitorBase *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = _t->capac(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->unit(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->showVal(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        CapacitorBase *_t = static_cast<CapacitorBase *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setCapac(*reinterpret_cast< double*>(_v)); break;
        case 1: _t->setUnit(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setShowVal(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject CapacitorBase::staticMetaObject = {
    { &Component::staticMetaObject, qt_meta_stringdata_CapacitorBase.data,
      qt_meta_data_CapacitorBase,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CapacitorBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CapacitorBase::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CapacitorBase.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "eCapacitor"))
        return static_cast< eCapacitor*>(this);
    return Component::qt_metacast(_clname);
}

int CapacitorBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Component::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
   if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
