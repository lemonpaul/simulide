/****************************************************************************
** Meta object code from reading C++ file 'ledbar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/circuitwidget/components/ledbar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ledbar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LedBar_t {
    QByteArrayData data[10];
    char stringdata0[85];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LedBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LedBar_t qt_meta_stringdata_LedBar = {
    {
QT_MOC_LITERAL(0, 0, 6), // "LedBar"
QT_MOC_LITERAL(1, 7, 6), // "remove"
QT_MOC_LITERAL(2, 14, 0), // ""
QT_MOC_LITERAL(3, 15, 5), // "Color"
QT_MOC_LITERAL(4, 21, 17), // "LedBase::LedColor"
QT_MOC_LITERAL(5, 39, 4), // "Size"
QT_MOC_LITERAL(6, 44, 9), // "Threshold"
QT_MOC_LITERAL(7, 54, 10), // "MaxCurrent"
QT_MOC_LITERAL(8, 65, 10), // "Resistance"
QT_MOC_LITERAL(9, 76, 8) // "Grounded"

    },
    "LedBar\0remove\0\0Color\0LedBase::LedColor\0"
    "Size\0Threshold\0MaxCurrent\0Resistance\0"
    "Grounded"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LedBar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       6,   20, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

 // properties: name, type, flags
       3, 0x80000000 | 4, 0x0019510b,
       5, QMetaType::Int, 0x00195103,
       6, QMetaType::Double, 0x00195103,
       7, QMetaType::Double, 0x00195103,
       8, QMetaType::Double, 0x00195003,
       9, QMetaType::Bool, 0x00195103,

       0        // eod
};

void LedBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LedBar *_t = static_cast<LedBar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->remove(); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        LedBar *_t = static_cast<LedBar *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< LedBase::LedColor*>(_v) = _t->color(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->size(); break;
        case 2: *reinterpret_cast< double*>(_v) = _t->threshold(); break;
        case 3: *reinterpret_cast< double*>(_v) = _t->maxCurrent(); break;
        case 4: *reinterpret_cast< double*>(_v) = _t->res(); break;
        case 5: *reinterpret_cast< bool*>(_v) = _t->grounded(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        LedBar *_t = static_cast<LedBar *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setColor(*reinterpret_cast< LedBase::LedColor*>(_v)); break;
        case 1: _t->setSize(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setThreshold(*reinterpret_cast< double*>(_v)); break;
        case 3: _t->setMaxCurrent(*reinterpret_cast< double*>(_v)); break;
        case 4: _t->setRes(*reinterpret_cast< double*>(_v)); break;
        case 5: _t->setGrounded(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

static const QMetaObject * const qt_meta_extradata_LedBar[] = {
        &LedBase::staticMetaObject,
    nullptr
};

QT_INIT_METAOBJECT const QMetaObject LedBar::staticMetaObject = {
    { &Component::staticMetaObject, qt_meta_stringdata_LedBar.data,
      qt_meta_data_LedBar,  qt_static_metacall, qt_meta_extradata_LedBar, nullptr}
};


const QMetaObject *LedBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LedBar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LedBar.stringdata0))
        return static_cast<void*>(this);
    return Component::qt_metacast(_clname);
}

int LedBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
