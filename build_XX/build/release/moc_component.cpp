/****************************************************************************
** Meta object code from reading C++ file 'component.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/circuitwidget/component.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'component.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Component_t {
    QByteArrayData data[26];
    char stringdata0[201];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Component_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Component_t qt_meta_stringdata_Component = {
    {
QT_MOC_LITERAL(0, 0, 9), // "Component"
QT_MOC_LITERAL(1, 10, 5), // "moved"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 14), // "slotProperties"
QT_MOC_LITERAL(4, 32, 8), // "rotateCW"
QT_MOC_LITERAL(5, 41, 9), // "rotateCCW"
QT_MOC_LITERAL(6, 51, 10), // "rotateHalf"
QT_MOC_LITERAL(7, 62, 6), // "H_flip"
QT_MOC_LITERAL(8, 69, 6), // "V_flip"
QT_MOC_LITERAL(9, 76, 10), // "slotRemove"
QT_MOC_LITERAL(10, 87, 8), // "slotCopy"
QT_MOC_LITERAL(11, 96, 6), // "remove"
QT_MOC_LITERAL(12, 103, 8), // "itemtype"
QT_MOC_LITERAL(13, 112, 2), // "id"
QT_MOC_LITERAL(14, 115, 7), // "Show_id"
QT_MOC_LITERAL(15, 123, 8), // "rotation"
QT_MOC_LITERAL(16, 132, 1), // "x"
QT_MOC_LITERAL(17, 134, 1), // "y"
QT_MOC_LITERAL(18, 136, 6), // "labelx"
QT_MOC_LITERAL(19, 143, 6), // "labely"
QT_MOC_LITERAL(20, 150, 8), // "labelrot"
QT_MOC_LITERAL(21, 159, 9), // "valLabelx"
QT_MOC_LITERAL(22, 169, 9), // "valLabely"
QT_MOC_LITERAL(23, 179, 9), // "valLabRot"
QT_MOC_LITERAL(24, 189, 5), // "hflip"
QT_MOC_LITERAL(25, 195, 5) // "vflip"

    },
    "Component\0moved\0\0slotProperties\0"
    "rotateCW\0rotateCCW\0rotateHalf\0H_flip\0"
    "V_flip\0slotRemove\0slotCopy\0remove\0"
    "itemtype\0id\0Show_id\0rotation\0x\0y\0"
    "labelx\0labely\0labelrot\0valLabelx\0"
    "valLabely\0valLabRot\0hflip\0vflip"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Component[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
      14,   74, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   65,    2, 0x0a /* Public */,
       4,    0,   66,    2, 0x0a /* Public */,
       5,    0,   67,    2, 0x0a /* Public */,
       6,    0,   68,    2, 0x0a /* Public */,
       7,    0,   69,    2, 0x0a /* Public */,
       8,    0,   70,    2, 0x0a /* Public */,
       9,    0,   71,    2, 0x0a /* Public */,
      10,    0,   72,    2, 0x0a /* Public */,
      11,    0,   73,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      12, QMetaType::QString, 0x00095001,
      13, QMetaType::QString, 0x00195103,
      14, QMetaType::Bool, 0x00195003,
      15, QMetaType::QReal, 0x00095103,
      16, QMetaType::Int, 0x00095103,
      17, QMetaType::Int, 0x00095103,
      18, QMetaType::Int, 0x00095003,
      19, QMetaType::Int, 0x00095003,
      20, QMetaType::Int, 0x00095003,
      21, QMetaType::Int, 0x00095003,
      22, QMetaType::Int, 0x00095003,
      23, QMetaType::Int, 0x00095103,
      24, QMetaType::Int, 0x00095103,
      25, QMetaType::Int, 0x00095103,

       0        // eod
};

