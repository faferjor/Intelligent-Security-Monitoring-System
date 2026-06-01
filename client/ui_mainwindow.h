/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QSplitter *splitter;
    QWidget *videoPanel;
    QVBoxLayout *videoPanelLayout;
    QGroupBox *videoGroup;
    QHBoxLayout *videoGroupLayout;
    QWidget *localVideoContainer;
    QVBoxLayout *localVideoContainerLayout;
    QLabel *localLabelTitle;
    QLabel *localVideoLabel;
    QWidget *remoteVideoContainer;
    QVBoxLayout *remoteVideoContainerLayout;
    QLabel *remoteLabelTitle;
    QLabel *remoteVideoLabel;
    QWidget *controlPanel;
    QVBoxLayout *controlPanelLayout;
    QGroupBox *serverGroup;
    QVBoxLayout *serverGroupLayout;
    QHBoxLayout *serverInputLayout;
    QLabel *hostLabel;
    QLineEdit *hostEdit;
    QLabel *portLabel;
    QLineEdit *portEdit;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *serverBtnLayout;
    QPushButton *connectBtn;
    QPushButton *disconnectBtn;
    QGroupBox *cameraGroup;
    QHBoxLayout *cameraBtnLayout;
    QPushButton *startCameraBtn;
    QPushButton *stopCameraBtn;
    QPushButton *captureBtn;
    QGroupBox *logGroup;
    QVBoxLayout *logGroupLayout;
    QTextEdit *logTextEdit;
    QSpacerItem *verticalSpacer;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1000, 700);
        MainWindow->setMinimumSize(QSize(1000, 700));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName("centralWidget");
        mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->setObjectName("mainLayout");
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Horizontal);
        videoPanel = new QWidget(splitter);
        videoPanel->setObjectName("videoPanel");
        videoPanelLayout = new QVBoxLayout(videoPanel);
        videoPanelLayout->setObjectName("videoPanelLayout");
        videoPanelLayout->setContentsMargins(0, 0, 0, 0);
        videoGroup = new QGroupBox(videoPanel);
        videoGroup->setObjectName("videoGroup");
        videoGroupLayout = new QHBoxLayout(videoGroup);
        videoGroupLayout->setObjectName("videoGroupLayout");
        localVideoContainer = new QWidget(videoGroup);
        localVideoContainer->setObjectName("localVideoContainer");
        localVideoContainerLayout = new QVBoxLayout(localVideoContainer);
        localVideoContainerLayout->setObjectName("localVideoContainerLayout");
        localVideoContainerLayout->setContentsMargins(0, 0, 0, 0);
        localLabelTitle = new QLabel(localVideoContainer);
        localLabelTitle->setObjectName("localLabelTitle");
        localLabelTitle->setAlignment(Qt::AlignCenter);

        localVideoContainerLayout->addWidget(localLabelTitle);

        localVideoLabel = new QLabel(localVideoContainer);
        localVideoLabel->setObjectName("localVideoLabel");
        localVideoLabel->setMinimumSize(QSize(320, 240));
        localVideoLabel->setStyleSheet(QString::fromUtf8("background-color: black; border: 2px solid #333;"));
        localVideoLabel->setAlignment(Qt::AlignCenter);

        localVideoContainerLayout->addWidget(localVideoLabel);


        videoGroupLayout->addWidget(localVideoContainer);

        remoteVideoContainer = new QWidget(videoGroup);
        remoteVideoContainer->setObjectName("remoteVideoContainer");
        remoteVideoContainerLayout = new QVBoxLayout(remoteVideoContainer);
        remoteVideoContainerLayout->setObjectName("remoteVideoContainerLayout");
        remoteVideoContainerLayout->setContentsMargins(0, 0, 0, 0);
        remoteLabelTitle = new QLabel(remoteVideoContainer);
        remoteLabelTitle->setObjectName("remoteLabelTitle");
        remoteLabelTitle->setAlignment(Qt::AlignCenter);

        remoteVideoContainerLayout->addWidget(remoteLabelTitle);

        remoteVideoLabel = new QLabel(remoteVideoContainer);
        remoteVideoLabel->setObjectName("remoteVideoLabel");
        remoteVideoLabel->setMinimumSize(QSize(320, 240));
        remoteVideoLabel->setStyleSheet(QString::fromUtf8("background-color: #1a1a2e; border: 2px solid #333;"));
        remoteVideoLabel->setAlignment(Qt::AlignCenter);

        remoteVideoContainerLayout->addWidget(remoteVideoLabel);


        videoGroupLayout->addWidget(remoteVideoContainer);


        videoPanelLayout->addWidget(videoGroup);

        splitter->addWidget(videoPanel);
        controlPanel = new QWidget(splitter);
        controlPanel->setObjectName("controlPanel");
        controlPanelLayout = new QVBoxLayout(controlPanel);
        controlPanelLayout->setObjectName("controlPanelLayout");
        controlPanelLayout->setContentsMargins(0, 0, 0, 0);
        serverGroup = new QGroupBox(controlPanel);
        serverGroup->setObjectName("serverGroup");
        serverGroupLayout = new QVBoxLayout(serverGroup);
        serverGroupLayout->setObjectName("serverGroupLayout");
        serverInputLayout = new QHBoxLayout();
        serverInputLayout->setObjectName("serverInputLayout");
        hostLabel = new QLabel(serverGroup);
        hostLabel->setObjectName("hostLabel");

        serverInputLayout->addWidget(hostLabel);

        hostEdit = new QLineEdit(serverGroup);
        hostEdit->setObjectName("hostEdit");

        serverInputLayout->addWidget(hostEdit);

        portLabel = new QLabel(serverGroup);
        portLabel->setObjectName("portLabel");

        serverInputLayout->addWidget(portLabel);

        portEdit = new QLineEdit(serverGroup);
        portEdit->setObjectName("portEdit");
        portEdit->setMaximumSize(QSize(80, 16777215));

        serverInputLayout->addWidget(portEdit);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        serverInputLayout->addItem(horizontalSpacer);


        serverGroupLayout->addLayout(serverInputLayout);

        serverBtnLayout = new QHBoxLayout();
        serverBtnLayout->setObjectName("serverBtnLayout");
        connectBtn = new QPushButton(serverGroup);
        connectBtn->setObjectName("connectBtn");

        serverBtnLayout->addWidget(connectBtn);

        disconnectBtn = new QPushButton(serverGroup);
        disconnectBtn->setObjectName("disconnectBtn");

        serverBtnLayout->addWidget(disconnectBtn);


        serverGroupLayout->addLayout(serverBtnLayout);


        controlPanelLayout->addWidget(serverGroup);

        cameraGroup = new QGroupBox(controlPanel);
        cameraGroup->setObjectName("cameraGroup");
        cameraBtnLayout = new QHBoxLayout(cameraGroup);
        cameraBtnLayout->setObjectName("cameraBtnLayout");
        startCameraBtn = new QPushButton(cameraGroup);
        startCameraBtn->setObjectName("startCameraBtn");

        cameraBtnLayout->addWidget(startCameraBtn);

        stopCameraBtn = new QPushButton(cameraGroup);
        stopCameraBtn->setObjectName("stopCameraBtn");

        cameraBtnLayout->addWidget(stopCameraBtn);

        captureBtn = new QPushButton(cameraGroup);
        captureBtn->setObjectName("captureBtn");

        cameraBtnLayout->addWidget(captureBtn);


        controlPanelLayout->addWidget(cameraGroup);

        logGroup = new QGroupBox(controlPanel);
        logGroup->setObjectName("logGroup");
        logGroupLayout = new QVBoxLayout(logGroup);
        logGroupLayout->setObjectName("logGroupLayout");
        logTextEdit = new QTextEdit(logGroup);
        logTextEdit->setObjectName("logTextEdit");
        logTextEdit->setMaximumSize(QSize(16777215, 150));
        logTextEdit->setReadOnly(true);

        logGroupLayout->addWidget(logTextEdit);


        controlPanelLayout->addWidget(logGroup);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        controlPanelLayout->addItem(verticalSpacer);

        splitter->addWidget(controlPanel);

        mainLayout->addWidget(splitter);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Smart Security - Client", nullptr));
        videoGroup->setTitle(QCoreApplication::translate("MainWindow", "Video Display", nullptr));
        localLabelTitle->setText(QCoreApplication::translate("MainWindow", "Local Camera", nullptr));
        localVideoLabel->setText(QString());
        remoteLabelTitle->setText(QCoreApplication::translate("MainWindow", "Server Detection", nullptr));
        remoteVideoLabel->setText(QString());
        serverGroup->setTitle(QCoreApplication::translate("MainWindow", "Server Connection", nullptr));
        hostLabel->setText(QCoreApplication::translate("MainWindow", "Server:", nullptr));
        hostEdit->setText(QCoreApplication::translate("MainWindow", "127.0.0.1", nullptr));
        portLabel->setText(QCoreApplication::translate("MainWindow", "Port:", nullptr));
        portEdit->setText(QCoreApplication::translate("MainWindow", "8888", nullptr));
        connectBtn->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        disconnectBtn->setText(QCoreApplication::translate("MainWindow", "Disconnect", nullptr));
        cameraGroup->setTitle(QCoreApplication::translate("MainWindow", "Camera Controls", nullptr));
        startCameraBtn->setText(QCoreApplication::translate("MainWindow", "Start Camera", nullptr));
        stopCameraBtn->setText(QCoreApplication::translate("MainWindow", "Stop Camera", nullptr));
        captureBtn->setText(QCoreApplication::translate("MainWindow", "Capture", nullptr));
        logGroup->setTitle(QCoreApplication::translate("MainWindow", "Log", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
