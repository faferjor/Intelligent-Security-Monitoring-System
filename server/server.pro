QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# OpenCV 头文件路径
INCLUDEPATH += E:/opencv/build/install/include
INCLUDEPATH += E:/opencv/build/install/include/opencv2

# OpenCV 库文件路径 + 链接库（关键修正）
LIBS += E:/opencv/build/lib/libopencv_*.a




# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    detectormodel.cpp \
    main.cpp \
    mainwindow.cpp \
    tcpserver.cpp \
    database.cpp

HEADERS += \
    detectormodel.h \
    mainwindow.h \
    tcpserver.h \
    database.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
