/****************************************************************************
** Meta object code from reading C++ file 'mcucomponent.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/circuitwidget/components/mcu/mcucomponent.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mcucomponent.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_McuComponent_t {
    QByteArrayData data[22];
    char stringdata0[224];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_McuComponent_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_McuComponent_t qt_meta_stringdata_McuComponent = {
    {
QT_MOC_LITERAL(0, 0, 12), // "McuComponent"
QT_MOC_LITERAL(1, 13, 9), // "terminate"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 6), // "remove"
QT_MOC_LITERAL(4, 31, 5), // "reset"
QT_MOC_LITERAL(5, 37, 4), // "load"
QT_MOC_LITERAL(6, 42, 8), // "fileName"
QT_MOC_LITERAL(7, 51, 8), // "slotLoad"
QT_MOC_LITERAL(8, 60, 10), // "slotReload"
QT_MOC_LITERAL(9, 71, 12), // "slotOpenTerm"
QT_MOC_LITERAL(10, 84, 13), // "slotCloseTerm"
QT_MOC_LITERAL(11, 98, 14), // "slotOpenSerial"
QT_MOC_LITERAL(12, 113, 15), // "slotCloseSerial"
QT_MOC_LITERAL(13, 129, 11), // "contextMenu"
QT_MOC_LITERAL(14, 141, 31), // "QGraphicsSceneContextMenuEvent*"
QT_MOC_LITERAL(15, 173, 5), // "event"
QT_MOC_LITERAL(16, 179, 6), // "QMenu*"
QT_MOC_LITERAL(17, 186, 4), // "menu"
QT_MOC_LITERAL(18, 191, 7), // "Program"
QT_MOC_LITERAL(19, 199, 3), // "Mhz"
QT_MOC_LITERAL(20, 203, 8), // "Ser_Port"
QT_MOC_LITERAL(21, 212, 11) // "Ser_Monitor"

    },
    "McuComponent\0terminate\0\0remove\0reset\0"
    "load\0fileName\0slotLoad\0slotReload\0"
    "slotOpenTerm\0slotCloseTerm\0slotOpenSerial\0"
    "slotCloseSerial\0contextMenu\0"
    "QGraphicsSceneContextMenuEvent*\0event\0"
    "QMenu*\0menu\0Program\0Mhz\0Ser_Port\0"
    "Ser_Monitor"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_McuComponent[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       4,   86, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x0a /* Public */,
       3,    0,   70,    2, 0x0a /* Public */,
       4,    0,   71,    2, 0x0a /* Public */,
       5,    1,   72,    2, 0x0a /* Public */,
       7,    0,   75,    2, 0x0a /* Public */,
       8,    0,   76,    2, 0x0a /* Public */,
       9,    0,   77,    2, 0x0a /* Public */,
      10,    0,   78,    2, 0x0a /* Public */,
      11,    0,   79,    2, 0x0a /* Public */,
      12,    0,   80,    2, 0x0a /* Public */,
      13,    2,   81,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14, 0x80000000 | 16,   15,   17,

 // properties: name, type, flags
      18, QMetaType::QString, 0x00195103,
      19, QMetaType::Int, 0x00195003,
      20, QMetaType::Bool, 0x00095003,
      21, QMetaType::Bool, 0x00095003,

       0        // eod
};

void McuComponent::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        McuComponent *_t = static_cast<McuComponent *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->terminate(); break;
        case 1: _t->remove(); break;
        case 2: _t->reset(); break;
        case 3: _t->load((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->slotLoad(); break;
        case 5: _t->slotReload(); break;
        case 6: _t->slotOpenTerm(); break;
        case 7: _t->slotCloseTerm(); break;
        case 8: _t->slotOpenSerial(); break;
        case 9: _t->slotCloseSerial(); break;
        case 10: _t->contextMenu((*reinterpret_cast< QGraphicsSceneContextMenuEvent*(*)>(_a[1])),(*reinterpret_cast< QMenu*(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMenu* >(); break;
            }
            break;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        McuComponent *_t = static_cast<McuComponent *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->program(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->freq(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->serPort(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->serMon(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        McuComponent *_t = static_cast<McuComponent *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setProgram(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setFreq(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setSerPort(*reinterpret_cast< bool*>(_v)); break;
        case 3: _t->setSerMon(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject McuComponent::staticMetaObject = {
    { &Chip::staticMetaObject, qt_meta_stringdata_McuComponent.data,
      qt_meta_data_McuComponent,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *McuComponent::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *McuComponent::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_McuComponent.stringdata0))
        return static_cast<void*>(this);
    return Chip::qt_metacast(_clname);
}

int McuComponent::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Chip::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
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
