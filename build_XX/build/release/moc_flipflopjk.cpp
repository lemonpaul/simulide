/****************************************************************************
** Meta object code from reading C++ file 'flipflopjk.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/circuitwidget/components/flipflopjk.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'flipflopjk.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FlipFlopJK_t {
    QByteArrayData data[9];
    char stringdata0[107];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FlipFlopJK_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FlipFlopJK_t qt_meta_stringdata_FlipFlopJK = {
    {
QT_MOC_LITERAL(0, 0, 10), // "FlipFlopJK"
QT_MOC_LITERAL(1, 11, 12), // "Input_High_V"
QT_MOC_LITERAL(2, 24, 11), // "Input_Low_V"
QT_MOC_LITERAL(3, 36, 11), // "Input_Imped"
QT_MOC_LITERAL(4, 48, 10), // "Out_High_V"
QT_MOC_LITERAL(5, 59, 9), // "Out_Low_V"
QT_MOC_LITERAL(6, 69, 9), // "Out_Imped"
QT_MOC_LITERAL(7, 79, 14), // "Clock_Inverted"
QT_MOC_LITERAL(8, 94, 12) // "S_R_Inverted"

    },
    "FlipFlopJK\0Input_High_V\0Input_Low_V\0"
    "Input_Imped\0Out_High_V\0Out_Low_V\0"
    "Out_Imped\0Clock_Inverted\0S_R_Inverted"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FlipFlopJK[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       8,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::Double, 0x00195003,
       2, QMetaType::Double, 0x00195003,
       3, QMetaType::Double, 0x00195003,
       4, QMetaType::Double, 0x00195003,
       5, QMetaType::Double, 0x00195003,
       6, QMetaType::Double, 0x00195003,
       7, QMetaType::Bool, 0x00195003,
       8, QMetaType::Bool, 0x00195003,

       0        // eod
};

void FlipFlopJK::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        FlipFlopJK *_t = static_cast<FlipFlopJK *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = _t->inputHighV(); break;
        case 1: *reinterpret_cast< double*>(_v) = _t->inputLowV(); break;
        case 2: *reinterpret_cast< double*>(_v) = _t->inputImp(); break;
        case 3: *reinterpret_cast< double*>(_v) = _t->outHighV(); break;
        case 4: *reinterpret_cast< double*>(_v) = _t->outLowV(); break;
        case 5: *reinterpret_cast< double*>(_v) = _t->outImp(); break;
        case 6: *reinterpret_cast< bool*>(_v) = _t->clockInv(); break;
        case 7: *reinterpret_cast< bool*>(_v) = _t->srInv(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        FlipFlopJK *_t = static_cast<FlipFlopJK *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setInputHighV(*reinterpret_cast< double*>(_v)); break;
        case 1: _t->setInputLowV(*reinterpret_cast< double*>(_v)); break;
        case 2: _t->setInputImp(*reinterpret_cast< double*>(_v)); break;
        case 3: _t->setOutHighV(*reinterpret_cast< double*>(_v)); break;
        case 4: _t->setOutLowV(*reinterpret_cast< double*>(_v)); break;
        case 5: _t->setOutImp(*reinterpret_cast< double*>(_v)); break;
        case 6: _t->setClockInv(*reinterpret_cast< bool*>(_v)); break;
        case 7: _t->setSrInv(*reinterpret_cast< bool*>(_v)); break;
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

QT_INIT_METAOBJECT const QMetaObject FlipFlopJK::staticMetaObject = {
    { &LogicComponent::staticMetaObject, qt_meta_stringdata_FlipFlopJK.data,
      qt_meta_data_FlipFlopJK,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FlipFlopJK::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FlipFlopJK::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FlipFlopJK.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "eFlipFlopJK"))
        return static_cast< eFlipFlopJK*>(this);
    return LogicComponent::qt_metacast(_clname);
}

int FlipFlopJK::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = LogicComponent::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
   if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
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
