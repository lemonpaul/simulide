/****************************************************************************
** Meta object code from reading C++ file 'shape.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/circuitwidget/components/shape.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'shape.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Shape_t {
    QByteArrayData data[7];
    char stringdata0[49];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Shape_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Shape_t qt_meta_stringdata_Shape = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Shape"
QT_MOC_LITERAL(1, 6, 6), // "H_size"
QT_MOC_LITERAL(2, 13, 6), // "V_size"
QT_MOC_LITERAL(3, 20, 6), // "Border"
QT_MOC_LITERAL(4, 27, 5), // "Color"
QT_MOC_LITERAL(5, 33, 7), // "Opacity"
QT_MOC_LITERAL(6, 41, 7) // "Z_Value"

    },
    "Shape\0H_size\0V_size\0Border\0Color\0"
    "Opacity\0Z_Value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Shape[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       6,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::Int, 0x00195003,
       2, QMetaType::Int, 0x00195003,
       3, QMetaType::Int, 0x00195103,
       4, QMetaType::QColor, 0x00195103,
       5, QMetaType::QReal, 0x00195103,
       6, QMetaType::QReal, 0x00195003,

       0        // eod
};

void Shape::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        Shape *_t = static_cast<Shape *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->hSize(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->vSize(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->border(); break;
        case 3: *reinterpret_cast< QColor*>(_v) = _t->color(); break;
        case 4: *reinterpret_cast< qreal*>(_v) = _t->opacity(); break;
        case 5: *reinterpret_cast< qreal*>(_v) = _t->zValue(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        Shape *_t = static_cast<Shape *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setHSize(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->setVSize(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setBorder(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->setColor(*reinterpret_cast< QColor*>(_v)); break;
        case 4: _t->setOpacity(*reinterpret_cast< qreal*>(_v)); break;
        case 5: _t->setZValue(*reinterpret_cast< qreal*>(_v)); break;
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

QT_INIT_METAOBJECT const QMetaObject Shape::staticMetaObject = {
    { &Component::staticMetaObject, qt_meta_stringdata_Shape.data,
      qt_meta_data_Shape,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Shape::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Shape::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Shape.stringdata0))
        return static_cast<void*>(this);
    return Component::qt_metacast(_clname);
}

int Shape::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Component::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
   if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
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
