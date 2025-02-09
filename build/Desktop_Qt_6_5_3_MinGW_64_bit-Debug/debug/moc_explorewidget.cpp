/****************************************************************************
** Meta object code from reading C++ file 'explorewidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../explorewidget.h"
#include <QtGui/qtextcursor.h>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'explorewidget.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSExploreWidgetENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSExploreWidgetENDCLASS = QtMocHelpers::stringData(
    "ExploreWidget",
    "startSearch",
    "",
    "downLoadByBv",
    "title",
    "bvId",
    "cId",
    "searchTitleByBv",
    "searchByTitle",
    "sTitle",
    "on_selectDirBtn_clicked",
    "on_downloadBtn_clicked"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSExploreWidgetENDCLASS_t {
    uint offsetsAndSizes[24];
    char stringdata0[14];
    char stringdata1[12];
    char stringdata2[1];
    char stringdata3[13];
    char stringdata4[6];
    char stringdata5[5];
    char stringdata6[4];
    char stringdata7[16];
    char stringdata8[14];
    char stringdata9[7];
    char stringdata10[24];
    char stringdata11[23];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSExploreWidgetENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSExploreWidgetENDCLASS_t qt_meta_stringdata_CLASSExploreWidgetENDCLASS = {
    {
        QT_MOC_LITERAL(0, 13),  // "ExploreWidget"
        QT_MOC_LITERAL(14, 11),  // "startSearch"
        QT_MOC_LITERAL(26, 0),  // ""
        QT_MOC_LITERAL(27, 12),  // "downLoadByBv"
        QT_MOC_LITERAL(40, 5),  // "title"
        QT_MOC_LITERAL(46, 4),  // "bvId"
        QT_MOC_LITERAL(51, 3),  // "cId"
        QT_MOC_LITERAL(55, 15),  // "searchTitleByBv"
        QT_MOC_LITERAL(71, 13),  // "searchByTitle"
        QT_MOC_LITERAL(85, 6),  // "sTitle"
        QT_MOC_LITERAL(92, 23),  // "on_selectDirBtn_clicked"
        QT_MOC_LITERAL(116, 22)   // "on_downloadBtn_clicked"
    },
    "ExploreWidget",
    "startSearch",
    "",
    "downLoadByBv",
    "title",
    "bvId",
    "cId",
    "searchTitleByBv",
    "searchByTitle",
    "sTitle",
    "on_selectDirBtn_clicked",
    "on_downloadBtn_clicked"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSExploreWidgetENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   50,    2, 0x0a,    1 /* Public */,
       3,    3,   51,    2, 0x0a,    2 /* Public */,
       7,    1,   58,    2, 0x0a,    6 /* Public */,
       8,    1,   61,    2, 0x0a,    8 /* Public */,
      10,    0,   64,    2, 0x08,   10 /* Private */,
      11,    0,   65,    2, 0x08,   11 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    4,    5,    6,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject ExploreWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSExploreWidgetENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSExploreWidgetENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSExploreWidgetENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ExploreWidget, std::true_type>,
        // method 'startSearch'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'downLoadByBv'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'searchTitleByBv'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'searchByTitle'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'on_selectDirBtn_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_downloadBtn_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void ExploreWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ExploreWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->startSearch(); break;
        case 1: _t->downLoadByBv((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 2: _t->searchTitleByBv((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->searchByTitle((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->on_selectDirBtn_clicked(); break;
        case 5: _t->on_downloadBtn_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject *ExploreWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ExploreWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSExploreWidgetENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ExploreWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
