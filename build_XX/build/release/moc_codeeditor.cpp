/****************************************************************************
** Meta object code from reading C++ file 'codeeditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/editorwidget/codeeditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'codeeditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CodeEditor_t {
    QByteArrayData data[24];
    char stringdata0[247];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CodeEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CodeEditor_t qt_meta_stringdata_CodeEditor = {
    {
QT_MOC_LITERAL(0, 0, 10), // "CodeEditor"
QT_MOC_LITERAL(1, 11, 3), // "msg"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 4), // "text"
QT_MOC_LITERAL(4, 21, 12), // "stopDebbuger"
QT_MOC_LITERAL(5, 34, 12), // "slotAddBreak"
QT_MOC_LITERAL(6, 47, 12), // "slotRemBreak"
QT_MOC_LITERAL(7, 60, 9), // "timerTick"
QT_MOC_LITERAL(8, 70, 7), // "compile"
QT_MOC_LITERAL(9, 78, 6), // "upload"
QT_MOC_LITERAL(10, 85, 4), // "step"
QT_MOC_LITERAL(11, 90, 8), // "stepOver"
QT_MOC_LITERAL(12, 99, 5), // "pause"
QT_MOC_LITERAL(13, 105, 6), // "resume"
QT_MOC_LITERAL(14, 112, 5), // "reset"
QT_MOC_LITERAL(15, 118, 3), // "run"
QT_MOC_LITERAL(16, 122, 25), // "updateLineNumberAreaWidth"
QT_MOC_LITERAL(17, 148, 13), // "newBlockCount"
QT_MOC_LITERAL(18, 162, 20), // "updateLineNumberArea"
QT_MOC_LITERAL(19, 183, 20), // "highlightCurrentLine"
QT_MOC_LITERAL(20, 204, 9), // "Font_Size"
QT_MOC_LITERAL(21, 214, 8), // "Tab_Size"
QT_MOC_LITERAL(22, 223, 11), // "Spaces_Tabs"
QT_MOC_LITERAL(23, 235, 11) // "Show_Spaces"

    },
    "CodeEditor\0msg\0\0text\0stopDebbuger\0"
    "slotAddBreak\0slotRemBreak\0timerTick\0"
    "compile\0upload\0step\0stepOver\0pause\0"
    "resume\0reset\0run\0updateLineNumberAreaWidth\0"
    "newBlockCount\0updateLineNumberArea\0"
    "highlightCurrentLine\0Font_Size\0Tab_Size\0"
    "Spaces_Tabs\0Show_Spaces"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CodeEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       4,  118, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   97,    2, 0x0a /* Public */,
       5,    0,   98,    2, 0x0a /* Public */,
       6,    0,   99,    2, 0x0a /* Public */,
       7,    0,  100,    2, 0x0a /* Public */,
       8,    0,  101,    2, 0x0a /* Public */,
       9,    0,  102,    2, 0x0a /* Public */,
      10,    0,  103,    2, 0x0a /* Public */,
      11,    0,  104,    2, 0x0a /* Public */,
      12,    0,  105,    2, 0x0a /* Public */,
      13,    0,  106,    2, 0x0a /* Public */,
      14,    0,  107,    2, 0x0a /* Public */,
      15,    0,  108,    2, 0x0a /* Public */,
      16,    1,  109,    2, 0x08 /* Private */,
      18,    2,  112,    2, 0x08 /* Private */,
      19,    0,  117,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::QRect, QMetaType::Int,    2,    2,
    QMetaType::Void,

 // properties: name, type, flags
      20, QMetaType::Int, 0x00195003,
      21, QMetaType::Int, 0x00195003,
      22, QMetaType::Bool, 0x00195003,
      23, QMetaType::Bool, 0x00195003,

       0        // eod
};

void CodeEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CodeEditor *_t = static_cast<CodeEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->msg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->stopDebbuger(); break;
        case 2: _t->slotAddBreak(); break;
        case 3: _t->slotRemBreak(); break;
        case 4: _t->timerTick(); break;
        case 5: _t->compile(); break;
        case 6: _t->upload(); break;
        case 7: _t->step(); break;
        case 8: _t->stepOver(); break;
        case 9: _t->pause(); break;
        case 10: _t->resume(); break;
        case 11: _t->reset(); break;
        case 12: _t->run(); break;
        case 13: _t->updateLineNumberAreaWidth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->updateLineNumberArea((*reinterpret_cast< const QRect(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: _t->highlightCurrentLine(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CodeEditor::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CodeEditor::msg)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        CodeEditor *_t = static_cast<CodeEditor *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->fontSize(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->tabSize(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->spaceTabs(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->showSpaces(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        CodeEditor *_t = static_cast<CodeEditor *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setFontSize(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->setTabSize(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setSpaceTabs(*reinterpret_cast< bool*>(_v)); break;
        case 3: _t->setShowSpaces(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject CodeEditor::staticMetaObject = {
    { &QPlainTextEdit::staticMetaObject, qt_meta_stringdata_CodeEditor.data,
      qt_meta_data_CodeEditor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CodeEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CodeEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CodeEditor.stringdata0))
        return static_cast<void*>(this);
    return QPlainTextEdit::qt_metacast(_clname);
}

int CodeEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPlainTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
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

// SIGNAL 0
void CodeEditor::msg(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_LineNumberArea_t {
    QByteArrayData data[1];
    char stringdata0[15];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LineNumberArea_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LineNumberArea_t qt_meta_stringdata_LineNumberArea = {
    {
QT_MOC_LITERAL(0, 0, 14) // "LineNumberArea"

    },
    "LineNumberArea"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LineNumberArea[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void LineNumberArea::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject LineNumberArea::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_LineNumberArea.data,
      qt_meta_data_LineNumberArea,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *LineNumberArea::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LineNumberArea::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LineNumberArea.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LineNumberArea::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
