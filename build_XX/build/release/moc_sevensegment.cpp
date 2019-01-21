/****************************************************************************
** Meta object code from reading C++ file 'sevensegment.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/circuitwidget/components/sevensegment.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sevensegment.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SevenSegment_t {
    QByteArrayData data[9];
    char stringdata0[109];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SevenSegment_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SevenSegment_t qt_meta_stringdata_SevenSegment = {
    {
QT_MOC_LITERAL(0, 0, 12), // "SevenSegment"
QT_MOC_LITERAL(1, 13, 5), // "Color"
QT_MOC_LITERAL(2, 19, 17), // "LedBase::LedColor"
QT_MOC_LITERAL(3, 37, 11), // "NumDisplays"
QT_MOC_LITERAL(4, 49, 13), // "CommonCathode"
QT_MOC_LITERAL(5, 63, 13), // "Vertical_Pins"
QT_MOC_LITERAL(6, 77, 9), // "Threshold"
QT_MOC_LITERAL(7, 87, 10), // "MaxCurrent"
QT_MOC_LITERAL(8, 98, 10) // "Resistance"

    },
    "SevenSegment\0Color\0LedBase::LedColor\0"
    "NumDisplays\0CommonCathode\0Vertical_Pins\0"
    "Threshold\0MaxCurrent\0Resistance"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SevenSegment[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       7,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, 0x80000000 | 2, 0x0019510b,
       3, QMetaType::Int, 0x00195103,
       4, QMetaType::Bool, 0x00195103,
       5, QMetaType::Bool, 0x00195003,
       6, QMetaType::Double, 0x00195103,
       7, QMetaType::Double, 0x00195103,
       8, QMetaType::Double, 0x00195103,

       0        // eod
};

void SevenSegment::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        SevenSegment *_t = static_cast<SevenSegment *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< LedBase::LedColor*>(_v) = _t->color(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->numDisplays(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->isCommonCathode(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->verticalPins(); break;
        case 4: *reinterpret_cast< double*>(_v) = _t->threshold(); break;
        case 5: *reinterpret_cast< double*>(_v) = _t->maxCurrent(); break;
        case 6: *reinterpret_cast< double*>(_v) = _t->resistance(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        SevenSegment *_t = static_cast<SevenSegment *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setColor(*reinterpret_cast< LedBase::LedColor*>(_v)); break;
        case 1: _t->setNumDisplays(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setCommonCathode(*reinterpret_cast< bool*>(_v)); break;
        case 3: _t->setVerticalPins(*reinterpret_cast< bool*>(_v)); break;
        case 4: _t->setThreshold(*reinterpret_cast< double*>(_v)); break;
        case 5: _t->setMaxCurrent(*reinterpret_cast< double*>(_v)); break;
        case 6: _t->setResistance(*reinterpret_cast< double*>(_v)); break;
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

static const QMetaObject * const qt_meta_extradata_SevenSegment[] = {
        &LedBase::staticMetaObject,
    nullptr
};

QT_INIT_METAOBJECT const QMetaObject SevenSegment::staticMetaObject = {
    { &Component::staticMetaObject, qt_meta_stringdata_SevenSegment.data,
      qt_meta_data_SevenSegment,  qt_static_metacall, qt_meta_extradata_SevenSegment, nullptr}
};


const QMetaObject *SevenSegment::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SevenSegment::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SevenSegment.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "eElement"))
        return static_cast< eElement*>(this);
    return Component::qt_metacast(_clname);
}

int SevenSegment::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Component::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
   if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 7;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
