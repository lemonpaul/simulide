/****************************************************************************
** Meta object code from reading C++ file 'gate.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/circuitwidget/components/gate.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gate.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Gate_t {
    QByteArrayData data[11];
    char stringdata0[105];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Gate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Gate_t qt_meta_stringdata_Gate = {
    {
QT_MOC_LITERAL(0, 0, 4), // "Gate"
QT_MOC_LITERAL(1, 5, 6), // "remove"
QT_MOC_LITERAL(2, 12, 0), // ""
QT_MOC_LITERAL(3, 13, 12), // "Input_High_V"
QT_MOC_LITERAL(4, 26, 11), // "Input_Low_V"
QT_MOC_LITERAL(5, 38, 11), // "Input_Imped"
QT_MOC_LITERAL(6, 50, 10), // "Out_High_V"
QT_MOC_LITERAL(7, 61, 9), // "Out_Low_V"
QT_MOC_LITERAL(8, 71, 9), // "Out_Imped"
QT_MOC_LITERAL(9, 81, 8), // "Inverted"
QT_MOC_LITERAL(10, 90, 14) // "Open_Collector"

    },
    "Gate\0remove\0\0Input_High_V\0Input_Low_V\0"
    "Input_Imped\0Out_High_V\0Out_Low_V\0"
    "Out_Imped\0Inverted\0Open_Collector"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Gate[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       8,   20, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

 // properties: name, type, flags
       3, QMetaType::Double, 0x00195003,
       4, QMetaType::Double, 0x00195003,
       5, QMetaType::Double, 0x00195003,
       6, QMetaType::Double, 0x00195003,
       7, QMetaType::Double, 0x00195003,
       8, QMetaType::Double, 0x00195003,
       9, QMetaType::Bool, 0x00195103,
      10, QMetaType::Bool, 0x00195003,

       0        // eod
};

void Gate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Gate *_t = static_cast<Gate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->remove(); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        Gate *_t = static_cast<Gate *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = _t->inputHighV(); break;
        case 1: *reinterpret_cast< double*>(_v) = _t->inputLowV(); break;
        case 2: *reinterpret_cast< double*>(_v) = _t->inputImp(); break;
        case 3: *reinterpret_cast< double*>(_v) = _t->outHighV(); break;
        case 4: *reinterpret_cast< double*>(_v) = _t->outLowV(); break;
        case 5: *reinterpret_cast< double*>(_v) = _t->outImp(); break;
        case 6: *reinterpret_cast< bool*>(_v) = _t->inverted(); break;
        case 7: *reinterpret_cast< bool*>(_v) = _t->openCol(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        Gate *_t = static_cast<Gate *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setInputHighV(*reinterpret_cast< double*>(_v)); break;
        case 1: _t->setInputLowV(*reinterpret_cast< double*>(_v)); break;
        case 2: _t->setInputImp(*reinterpret_cast< double*>(_v)); break;
        case 3: _t->setOutHighV(*reinterpret_cast< double*>(_v)); break;
        case 4: _t->setOutLowV(*reinterpret_cast< double*>(_v)); break;
        case 5: _t->setOutImp(*reinterpret_cast< double*>(_v)); break;
        case 6: _t->setInverted(*reinterpret_cast< bool*>(_v)); break;
        case 7: _t->setOpenCol(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject Gate::staticMetaObject = {
    { &Component::staticMetaObject, qt_meta_stringdata_Gate.data,
      qt_meta_data_Gate,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Gate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Gate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Gate.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "eGate"))
        return static_cast< eGate*>(this);
    return Component::qt_metacast(_clname);
}

int Gate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 8;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
