/****************************************************************************
** Meta object code from reading C++ file 'wavegen.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/circuitwidget/components/wavegen.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wavegen.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WaveGen_t {
    QByteArrayData data[12];
    char stringdata0[97];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WaveGen_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WaveGen_t qt_meta_stringdata_WaveGen = {
    {
QT_MOC_LITERAL(0, 0, 7), // "WaveGen"
QT_MOC_LITERAL(1, 8, 12), // "updateValues"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 9), // "Volt_Base"
QT_MOC_LITERAL(4, 32, 11), // "Duty_Square"
QT_MOC_LITERAL(5, 44, 7), // "Quality"
QT_MOC_LITERAL(6, 52, 9), // "Wave_Type"
QT_MOC_LITERAL(7, 62, 9), // "wave_type"
QT_MOC_LITERAL(8, 72, 4), // "Sine"
QT_MOC_LITERAL(9, 77, 3), // "Saw"
QT_MOC_LITERAL(10, 81, 8), // "Triangle"
QT_MOC_LITERAL(11, 90, 6) // "Square"

    },
    "WaveGen\0updateValues\0\0Volt_Base\0"
    "Duty_Square\0Quality\0Wave_Type\0wave_type\0"
    "Sine\0Saw\0Triangle\0Square"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WaveGen[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       4,   20, // properties
       1,   32, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,

 // properties: name, type, flags
       3, QMetaType::Double, 0x00195003,
       4, QMetaType::Double, 0x00195003,
       5, QMetaType::Int, 0x00195103,
       6, 0x80000000 | 7, 0x0019500b,

 // enums: name, flags, count, data
       7, 0x0,    4,   36,

 // enum data: key, value
       8, uint(WaveGen::Sine),
       9, uint(WaveGen::Saw),
      10, uint(WaveGen::Triangle),
      11, uint(WaveGen::Square),

       0        // eod
};

void WaveGen::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WaveGen *_t = static_cast<WaveGen *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateValues(); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        WaveGen *_t = static_cast<WaveGen *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = _t->voltBase(); break;
        case 1: *reinterpret_cast< double*>(_v) = _t->duty(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->quality(); break;
        case 3: *reinterpret_cast< wave_type*>(_v) = _t->waveType(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        WaveGen *_t = static_cast<WaveGen *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setVoltBase(*reinterpret_cast< double*>(_v)); break;
        case 1: _t->setDuty(*reinterpret_cast< double*>(_v)); break;
        case 2: _t->setQuality(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->setWaveType(*reinterpret_cast< wave_type*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject WaveGen::staticMetaObject = {
    { &ClockBase::staticMetaObject, qt_meta_stringdata_WaveGen.data,
      qt_meta_data_WaveGen,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *WaveGen::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WaveGen::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WaveGen.stringdata0))
        return static_cast<void*>(this);
    return ClockBase::qt_metacast(_clname);
}

int WaveGen::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ClockBase::qt_metacall(_c, _id, _a);
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
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
