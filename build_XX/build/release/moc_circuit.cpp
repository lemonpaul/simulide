/****************************************************************************
** Meta object code from reading C++ file 'circuit.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/circuitwidget/circuit.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'circuit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Circuit_t {
    QByteArrayData data[17];
    char stringdata0[142];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Circuit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Circuit_t qt_meta_stringdata_Circuit = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Circuit"
QT_MOC_LITERAL(1, 8, 16), // "createSubcircuit"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 4), // "copy"
QT_MOC_LITERAL(4, 31, 10), // "eventpoint"
QT_MOC_LITERAL(5, 42, 5), // "paste"
QT_MOC_LITERAL(6, 48, 4), // "undo"
QT_MOC_LITERAL(7, 53, 4), // "redo"
QT_MOC_LITERAL(8, 58, 10), // "importCirc"
QT_MOC_LITERAL(9, 69, 3), // "bom"
QT_MOC_LITERAL(10, 73, 5), // "Speed"
QT_MOC_LITERAL(11, 79, 9), // "ReactStep"
QT_MOC_LITERAL(12, 89, 9), // "NoLinStep"
QT_MOC_LITERAL(13, 99, 8), // "NoLinAcc"
QT_MOC_LITERAL(14, 108, 9), // "Draw_Grid"
QT_MOC_LITERAL(15, 118, 15), // "Show_ScrollBars"
QT_MOC_LITERAL(16, 134, 7) // "Animate"

    },
    "Circuit\0createSubcircuit\0\0copy\0"
    "eventpoint\0paste\0undo\0redo\0importCirc\0"
    "bom\0Speed\0ReactStep\0NoLinStep\0NoLinAcc\0"
    "Draw_Grid\0Show_ScrollBars\0Animate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Circuit[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       7,   62, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a /* Public */,
       3,    1,   50,    2, 0x0a /* Public */,
       5,    1,   53,    2, 0x0a /* Public */,
       6,    0,   56,    2, 0x0a /* Public */,
       7,    0,   57,    2, 0x0a /* Public */,
       8,    1,   58,    2, 0x0a /* Public */,
       9,    0,   61,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPointF,    4,
    QMetaType::Void, QMetaType::QPointF,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPointF,    4,
    QMetaType::Void,

 // properties: name, type, flags
      10, QMetaType::Int, 0x00195003,
      11, QMetaType::Int, 0x00195103,
      12, QMetaType::Int, 0x00195103,
      13, QMetaType::Int, 0x00195103,
      14, QMetaType::Bool, 0x00195003,
      15, QMetaType::Bool, 0x00195003,
      16, QMetaType::Bool, 0x00195103,

       0        // eod
};

void Circuit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Circuit *_t = static_cast<Circuit *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->createSubcircuit(); break;
        case 1: _t->copy((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 2: _t->paste((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 3: _t->undo(); break;
        case 4: _t->redo(); break;
        case 5: _t->importCirc((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 6: _t->bom(); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        Circuit *_t = static_cast<Circuit *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->circSpeed(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->reactStep(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->noLinStep(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->noLinAcc(); break;
        case 4: *reinterpret_cast< bool*>(_v) = _t->drawGrid(); break;
        case 5: *reinterpret_cast< bool*>(_v) = _t->showScroll(); break;
        case 6: *reinterpret_cast< bool*>(_v) = _t->animate(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        Circuit *_t = static_cast<Circuit *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setCircSpeed(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->setReactStep(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setNoLinStep(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->setNoLinAcc(*reinterpret_cast< int*>(_v)); break;
        case 4: _t->setDrawGrid(*reinterpret_cast< bool*>(_v)); break;
        case 5: _t->setShowScroll(*reinterpret_cast< bool*>(_v)); break;
        case 6: _t->setAnimate(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject Circuit::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_Circuit.data,
      qt_meta_data_Circuit,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Circuit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Circuit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Circuit.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsScene::qt_metacast(_clname);
}

int Circuit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
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
