/****************************************************************************
** Meta object code from reading C++ file 'logicinput.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/circuitwidget/components/logicinput.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'logicinput.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LogicInput_t {
    QByteArrayData data[7];
    char stringdata0[58];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LogicInput_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LogicInput_t qt_meta_stringdata_LogicInput = {
    {
QT_MOC_LITERAL(0, 0, 10), // "LogicInput"
QT_MOC_LITERAL(1, 11, 15), // "onbuttonclicked"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 6), // "remove"
QT_MOC_LITERAL(4, 35, 7), // "Voltage"
QT_MOC_LITERAL(5, 43, 4), // "Unit"
QT_MOC_LITERAL(6, 48, 9) // "Show_Volt"

    },
    "LogicInput\0onbuttonclicked\0\0remove\0"
    "Voltage\0Unit\0Show_Volt"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LogicInput[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       3,   26, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x0a /* Public */,
       3,    0,   25,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       4, QMetaType::Double, 0x00195003,
       5, QMetaType::QString, 0x00195103,
       6, QMetaType::Bool, 0x00195003,

       0        // eod
};

void LogicInput::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LogicInput *_t = static_cast<LogicInput *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onbuttonclicked(); break;
        case 1: _t->remove(); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        LogicInput *_t = static_cast<LogicInput *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = _t->volt(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->unit(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->showVal(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        LogicInput *_t = static_cast<LogicInput *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setVolt(*reinterpret_cast< double*>(_v)); break;
        case 1: _t->setUnit(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setShowVal(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject LogicInput::staticMetaObject = {
    { &Component::staticMetaObject, qt_meta_stringdata_LogicInput.data,
      qt_meta_data_LogicInput,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *LogicInput::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LogicInput::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LogicInput.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "eElement"))
        return static_cast< eElement*>(this);
    return Component::qt_metacast(_clname);
}

int LogicInput::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Component::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
