/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSDetectionThreadENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSDetectionThreadENDCLASS = QtMocHelpers::stringData(
    "DetectionThread",
    "detectionCompleted",
    "",
    "std::vector<DetectionResult>",
    "results"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSDetectionThreadENDCLASS_t {
    uint offsetsAndSizes[10];
    char stringdata0[16];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[29];
    char stringdata4[8];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSDetectionThreadENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSDetectionThreadENDCLASS_t qt_meta_stringdata_CLASSDetectionThreadENDCLASS = {
    {
        QT_MOC_LITERAL(0, 15),  // "DetectionThread"
        QT_MOC_LITERAL(16, 18),  // "detectionCompleted"
        QT_MOC_LITERAL(35, 0),  // ""
        QT_MOC_LITERAL(36, 28),  // "std::vector<DetectionResult>"
        QT_MOC_LITERAL(65, 7)   // "results"
    },
    "DetectionThread",
    "detectionCompleted",
    "",
    "std::vector<DetectionResult>",
    "results"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSDetectionThreadENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   20,    2, 0x06,    1 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

Q_CONSTINIT const QMetaObject DetectionThread::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_CLASSDetectionThreadENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSDetectionThreadENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSDetectionThreadENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<DetectionThread, std::true_type>,
        // method 'detectionCompleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<DetectionResult> &, std::false_type>
    >,
    nullptr
} };

void DetectionThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DetectionThread *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->detectionCompleted((*reinterpret_cast< std::add_pointer_t<std::vector<DetectionResult>>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DetectionThread::*)(const std::vector<DetectionResult> & );
            if (_t _q_method = &DetectionThread::detectionCompleted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *DetectionThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DetectionThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSDetectionThreadENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int DetectionThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void DetectionThread::detectionCompleted(const std::vector<DetectionResult> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "onStartServerClicked",
    "",
    "port",
    "onStopServerClicked",
    "onStartCameraClicked",
    "onStopCameraClicked",
    "onCaptureClicked",
    "onRecordClicked",
    "onClientConnected",
    "clientId",
    "onClientDisconnected",
    "onMessageReceived",
    "message",
    "onFrameReceived",
    "frame",
    "updateCameraFrame",
    "onDetectionCompleted",
    "std::vector<DetectionResult>",
    "results",
    "updateLog"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {
    uint offsetsAndSizes[42];
    char stringdata0[11];
    char stringdata1[21];
    char stringdata2[1];
    char stringdata3[5];
    char stringdata4[20];
    char stringdata5[21];
    char stringdata6[20];
    char stringdata7[17];
    char stringdata8[16];
    char stringdata9[18];
    char stringdata10[9];
    char stringdata11[21];
    char stringdata12[18];
    char stringdata13[8];
    char stringdata14[16];
    char stringdata15[6];
    char stringdata16[18];
    char stringdata17[21];
    char stringdata18[29];
    char stringdata19[8];
    char stringdata20[10];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMainWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMainWindowENDCLASS_t qt_meta_stringdata_CLASSMainWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 20),  // "onStartServerClicked"
        QT_MOC_LITERAL(32, 0),  // ""
        QT_MOC_LITERAL(33, 4),  // "port"
        QT_MOC_LITERAL(38, 19),  // "onStopServerClicked"
        QT_MOC_LITERAL(58, 20),  // "onStartCameraClicked"
        QT_MOC_LITERAL(79, 19),  // "onStopCameraClicked"
        QT_MOC_LITERAL(99, 16),  // "onCaptureClicked"
        QT_MOC_LITERAL(116, 15),  // "onRecordClicked"
        QT_MOC_LITERAL(132, 17),  // "onClientConnected"
        QT_MOC_LITERAL(150, 8),  // "clientId"
        QT_MOC_LITERAL(159, 20),  // "onClientDisconnected"
        QT_MOC_LITERAL(180, 17),  // "onMessageReceived"
        QT_MOC_LITERAL(198, 7),  // "message"
        QT_MOC_LITERAL(206, 15),  // "onFrameReceived"
        QT_MOC_LITERAL(222, 5),  // "frame"
        QT_MOC_LITERAL(228, 17),  // "updateCameraFrame"
        QT_MOC_LITERAL(246, 20),  // "onDetectionCompleted"
        QT_MOC_LITERAL(267, 28),  // "std::vector<DetectionResult>"
        QT_MOC_LITERAL(296, 7),  // "results"
        QT_MOC_LITERAL(304, 9)   // "updateLog"
    },
    "MainWindow",
    "onStartServerClicked",
    "",
    "port",
    "onStopServerClicked",
    "onStartCameraClicked",
    "onStopCameraClicked",
    "onCaptureClicked",
    "onRecordClicked",
    "onClientConnected",
    "clientId",
    "onClientDisconnected",
    "onMessageReceived",
    "message",
    "onFrameReceived",
    "frame",
    "updateCameraFrame",
    "onDetectionCompleted",
    "std::vector<DetectionResult>",
    "results",
    "updateLog"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   92,    2, 0x08,    1 /* Private */,
       4,    0,   95,    2, 0x08,    3 /* Private */,
       5,    0,   96,    2, 0x08,    4 /* Private */,
       6,    0,   97,    2, 0x08,    5 /* Private */,
       7,    0,   98,    2, 0x08,    6 /* Private */,
       8,    0,   99,    2, 0x08,    7 /* Private */,
       9,    1,  100,    2, 0x08,    8 /* Private */,
      11,    1,  103,    2, 0x08,   10 /* Private */,
      12,    2,  106,    2, 0x08,   12 /* Private */,
      14,    2,  111,    2, 0x08,   15 /* Private */,
      16,    0,  116,    2, 0x08,   18 /* Private */,
      17,    1,  117,    2, 0x08,   19 /* Private */,
      20,    1,  120,    2, 0x08,   21 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::UShort,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   10,   13,
    QMetaType::Void, QMetaType::QString, QMetaType::QImage,   10,   15,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void, QMetaType::QString,   13,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'onStartServerClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<quint16, std::false_type>,
        // method 'onStopServerClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onStartCameraClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onStopCameraClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onCaptureClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onRecordClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onClientConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onClientDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onMessageReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onFrameReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QImage &, std::false_type>,
        // method 'updateCameraFrame'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDetectionCompleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<DetectionResult> &, std::false_type>,
        // method 'updateLog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onStartServerClicked((*reinterpret_cast< std::add_pointer_t<quint16>>(_a[1]))); break;
        case 1: _t->onStopServerClicked(); break;
        case 2: _t->onStartCameraClicked(); break;
        case 3: _t->onStopCameraClicked(); break;
        case 4: _t->onCaptureClicked(); break;
        case 5: _t->onRecordClicked(); break;
        case 6: _t->onClientConnected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->onClientDisconnected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->onMessageReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 9: _t->onFrameReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QImage>>(_a[2]))); break;
        case 10: _t->updateCameraFrame(); break;
        case 11: _t->onDetectionCompleted((*reinterpret_cast< std::add_pointer_t<std::vector<DetectionResult>>>(_a[1]))); break;
        case 12: _t->updateLog((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
