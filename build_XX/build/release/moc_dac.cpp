/****************************************************************************
** Meta object code from reading C++ file 'dac.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/circuitwidget/components/dac.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dac.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DAC_t {
    QByteArrayData data[6];
    char stringdata0[55];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DAC_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DAC_t qt_meta_stringdata_DAC = {
    {
QT_MOC_LITERAL(0, 0, 3), // "DAC"
QT_MOC_LITERAL(1, 4, 12), // "Input_High_V"
QT_MOC_LITERAL(2, 17, 11), // "Input_Low_V"
QT_MOC_LITERAL(3, 29, 11), // "Input_Imped"
QT_MOC_LITERAL(4, 41, 4), // "Vref"
QT_MOC_LITERAL(5, 46, 8) // "Num_Bits"

    },
    "DAC\0Input_High_V\0Input_Low_V\0Input_Imped\0"
    "Vref\0Num_Bits"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DAC[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       5,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::Double, 0x00195003,
       2, QMetaType::Double, 0x00195003,
       3, QMetaType::Double, 0x00195003,
       4, QMetaType::Double, 0x00195003,
       5, QMetaType::Int, 0x00195003,

       0        // eod
};

void DAC::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        DAC *_t = static_cast<DAC *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = _t->inputHighV(); break;
        case 1: *reinterpret_cast< double*>(_v) = _t->inputLowV(); break;
        case 2: *reinterpret_cast< double*>(_v) = _t->inputImp(); break;
        case 3: *reinterpret_cast< double*>(_v) = _t->maxVolt(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->numInps(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        DAC *_t = static_cast<DAC *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setInputHighV(*reinterpret_cast< double*>(_v)); break;
        case 1: _t->setInputLowV(*reinterpret_cast< double*>(_v)); break;
        case 2: _t->setInputImp(*reinterpret_cast< double*>(_v)); break;
        case 3: _t->setMaxVolt(*reinterpret_cast< double*>(_v)); break;
        case 4: _t->setNumInps(*reinterpret_cast< int*>(_v)); break;
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

QT_INIT_METAOBJECT const QMetaObject DAC::staticMetaObject = {
    { &LogicComponent::staticMetaObject, qt_meta_stringdata_DAC.data,
      qt_meta_data_DAC,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DAC::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DAC::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DAC.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "eOutBus"))
        return static_cast< eOutBus*>(this);
    return LogicComponent::qt_metacast(_clname);
}

int DAC::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = LogicComponent::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
   if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
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
