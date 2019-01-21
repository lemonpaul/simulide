/****************************************************************************
** Meta object code from reading C++ file 'serialportwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/serialporwidget/serialportwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serialportwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SerialPortWidget_t {
    QByteArrayData data[13];
    char stringdata0[152];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialPortWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialPortWidget_t qt_meta_stringdata_SerialPortWidget = {
    {
QT_MOC_LITERAL(0, 0, 16), // "SerialPortWidget"
QT_MOC_LITERAL(1, 17, 7), // "getData"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 4), // "data"
QT_MOC_LITERAL(4, 31, 13), // "slotWriteData"
QT_MOC_LITERAL(5, 45, 25), // "checkCustomBaudRatePolicy"
QT_MOC_LITERAL(6, 71, 3), // "idx"
QT_MOC_LITERAL(7, 75, 27), // "checkCustomDevicePathPolicy"
QT_MOC_LITERAL(8, 103, 12), // "showPortInfo"
QT_MOC_LITERAL(9, 116, 4), // "open"
QT_MOC_LITERAL(10, 121, 8), // "readData"
QT_MOC_LITERAL(11, 130, 8), // "itemtype"
QT_MOC_LITERAL(12, 139, 12) // "SettingsProp"

    },
    "SerialPortWidget\0getData\0\0data\0"
    "slotWriteData\0checkCustomBaudRatePolicy\0"
    "idx\0checkCustomDevicePathPolicy\0"
    "showPortInfo\0open\0readData\0itemtype\0"
    "SettingsProp"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialPortWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       2,   66, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   52,    2, 0x0a /* Public */,
       5,    1,   55,    2, 0x08 /* Private */,
       7,    1,   58,    2, 0x08 /* Private */,
       8,    1,   61,    2, 0x08 /* Private */,
       9,    0,   64,    2, 0x08 /* Private */,
      10,    0,   65,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      11, QMetaType::QString, 0x00095001,
      12, QMetaType::QStringList, 0x00095103,

       0        // eod
};

void SerialPortWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SerialPortWidget *_t = static_cast<SerialPortWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->getData((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->slotWriteData((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 2: _t->checkCustomBaudRatePolicy((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->checkCustomDevicePathPolicy((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->showPortInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->open(); break;
        case 6: _t->readData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SerialPortWidget::*)(const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortWidget::getData)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        SerialPortWidget *_t = static_cast<SerialPortWidget *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->itemType(); break;
        case 1: *reinterpret_cast< QStringList*>(_v) = _t->settingsProp(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        SerialPortWidget *_t = static_cast<SerialPortWidget *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setSettingsProp(*reinterpret_cast< QStringList*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject SerialPortWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SerialPortWidget.data,
      qt_meta_data_SerialPortWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SerialPortWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialPortWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SerialPortWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SerialPortWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void SerialPortWidget::getData(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
