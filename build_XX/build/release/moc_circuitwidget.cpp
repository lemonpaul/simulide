/****************************************************************************
** Meta object code from reading C++ file 'circuitwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/circuitwidget/circuitwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'circuitwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CircuitWidget_t {
    QByteArrayData data[13];
    char stringdata0[113];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CircuitWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CircuitWidget_t qt_meta_stringdata_CircuitWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CircuitWidget"
QT_MOC_LITERAL(1, 14, 13), // "dataAvailable"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 4), // "data"
QT_MOC_LITERAL(4, 34, 10), // "newCircuit"
QT_MOC_LITERAL(5, 45, 8), // "openCirc"
QT_MOC_LITERAL(6, 54, 8), // "loadCirc"
QT_MOC_LITERAL(7, 63, 4), // "path"
QT_MOC_LITERAL(8, 68, 8), // "saveCirc"
QT_MOC_LITERAL(9, 77, 10), // "saveCircAs"
QT_MOC_LITERAL(10, 88, 9), // "powerCirc"
QT_MOC_LITERAL(11, 98, 8), // "openInfo"
QT_MOC_LITERAL(12, 107, 5) // "about"

    },
    "CircuitWidget\0dataAvailable\0\0data\0"
    "newCircuit\0openCirc\0loadCirc\0path\0"
    "saveCirc\0saveCircAs\0powerCirc\0openInfo\0"
    "about"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CircuitWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   62,    2, 0x0a /* Public */,
       5,    0,   63,    2, 0x0a /* Public */,
       6,    1,   64,    2, 0x0a /* Public */,
       8,    0,   67,    2, 0x0a /* Public */,
       9,    0,   68,    2, 0x0a /* Public */,
      10,    0,   69,    2, 0x0a /* Public */,
      11,    0,   70,    2, 0x0a /* Public */,
      12,    0,   71,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CircuitWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CircuitWidget *_t = static_cast<CircuitWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dataAvailable((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: { bool _r = _t->newCircuit();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 2: _t->openCirc(); break;
        case 3: _t->loadCirc((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->saveCirc(); break;
        case 5: { bool _r = _t->saveCircAs();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 6: _t->powerCirc(); break;
        case 7: _t->openInfo(); break;
        case 8: _t->about(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CircuitWidget::*)(const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CircuitWidget::dataAvailable)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CircuitWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CircuitWidget.data,
      qt_meta_data_CircuitWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CircuitWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CircuitWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CircuitWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CircuitWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void CircuitWidget::dataAvailable(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
