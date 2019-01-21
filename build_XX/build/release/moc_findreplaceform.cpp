/****************************************************************************
** Meta object code from reading C++ file 'findreplaceform.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/editorwidget/findreplacedialog/findreplaceform.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'findreplaceform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FindReplaceForm_t {
    QByteArrayData data[13];
    char stringdata0[121];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FindReplaceForm_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FindReplaceForm_t qt_meta_stringdata_FindReplaceForm = {
    {
QT_MOC_LITERAL(0, 0, 15), // "FindReplaceForm"
QT_MOC_LITERAL(1, 16, 4), // "find"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 4), // "down"
QT_MOC_LITERAL(4, 27, 8), // "findNext"
QT_MOC_LITERAL(5, 36, 8), // "findPrev"
QT_MOC_LITERAL(6, 45, 7), // "replace"
QT_MOC_LITERAL(7, 53, 10), // "replaceAll"
QT_MOC_LITERAL(8, 64, 17), // "textToFindChanged"
QT_MOC_LITERAL(9, 82, 14), // "validateRegExp"
QT_MOC_LITERAL(10, 97, 4), // "text"
QT_MOC_LITERAL(11, 102, 14), // "regexpSelected"
QT_MOC_LITERAL(12, 117, 3) // "sel"

    },
    "FindReplaceForm\0find\0\0down\0findNext\0"
    "findPrev\0replace\0replaceAll\0"
    "textToFindChanged\0validateRegExp\0text\0"
    "regexpSelected\0sel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FindReplaceForm[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x0a /* Public */,
       1,    0,   62,    2, 0x0a /* Public */,
       4,    0,   63,    2, 0x0a /* Public */,
       5,    0,   64,    2, 0x0a /* Public */,
       6,    0,   65,    2, 0x0a /* Public */,
       7,    0,   66,    2, 0x0a /* Public */,
       8,    0,   67,    2, 0x09 /* Protected */,
       9,    1,   68,    2, 0x09 /* Protected */,
      11,    1,   71,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::Bool,   12,

       0        // eod
};

void FindReplaceForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FindReplaceForm *_t = static_cast<FindReplaceForm *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->find((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->find(); break;
        case 2: _t->findNext(); break;
        case 3: _t->findPrev(); break;
        case 4: _t->replace(); break;
        case 5: _t->replaceAll(); break;
        case 6: _t->textToFindChanged(); break;
        case 7: _t->validateRegExp((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->regexpSelected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FindReplaceForm::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FindReplaceForm.data,
      qt_meta_data_FindReplaceForm,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FindReplaceForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FindReplaceForm::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FindReplaceForm.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int FindReplaceForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
