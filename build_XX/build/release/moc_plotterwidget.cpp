/****************************************************************************
** Meta object code from reading C++ file 'plotterwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/plotterwidget/plotterwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'plotterwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PlotterWidget_t {
    QByteArrayData data[8];
    char stringdata0[68];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PlotterWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PlotterWidget_t qt_meta_stringdata_PlotterWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "PlotterWidget"
QT_MOC_LITERAL(1, 14, 10), // "maxChanged"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 5), // "value"
QT_MOC_LITERAL(4, 32, 10), // "minChanged"
QT_MOC_LITERAL(5, 43, 8), // "itemtype"
QT_MOC_LITERAL(6, 52, 7), // "MaxVolt"
QT_MOC_LITERAL(7, 60, 7) // "MinVolt"

    },
    "PlotterWidget\0maxChanged\0\0value\0"
    "minChanged\0itemtype\0MaxVolt\0MinVolt"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PlotterWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       3,   30, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x0a /* Public */,
       4,    1,   27,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,

 // properties: name, type, flags
       5, QMetaType::QString, 0x00095001,
       6, QMetaType::Double, 0x00095103,
       7, QMetaType::Double, 0x00095103,

       0        // eod
};

void PlotterWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PlotterWidget *_t = static_cast<PlotterWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->maxChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->minChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        PlotterWidget *_t = static_cast<PlotterWidget *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->itemType(); break;
        case 1: *reinterpret_cast< double*>(_v) = _t->maxVolt(); break;
        case 2: *reinterpret_cast< double*>(_v) = _t->minVolt(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        PlotterWidget *_t = static_cast<PlotterWidget *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setMaxVolt(*reinterpret_cast< double*>(_v)); break;
        case 2: _t->setMinVolt(*reinterpret_cast< double*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject PlotterWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PlotterWidget.data,
      qt_meta_data_PlotterWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *PlotterWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PlotterWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PlotterWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int PlotterWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
