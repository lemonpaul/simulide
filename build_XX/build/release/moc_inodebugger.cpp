/****************************************************************************
** Meta object code from reading C++ file 'inodebugger.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/editorwidget/inodebugger.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'inodebugger.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_InoDebugger_t {
    QByteArrayData data[10];
    char stringdata0[81];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_InoDebugger_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_InoDebugger_t qt_meta_stringdata_InoDebugger = {
    {
QT_MOC_LITERAL(0, 0, 11), // "InoDebugger"
QT_MOC_LITERAL(1, 12, 5), // "Board"
QT_MOC_LITERAL(2, 18, 7), // "board_t"
QT_MOC_LITERAL(3, 26, 12), // "Custom_Board"
QT_MOC_LITERAL(4, 39, 3), // "Uno"
QT_MOC_LITERAL(5, 43, 4), // "Mega"
QT_MOC_LITERAL(6, 48, 4), // "Nano"
QT_MOC_LITERAL(7, 53, 11), // "Duemilanove"
QT_MOC_LITERAL(8, 65, 8), // "Leonardo"
QT_MOC_LITERAL(9, 74, 6) // "Custom"

    },
    "InoDebugger\0Board\0board_t\0Custom_Board\0"
    "Uno\0Mega\0Nano\0Duemilanove\0Leonardo\0"
    "Custom"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_InoDebugger[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       2,   14, // properties
       1,   20, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, 0x80000000 | 2, 0x0019510b,
       3, QMetaType::QString, 0x00195003,

 // enums: name, flags, count, data
       2, 0x0,    6,   24,

 // enum data: key, value
       4, uint(InoDebugger::Uno),
       5, uint(InoDebugger::Mega),
       6, uint(InoDebugger::Nano),
       7, uint(InoDebugger::Duemilanove),
       8, uint(InoDebugger::Leonardo),
       9, uint(InoDebugger::Custom),

       0        // eod
};

void InoDebugger::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        InoDebugger *_t = static_cast<InoDebugger *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< board_t*>(_v) = _t->board(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->customBoard(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        InoDebugger *_t = static_cast<InoDebugger *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setBoard(*reinterpret_cast< board_t*>(_v)); break;
        case 1: _t->setCustomBoard(*reinterpret_cast< QString*>(_v)); break;
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

QT_INIT_METAOBJECT const QMetaObject InoDebugger::staticMetaObject = {
    { &BaseDebugger::staticMetaObject, qt_meta_stringdata_InoDebugger.data,
      qt_meta_data_InoDebugger,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *InoDebugger::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InoDebugger::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_InoDebugger.stringdata0))
        return static_cast<void*>(this);
    return BaseDebugger::qt_metacast(_clname);
}

int InoDebugger::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseDebugger::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
   if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
