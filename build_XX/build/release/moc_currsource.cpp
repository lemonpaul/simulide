/****************************************************************************
** Meta object code from reading C++ file 'currsource.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/circuitwidget/components/currsource.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'currsource.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CurrSource_t {
    QByteArrayData data[9];
    char stringdata0[74];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CurrSource_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CurrSource_t qt_meta_stringdata_CurrSource = {
    {
QT_MOC_LITERAL(0, 0, 10), // "CurrSource"
QT_MOC_LITERAL(1, 11, 15), // "onbuttonclicked"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 11), // "currChanged"
QT_MOC_LITERAL(4, 40, 4), // "volt"
QT_MOC_LITERAL(5, 45, 6), // "remove"
QT_MOC_LITERAL(6, 52, 7), // "Current"
QT_MOC_LITERAL(7, 60, 4), // "Unit"
QT_MOC_LITERAL(8, 65, 8) // "Show_Amp"

    },
    "CurrSource\0onbuttonclicked\0\0currChanged\0"
    "volt\0remove\0Current\0Unit\0Show_Amp"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CurrSource[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       3,   34, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a /* Public */,
       3,    1,   30,    2, 0x0a /* Public */,
       5,    0,   33,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,

 // properties: name, type, flags
       6, QMetaType::Double, 0x00195103,
       7, QMetaType::QString, 0x00195103,
       8, QMetaType::Bool, 0x00195003,

       0        // eod
};

void CurrSource::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CurrSource *_t = static_cast<CurrSource *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onbuttonclicked(); break;
        case 1: _t->currChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->remove(); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        CurrSource *_t = static_cast<CurrSource *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = _t->current(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->unit(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->showVal(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        CurrSource *_t = static_cast<CurrSource *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setCurrent(*reinterpret_cast< double*>(_v)); break;
        case 1: _t->setUnit(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setShowVal(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject CurrSource::staticMetaObject = {
    { &Component::staticMetaObject, qt_meta_stringdata_CurrSource.data,
      qt_meta_data_CurrSource,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CurrSource::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CurrSource::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CurrSource.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "eElement"))
        return static_cast< eElement*>(this);
    return Component::qt_metacast(_clname);
}

int CurrSource::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Component::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
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
