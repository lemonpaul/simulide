/****************************************************************************
** Meta object code from reading C++ file 'textcomponent.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/circuitwidget/components/textcomponent.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'textcomponent.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TextComponent_t {
    QByteArrayData data[9];
    char stringdata0[76];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TextComponent_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TextComponent_t qt_meta_stringdata_TextComponent = {
    {
QT_MOC_LITERAL(0, 0, 13), // "TextComponent"
QT_MOC_LITERAL(1, 14, 14), // "updateGeometry"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 4), // "Text"
QT_MOC_LITERAL(4, 35, 4), // "Font"
QT_MOC_LITERAL(5, 40, 11), // "Fixed_Width"
QT_MOC_LITERAL(6, 52, 9), // "Font_Size"
QT_MOC_LITERAL(7, 62, 6), // "Margin"
QT_MOC_LITERAL(8, 69, 6) // "Border"

    },
    "TextComponent\0updateGeometry\0\0Text\0"
    "Font\0Fixed_Width\0Font_Size\0Margin\0"
    "Border"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TextComponent[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       6,   26, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    2,    2,    2,

 // properties: name, type, flags
       3, QMetaType::QString, 0x00095103,
       4, QMetaType::QString, 0x00195103,
       5, QMetaType::Bool, 0x00195003,
       6, QMetaType::Int, 0x00195003,
       7, QMetaType::Int, 0x00195103,
       8, QMetaType::Int, 0x00195103,

       0        // eod
};

void TextComponent::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TextComponent *_t = static_cast<TextComponent *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateGeometry((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        TextComponent *_t = static_cast<TextComponent *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getText(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->getFont(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->fixedW(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->fontSize(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->margin(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->border(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        TextComponent *_t = static_cast<TextComponent *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setText(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setFont(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setFixedW(*reinterpret_cast< bool*>(_v)); break;
        case 3: _t->setFontSize(*reinterpret_cast< int*>(_v)); break;
        case 4: _t->setMargin(*reinterpret_cast< int*>(_v)); break;
        case 5: _t->setBorder(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject TextComponent::staticMetaObject = {
    { &Component::staticMetaObject, qt_meta_stringdata_TextComponent.data,
      qt_meta_data_TextComponent,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TextComponent::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TextComponent::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TextComponent.stringdata0))
        return static_cast<void*>(this);
    return Component::qt_metacast(_clname);
}

int TextComponent::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Component::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
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
