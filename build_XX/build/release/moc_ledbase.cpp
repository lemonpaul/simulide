/****************************************************************************
** Meta object code from reading C++ file 'ledbase.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/circuitwidget/components/ledbase.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ledbase.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LedBase_t {
    QByteArrayData data[15];
    char stringdata0[108];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LedBase_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LedBase_t qt_meta_stringdata_LedBase = {
    {
QT_MOC_LITERAL(0, 0, 7), // "LedBase"
QT_MOC_LITERAL(1, 8, 6), // "remove"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 5), // "Color"
QT_MOC_LITERAL(4, 22, 8), // "LedColor"
QT_MOC_LITERAL(5, 31, 9), // "Threshold"
QT_MOC_LITERAL(6, 41, 10), // "MaxCurrent"
QT_MOC_LITERAL(7, 52, 10), // "Resistance"
QT_MOC_LITERAL(8, 63, 8), // "Grounded"
QT_MOC_LITERAL(9, 72, 6), // "yellow"
QT_MOC_LITERAL(10, 79, 3), // "red"
QT_MOC_LITERAL(11, 83, 5), // "green"
QT_MOC_LITERAL(12, 89, 4), // "blue"
QT_MOC_LITERAL(13, 94, 6), // "orange"
QT_MOC_LITERAL(14, 101, 6) // "purple"

    },
    "LedBase\0remove\0\0Color\0LedColor\0Threshold\0"
    "MaxCurrent\0Resistance\0Grounded\0yellow\0"
    "red\0green\0blue\0orange\0purple"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LedBase[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       5,   20, // properties
       1,   35, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

 // properties: name, type, flags
       3, 0x80000000 | 4, 0x0019510b,
       5, QMetaType::Double, 0x00195103,
       6, QMetaType::Double, 0x00195103,
       7, QMetaType::Double, 0x00195003,
       8, QMetaType::Bool, 0x00195103,

 // enums: name, flags, count, data
       4, 0x0,    6,   39,

 // enum data: key, value
       9, uint(LedBase::yellow),
      10, uint(LedBase::red),
      11, uint(LedBase::green),
      12, uint(LedBase::blue),
      13, uint(LedBase::orange),
      14, uint(LedBase::purple),

       0        // eod
};

void LedBase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LedBase *_t = static_cast<LedBase *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->remove(); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        LedBase *_t = static_cast<LedBase *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< LedColor*>(_v) = _t->color(); break;
        case 1: *reinterpret_cast< double*>(_v) = _t->threshold(); break;
        case 2: *reinterpret_cast< double*>(_v) = _t->maxCurrent(); break;
        case 3: *reinterpret_cast< double*>(_v) = _t->res(); break;
        case 4: *reinterpret_cast< bool*>(_v) = _t->grounded(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        LedBase *_t = static_cast<LedBase *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setColor(*reinterpret_cast< LedColor*>(_v)); break;
        case 1: _t->setThreshold(*reinterpret_cast< double*>(_v)); break;
        case 2: _t->setMaxCurrent(*reinterpret_cast< double*>(_v)); break;
        case 3: _t->setRes(*reinterpret_cast< double*>(_v)); break;
        case 4: _t->setGrounded(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject LedBase::staticMetaObject = {
    { &Component::staticMetaObject, qt_meta_stringdata_LedBase.data,
      qt_meta_data_LedBase,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *LedBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LedBase::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LedBase.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "eLed"))
        return static_cast< eLed*>(this);
    return Component::qt_metacast(_clname);
}

int LedBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Component::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
