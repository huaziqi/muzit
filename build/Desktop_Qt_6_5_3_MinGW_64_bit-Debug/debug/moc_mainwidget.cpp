/****************************************************************************
** Meta object code from reading C++ file 'mainwidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../mainwidget.h"
#include <QtGui/qtextcursor.h>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWidgetENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSMainWidgetENDCLASS = QtMocHelpers::stringData(
    "MainWidget",
    "receiveUrl",
    "",
    "QList<MusicInfo>",
    "newMusics",
    "QDir",
    "musicDir",
    "changeVolume",
    "volume",
    "on_play_lists_clicked"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMainWidgetENDCLASS_t {
    uint offsetsAndSizes[20];
    char stringdata0[11];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[17];
    char stringdata4[10];
    char stringdata5[5];
    char stringdata6[9];
    char stringdata7[13];
    char stringdata8[7];
    char stringdata9[22];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMainWidgetENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMainWidgetENDCLASS_t qt_meta_stringdata_CLASSMainWidgetENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWidget"
        QT_MOC_LITERAL(11, 10),  // "receiveUrl"
        QT_MOC_LITERAL(22, 0),  // ""
        QT_MOC_LITERAL(23, 16),  // "QList<MusicInfo>"
        QT_MOC_LITERAL(40, 9),  // "newMusics"
        QT_MOC_LITERAL(50, 4),  // "QDir"
        QT_MOC_LITERAL(55, 8),  // "musicDir"
        QT_MOC_LITERAL(64, 12),  // "changeVolume"
        QT_MOC_LITERAL(77, 6),  // "volume"
        QT_MOC_LITERAL(84, 21)   // "on_play_lists_clicked"
    },
    "MainWidget",
    "receiveUrl",
    "",
    "QList<MusicInfo>",
    "newMusics",
    "QDir",
    "musicDir",
    "changeVolume",
    "volume",
    "on_play_lists_clicked"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWidgetENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   32,    2, 0x08,    1 /* Private */,
       7,    1,   37,    2, 0x08,    4 /* Private */,
       9,    0,   40,    2, 0x08,    6 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWidgetENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWidgetENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWidgetENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWidget, std::true_type>,
        // method 'receiveUrl'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QList<MusicInfo>, std::false_type>,
        QtPrivate::TypeAndForceComplete<QDir, std::false_type>,
        // method 'changeVolume'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_play_lists_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->receiveUrl((*reinterpret_cast< std::add_pointer_t<QList<MusicInfo>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QDir>>(_a[2]))); break;
        case 1: _t->changeVolume((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->on_play_lists_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject *MainWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWidgetENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MainWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
