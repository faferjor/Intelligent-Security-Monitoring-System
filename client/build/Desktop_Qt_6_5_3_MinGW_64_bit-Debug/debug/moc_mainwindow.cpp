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
struct qt_meta_stringdata_CLASSCameraThreadENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSCameraThreadENDCLASS = QtMocHelpers::stringData(
    "CameraThread",
    "frameCaptured",
    "",
    "cv::Mat",
    "frame"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSCameraThreadENDCLASS_t {
    uint offsetsAndSizes[10];
    char stringdata0[13];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSCameraThreadENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSCameraThreadENDCLASS_t qt_meta_stringdata_CLASSCameraThreadENDCLASS = {
    {
        QT_MOC_LITERAL(0, 12),  // "CameraThread"
        QT_MOC_LITERAL(13, 13),  // "frameCaptured"
        QT_MOC_LITERAL(27, 0),  // ""
        QT_MOC_LITERAL(28, 7),  // "cv::Mat"
        QT_MOC_LITERAL(36, 5)   // "frame"
    },
    "CameraThread",
    "frameCaptured",
    "",
    "cv::Mat",
    "frame"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSCameraThreadENDCLASS[] = {

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

Q_CONSTINIT const QMetaObject CameraThread::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_CLASSCameraThreadENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSCameraThreadENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSCameraThreadENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<CameraThread, std::true_type>,
        // method 'frameCaptured'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const cv::Mat &, std::false_type>
    >,
    nullptr
} };

void CameraThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CameraThread *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->frameCaptured((*reinterpret_cast< std::add_pointer_t<cv::Mat>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CameraThread::*)(const cv::Mat & );
            if (_t _q_method = &CameraThread::frameCaptured; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *CameraThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CameraThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSCameraThreadENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int CameraThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CameraThread::frameCaptured(const cv::Mat & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "onConnectClicked",
    "",
    "host",
    "port",
    "onDisconnectClicked",
    "onStartCameraClicked",
    "onStopCameraClicked",
    "onCaptureClicked",
    "onSendFrameClicked",
    "onServerConnected",
    "onServerDisconnected",
    "onConnectionError",
    "errorString",
    "onServerMessageReceived",
    "message",
    "onFrameReceived",
    "frame",
    "std::vector<DetectionResult>",
    "results",
    "updateCameraFrame",
    "cv::Mat",
    "updateLog"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {
    uint offsetsAndSizes[46];
    char stringdata0[11];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[5];
    char stringdata4[5];
    char stringdata5[20];
    char stringdata6[21];
    char stringdata7[20];
    char stringdata8[17];
    char stringdata9[19];
    char stringdata10[18];
    char stringdata11[21];
    char stringdata12[18];
    char stringdata13[12];
    char stringdata14[24];
    char stringdata15[8];
    char stringdata16[16];
    char stringdata17[6];
    char stringdata18[29];
    char stringdata19[8];
    char stringdata20[18];
    char stringdata21[8];
    char stringdata22[10];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMainWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMainWindowENDCLASS_t qt_meta_stringdata_CLASSMainWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 16),  // "onConnectClicked"
        QT_MOC_LITERAL(28, 0),  // ""
        QT_MOC_LITERAL(29, 4),  // "host"
        QT_MOC_LITERAL(34, 4),  // "port"
        QT_MOC_LITERAL(39, 19),  // "onDisconnectClicked"
        QT_MOC_LITERAL(59, 20),  // "onStartCameraClicked"
        QT_MOC_LITERAL(80, 19),  // "onStopCameraClicked"
        QT_MOC_LITERAL(100, 16),  // "onCaptureClicked"
        QT_MOC_LITERAL(117, 18),  // "onSendFrameClicked"
        QT_MOC_LITERAL(136, 17),  // "onServerConnected"
        QT_MOC_LITERAL(154, 20),  // "onServerDisconnected"
        QT_MOC_LITERAL(175, 17),  // "onConnectionError"
        QT_MOC_LITERAL(193, 11),  // "errorString"
        QT_MOC_LITERAL(205, 23),  // "onServerMessageReceived"
        QT_MOC_LITERAL(229, 7),  // "message"
        QT_MOC_LITERAL(237, 15),  // "onFrameReceived"
        QT_MOC_LITERAL(253, 5),  // "frame"
        QT_MOC_LITERAL(259, 28),  // "std::vector<DetectionResult>"
        QT_MOC_LITERAL(288, 7),  // "results"
        QT_MOC_LITERAL(296, 17),  // "updateCameraFrame"
        QT_MOC_LITERAL(314, 7),  // "cv::Mat"
        QT_MOC_LITERAL(322, 9)   // "updateLog"
    },
    "MainWindow",
    "onConnectClicked",
    "",
    "host",
    "port",
    "onDisconnectClicked",
    "onStartCameraClicked",
    "onStopCameraClicked",
    "onCaptureClicked",
    "onSendFrameClicked",
    "onServerConnected",
    "onServerDisconnected",
    "onConnectionError",
    "errorString",
    "onServerMessageReceived",
    "message",
    "onFrameReceived",
    "frame",
    "std::vector<DetectionResult>",
    "results",
    "updateCameraFrame",
    "cv::Mat",
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
       1,    2,   92,    2, 0x08,    1 /* Private */,
       5,    0,   97,    2, 0x08,    4 /* Private */,
       6,    0,   98,    2, 0x08,    5 /* Private */,
       7,    0,   99,    2, 0x08,    6 /* Private */,
       8,    0,  100,    2, 0x08,    7 /* Private */,
       9,    0,  101,    2, 0x08,    8 /* Private */,
      10,    0,  102,    2, 0x08,    9 /* Private */,
      11,    0,  103,    2, 0x08,   10 /* Private */,
      12,    1,  104,    2, 0x08,   11 /* Private */,
      14,    1,  107,    2, 0x08,   13 /* Private */,
      16,    2,  110,    2, 0x08,   15 /* Private */,
      20,    1,  115,    2, 0x08,   18 /* Private */,
      22,    1,  118,    2, 0x08,   20 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::UShort,    3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void, QMetaType::QImage, 0x80000000 | 18,   17,   19,
    QMetaType::Void, 0x80000000 | 21,   17,
    QMetaType::Void, QMetaType::QString,   15,

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
        // method 'onConnectClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<quint16, std::false_type>,
        // method 'onDisconnectClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onStartCameraClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onStopCameraClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onCaptureClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSendFrameClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onServerConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onServerDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onConnectionError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onServerMessageReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onFrameReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QImage &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<DetectionResult> &, std::false_type>,
        // method 'updateCameraFrame'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const cv::Mat &, std::false_type>,
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
        case 0: _t->onConnectClicked((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<quint16>>(_a[2]))); break;
        case 1: _t->onDisconnectClicked(); break;
        case 2: _t->onStartCameraClicked(); break;
        case 3: _t->onStopCameraClicked(); break;
        case 4: _t->onCaptureClicked(); break;
        case 5: _t->onSendFrameClicked(); break;
        case 6: _t->onServerConnected(); break;
        case 7: _t->onServerDisconnected(); break;
        case 8: _t->onConnectionError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->onServerMessageReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: _t->onFrameReceived((*reinterpret_cast< std::add_pointer_t<QImage>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::vector<DetectionResult>>>(_a[2]))); break;
        case 11: _t->updateCameraFrame((*reinterpret_cast< std::add_pointer_t<cv::Mat>>(_a[1]))); break;
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