void Component::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Component *_t = static_cast<Component *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->moved(); break;
        case 1: _t->slotProperties(); break;
        case 2: _t->rotateCW(); break;
        case 3: _t->rotateCCW(); break;
        case 4: _t->rotateHalf(); break;
        case 5: _t->H_flip(); break;
        case 6: _t->V_flip(); break;
        case 7: _t->slotRemove(); break;
        case 8: _t->slotCopy(); break;
        case 9: _t->remove(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Component::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Component::moved)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        Component *_t = static_cast<Component *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->itemType(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->itemID(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->showId(); break;
        case 3: *reinterpret_cast< qreal*>(_v) = _t->rotation(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->x(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->y(); break;
        case 6: *reinterpret_cast< int*>(_v) = _t->labelx(); break;
        case 7: *reinterpret_cast< int*>(_v) = _t->labely(); break;
        case 8: *reinterpret_cast< int*>(_v) = _t->labelRot(); break;
        case 9: *reinterpret_cast< int*>(_v) = _t->valLabelx(); break;
        case 10: *reinterpret_cast< int*>(_v) = _t->valLabely(); break;
        case 11: *reinterpret_cast< int*>(_v) = _t->valLabRot(); break;
        case 12: *reinterpret_cast< int*>(_v) = _t->hflip(); break;
        case 13: *reinterpret_cast< int*>(_v) = _t->vflip(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        Component *_t = static_cast<Component *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setId(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setShowId(*reinterpret_cast< bool*>(_v)); break;
        case 3: _t->setRotation(*reinterpret_cast< qreal*>(_v)); break;
        case 4: _t->setX(*reinterpret_cast< int*>(_v)); break;
        case 5: _t->setY(*reinterpret_cast< int*>(_v)); break;
        case 6: _t->setLabelX(*reinterpret_cast< int*>(_v)); break;
        case 7: _t->setLabelY(*reinterpret_cast< int*>(_v)); break;
        case 8: _t->setLabelRot(*reinterpret_cast< int*>(_v)); break;
        case 9: _t->setValLabelX(*reinterpret_cast< int*>(_v)); break;
        case 10: _t->setValLabelY(*reinterpret_cast< int*>(_v)); break;
        case 11: _t->setValLabRot(*reinterpret_cast< int*>(_v)); break;
        case 12: _t->setHflip(*reinterpret_cast< int*>(_v)); break;
        case 13: _t->setVflip(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject Component::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Component.data,
      qt_meta_data_Component,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Component::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Component::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Component.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QGraphicsItem"))
        return static_cast< QGraphicsItem*>(this);
    if (!strcmp(_clname, "org.qt-project.Qt.QGraphicsItem"))
        return static_cast< QGraphicsItem*>(this);
    return QObject::qt_metacast(_clname);
}

int Component::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 14;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Component::moved()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_Label_t {
    QByteArrayData data[10];
    char stringdata0[71];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Label_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Label_t qt_meta_stringdata_Label = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Label"
QT_MOC_LITERAL(1, 6, 8), // "rotateCW"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 9), // "rotateCCW"
QT_MOC_LITERAL(4, 26, 9), // "rotate180"
QT_MOC_LITERAL(5, 36, 6), // "H_flip"
QT_MOC_LITERAL(6, 43, 2), // "hf"
QT_MOC_LITERAL(7, 46, 6), // "V_flip"
QT_MOC_LITERAL(8, 53, 2), // "vf"
QT_MOC_LITERAL(9, 56, 14) // "updateGeometry"

    },
    "Label\0rotateCW\0\0rotateCCW\0rotate180\0"
    "H_flip\0hf\0V_flip\0vf\0updateGeometry"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Label[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x0a /* Public */,
       3,    0,   45,    2, 0x0a /* Public */,
       4,    0,   46,    2, 0x0a /* Public */,
       5,    1,   47,    2, 0x0a /* Public */,
       7,    1,   50,    2, 0x0a /* Public */,
       9,    3,   53,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    2,    2,    2,

       0        // eod
};

void Label::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Label *_t = static_cast<Label *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->rotateCW(); break;
        case 1: _t->rotateCCW(); break;
        case 2: _t->rotate180(); break;
        case 3: _t->H_flip((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->V_flip((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->updateGeometry((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Label::staticMetaObject = {
    { &QGraphicsTextItem::staticMetaObject, qt_meta_stringdata_Label.data,
      qt_meta_data_Label,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Label::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Label::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Label.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsTextItem::qt_metacast(_clname);
}

int Label::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsTextItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
