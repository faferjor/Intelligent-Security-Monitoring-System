/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
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
    "results",
    "cv::Mat",
    "frame"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSDetectionThreadENDCLASS_t {
    uint offsetsAndSizes[14];
    char stringdata0[16];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[29];
    char stringdata4[8];
    char stringdata5[8];
    char stringdata6[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSDetectionThreadENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSDetectionThreadENDCLASS_t qt_meta_stringdata_CLASSDetectionThreadENDCLASS = {
    {
        QT_MOC_LITERAL(0, 15),  // "DetectionThread"
        QT_MOC_LITERAL(16, 18),  // "detectionCompleted"
        QT_MOC_LITERAL(35, 0),  // ""
        QT_MOC_LITERAL(36, 28),  // "std::vector<DetectionResult>"
        QT_MOC_LITERAL(65, 7),  // "results"
        QT_MOC_LITERAL(73, 7),  // "cv::Mat"
        QT_MOC_LITERAL(81, 5)   // "frame"
    },
    "DetectionThread",
    "detectionCompleted",
    "",
    "std::vector<DetectionResult>",
    "results",
    "cv::Mat",
    "frame"
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
       1,    2,   20,    2, 0x06,    1 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,

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
        QtPrivate::TypeAndForceComplete<const std::vector<DetectionResult> &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const cv::Mat &, std::false_type>
    >,
    nullptr
} };

void DetectionThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DetectionThread *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->detectionCompleted((*reinterpret_cast< std::add_pointer_t<std::vector<DetectionResult>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<cv::Mat>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DetectionThread::*)(const std::vector<DetectionResult> & , const cv::Mat & );
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
void DetectionThread::detectionCompleted(const std::vector<DetectionResult> & _t1, const cv::Mat & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "onStartServerClicked",
    "",
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
    "cv::Mat",
    "updateLog",
    "onMotionDetectionChanged",
    "state",
    "onRegionDetectionChanged",
    "onLineCrossChanged",
    "onCrowdDetectionChanged",
    "onSetRegionClicked",
    "onSetLineClicked",
    "onSettingsClicked",
    "onAlarmRecordingTimeout"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {
    uint offsetsAndSizes[60];
    char stringdata0[11];
    char stringdata1[21];
    char stringdata2[1];
    char stringdata3[20];
    char stringdata4[21];
    char stringdata5[20];
    char stringdata6[17];
    char stringdata7[16];
    char stringdata8[18];
    char stringdata9[9];
    char stringdata10[21];
    char stringdata11[18];
    char stringdata12[8];
    char stringdata13[16];
    char stringdata14[6];
    char stringdata15[18];
    char stringdata16[21];
    char stringdata17[29];
    char stringdata18[8];
    char stringdata19[8];
    char stringdata20[10];
    char stringdata21[25];
    char stringdata22[6];
    char stringdata23[25];
    char stringdata24[19];
    char stringdata25[24];
    char stringdata26[19];
    char stringdata27[17];
    char stringdata28[18];
    char stringdata29[24];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMainWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMainWindowENDCLASS_t qt_meta_stringdata_CLASSMainWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 20),  // "onStartServerClicked"
        QT_MOC_LITERAL(32, 0),  // ""
        QT_MOC_LITERAL(33, 19),  // "onStopServerClicked"
        QT_MOC_LITERAL(53, 20),  // "onStartCameraClicked"
        QT_MOC_LITERAL(74, 19),  // "onStopCameraClicked"
        QT_MOC_LITERAL(94, 16),  // "onCaptureClicked"
        QT_MOC_LITERAL(111, 15),  // "onRecordClicked"
        QT_MOC_LITERAL(127, 17),  // "onClientConnected"
        QT_MOC_LITERAL(145, 8),  // "clientId"
        QT_MOC_LITERAL(154, 20),  // "onClientDisconnected"
        QT_MOC_LITERAL(175, 17),  // "onMessageReceived"
        QT_MOC_LITERAL(193, 7),  // "message"
        QT_MOC_LITERAL(201, 15),  // "onFrameReceived"
        QT_MOC_LITERAL(217, 5),  // "frame"
        QT_MOC_LITERAL(223, 17),  // "updateCameraFrame"
        QT_MOC_LITERAL(241, 20),  // "onDetectionCompleted"
        QT_MOC_LITERAL(262, 28),  // "std::vector<DetectionResult>"
        QT_MOC_LITERAL(291, 7),  // "results"
        QT_MOC_LITERAL(299, 7),  // "cv::Mat"
        QT_MOC_LITERAL(307, 9),  // "updateLog"
        QT_MOC_LITERAL(317, 24),  // "onMotionDetectionChanged"
        QT_MOC_LITERAL(342, 5),  // "state"
        QT_MOC_LITERAL(348, 24),  // "onRegionDetectionChanged"
        QT_MOC_LITERAL(373, 18),  // "onLineCrossChanged"
        QT_MOC_LITERAL(392, 23),  // "onCrowdDetectionChanged"
        QT_MOC_LITERAL(416, 18),  // "onSetRegionClicked"
        QT_MOC_LITERAL(435, 16),  // "onSetLineClicked"
        QT_MOC_LITERAL(452, 17),  // "onSettingsClicked"
        QT_MOC_LITERAL(470, 23)   // "onAlarmRecordingTimeout"
    },
    "MainWindow",
    "onStartServerClicked",
    "",
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
    "cv::Mat",
    "updateLog",
    "onMotionDetectionChanged",
    "state",
    "onRegionDetectionChanged",
    "onLineCrossChanged",
    "onCrowdDetectionChanged",
    "onSetRegionClicked",
    "onSetLineClicked",
    "onSettingsClicked",
    "onAlarmRecordingTimeout"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  140,    2, 0x08,    1 /* Private */,
       3,    0,  141,    2, 0x08,    2 /* Private */,
       4,    0,  142,    2, 0x08,    3 /* Private */,
       5,    0,  143,    2, 0x08,    4 /* Private */,
       6,    0,  144,    2, 0x08,    5 /* Private */,
       7,    0,  145,    2, 0x08,    6 /* Private */,
       8,    1,  146,    2, 0x08,    7 /* Private */,
      10,    1,  149,    2, 0x08,    9 /* Private */,
      11,    2,  152,    2, 0x08,   11 /* Private */,
      13,    2,  157,    2, 0x08,   14 /* Private */,
      15,    0,  162,    2, 0x08,   17 /* Private */,
      16,    2,  163,    2, 0x08,   18 /* Private */,
      20,    1,  168,    2, 0x08,   21 /* Private */,
      21,    1,  171,    2, 0x08,   23 /* Private */,
      23,    1,  174,    2, 0x08,   25 /* Private */,
      24,    1,  177,    2, 0x08,   27 /* Private */,
      25,    1,  180,    2, 0x08,   29 /* Private */,
      26,    0,  183,    2, 0x08,   31 /* Private */,
      27,    0,  184,    2, 0x08,   32 /* Private */,
      28,    0,  185,    2, 0x08,   33 /* Private */,
      29,    0,  186,    2, 0x08,   34 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    9,   12,
    QMetaType::Void, QMetaType::QString, QMetaType::QImage,    9,   14,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 17, 0x80000000 | 19,   18,   14,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

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
        QtPrivate::TypeAndForceComplete<const cv::Mat &, std::false_type>,
        // method 'updateLog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onMotionDetectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onRegionDetectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onLineCrossChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onCrowdDetectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onSetRegionClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSetLineClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSettingsClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAlarmRecordingTimeout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onStartServerClicked(); break;
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
        case 11: _t->onDetectionCompleted((*reinterpret_cast< std::add_pointer_t<std::vector<DetectionResult>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<cv::Mat>>(_a[2]))); break;
        case 12: _t->updateLog((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 13: _t->onMotionDetectionChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 14: _t->onRegionDetectionChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->onLineCrossChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->onCrowdDetectionChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 17: _t->onSetRegionClicked(); break;
        case 18: _t->onSetLineClicked(); break;
        case 19: _t->onSettingsClicked(); break;
        case 20: _t->onAlarmRecordingTimeout(); break;
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
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 21;
    }
    return _id;
}
QT_WARNING_POP
