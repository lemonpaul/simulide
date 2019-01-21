/****************************************************************************
** Meta object code from reading C++ file 'ledmatrix.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/circuitwidget/components/ledmatrix.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ledmatrix.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LedMatrix_t {
    QByteArrayData data[11];
    char stringdata0[98];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LedMatrix_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LedMatrix_t qt_meta_stringdata_LedMatrix = {
    {
QT_MOC_LITERAL(0, 0, 9), // "LedMatrix"
QT_MOC_LITERAL(1, 10, 6), // "remove"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 5), // "Color"
QT_MOC_LITERAL(4, 24, 17), // "LedBase::LedColor"
QT_MOC_LITERAL(5, 42, 4), // "Rows"
QT_MOC_LITERAL(6, 47, 4), // "Cols"
QT_MOC_LITERAL(7, 52, 13), // "Vertical_Pins"
QT_MOC_LITERAL(8, 66, 9), // "Threshold"
QT_MOC_LITERAL(9, 76, 10), // "MaxCurrent"
QT_MOC_LITERAL(10, 87, 10) // "Resistance"

    },
    "LedMatrix\0remove\0\0Color\0LedBase::LedColor\0"
    "Rows\0Cols\0Vertical_Pins\0Threshold\0"
    "MaxCurrent\0Resistance"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LedMatrix[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       7,   20, // properties
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
       6, QMetaType::Int, 0x00195103,
       7, QMetaType::Bool, 0x00195003,
       8, QMetaType::Double, 0x00195103,
       9, QMetaType::Double, 0x00195103,
      10, QMetaType::Double, 0x00195003,

       0        // eod
};

void LedMatrix::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LedMatrix *_t = static_cast<LedMatrix *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->remove(); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        LedMatrix *_t = static_cast<LedMatrix *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< LedBase::LedColor*>(_v) = _t->color(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->rows(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->cols(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->verticalPins(); break;
        case 4: *reinterpret_cast< double*>(_v) = _t->threshold(); break;
        case 5: *reinterpret_cast< double*>(_v) = _t->maxCurrent(); break;
        case 6: *reinterpret_cast< double*>(_v) = _t->res(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        LedMatrix *_t = static_cast<LedMatrix *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setColor(*reinterpret_cast< LedBase::LedColor*>(_v)); break;
        case 1: _t->setRows(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setCols(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->setVerticalPins(*reinterpret_cast< bool*>(_v)); break;
        case 4: _t->setThreshold(*reinterpret_cast< double*>(_v)); break;
        case 5: _t->setMaxCurrent(*reinterpret_cast< double*>(_v)); break;
        case 6: _t->setRes(*reinterpret_cast< double*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

static const QMetaObject * const qt_meta_extradata_LedMatrix[] = {
        &LedBase::staticMetaObject,
    nullptr
};

QT_INIT_METAOBJECT const QMetaObject LedMatrix::staticMetaObject = {
    { &Component::staticMetaObject, qt_meta_stringdata_LedMatrix.data,
      qt_meta_data_LedMatrix,  qt_static_metacall, qt_meta_extradata_LedMatrix, nullptr}
};


const QMetaObject *LedMatrix::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LedMatrix::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LedMatrix.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "eElement"))
        return static_cast< eElement*>(this);
    return Component::qt_metacast(_clname);
}

int LedMatrix::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
