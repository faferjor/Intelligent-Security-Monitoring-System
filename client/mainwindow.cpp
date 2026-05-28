#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QDir>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QTextEdit>
#include <QCoreApplication>

CameraThread::CameraThread(QObject *parent) : QThread(parent)
{
}

void CameraThread::run()
{
    isRunning = true;
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        qDebug() << "Failed to open camera in thread";
        return;
    }

    while (isRunning) {
        cv::Mat frame;
        cap >> frame;
        if (!frame.empty()) {
            emit frameCaptured(frame);
        }
        msleep(30);
    }

    cap.release();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // ui->setupUi(this); // 不使用UI文件，完全手动创建界面
    setupUI();

    tcpClient = new TcpClient(this);
    detector = new DetectorModel(this);
    cameraThread = new CameraThread(this);
    cameraTimer = new QTimer(this);

    // 获取应用程序运行目录
    QString modelPath =  "C:/Users/fafer/Desktop/protect/client/build/Desktop_Qt_6_5_3_MinGW_64_bit-Debug/debug/yolov8n.onnx";
    QString classesPath =  "C:/Users/fafer/Desktop/protect/client/build/Desktop_Qt_6_5_3_MinGW_64_bit-Debug/debug/coco.names";
    qDebug() << "Model Path:" << modelPath;
    qDebug() << "Classes Path:" << classesPath;

    // 尝试从运行目录加载
    bool modelLoaded = detector->loadModel(modelPath.toStdString(), classesPath.toStdString());
    if (!modelLoaded) {
        QString errorMsg = QString("Failed to load YOLOv8 model!\n\n" 
                                 "Please ensure the following files exist:\n"
                                 "1. %1\n"
                                 "2. %2\n\n"
                                 "You can download them from:\n"
                                 "- YOLOv8n ONNX: https://github.com/ultralytics/assets/releases/download/v0.0.0/yolov8n.onnx\n"
                                 "- coco.names: https://raw.githubusercontent.com/pjreddie/darknet/master/data/coco.names")
                                 .arg(modelPath, classesPath);
        
        QMessageBox::warning(this, "Error", errorMsg);
    }

    connect(tcpClient, &TcpClient::connected, this, &MainWindow::onServerConnected);
    connect(tcpClient, &TcpClient::disconnected, this, &MainWindow::onServerDisconnected);
    connect(tcpClient, &TcpClient::connectionError, this, &MainWindow::onConnectionError);
    connect(tcpClient, &TcpClient::serverMessageReceived, this, &MainWindow::onServerMessageReceived);
    connect(tcpClient, &TcpClient::frameReceived, this, &MainWindow::onFrameReceived);
    connect(cameraThread, &CameraThread::frameCaptured, this, &MainWindow::updateCameraFrame);

    updateLog("Smart Security Client initialized");
}

MainWindow::~MainWindow()
{
    if (isCameraRunning) {
        cameraThread->stop();
        cameraThread->wait();
    }
    if (cap.isOpened()) {
        cap.release();
    }
    if (tcpClient->isConnected()) {
        tcpClient->disconnectFromServer();
    }
    delete ui;
}

void MainWindow::setupUI()
{
    this->setWindowTitle("Smart Security - Client");

    QLabel* hostLabel = new QLabel("Server:", this);
    QLineEdit* hostEdit = new QLineEdit("127.0.0.1", this);
    QLabel* portLabel = new QLabel("Port:", this);
    QLineEdit* portEdit = new QLineEdit("8888", this);
    QPushButton* connectBtn = new QPushButton("Connect", this);
    QPushButton* disconnectBtn = new QPushButton("Disconnect", this);

    QPushButton* startCameraBtn = new QPushButton("Start Camera", this);
    QPushButton* stopCameraBtn = new QPushButton("Stop Camera", this);
    QPushButton* captureBtn = new QPushButton("Capture", this);
    QPushButton* sendFrameBtn = new QPushButton("Send Frame to Server", this);

    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    QLabel* localVideoLabel = new QLabel(this);
    localVideoLabel->setAlignment(Qt::AlignCenter);
    localVideoLabel->setStyleSheet("background-color: black;");
    QLabel* remoteVideoLabel = new QLabel(this);
    remoteVideoLabel->setAlignment(Qt::AlignCenter);
    remoteVideoLabel->setStyleSheet("background-color: #2b2b2b;");

    QWidget* controlPanel = new QWidget(this);
    QVBoxLayout* controlLayout = new QVBoxLayout(controlPanel);
    QGroupBox* serverGroup = new QGroupBox("Server Connection", this);
    QHBoxLayout* serverLayout = new QHBoxLayout(serverGroup);
    serverLayout->addWidget(hostLabel);
    serverLayout->addWidget(hostEdit);
    serverLayout->addWidget(portLabel);
    serverLayout->addWidget(portEdit);
    serverLayout->addWidget(connectBtn);
    serverLayout->addWidget(disconnectBtn);

    QGroupBox* cameraGroup = new QGroupBox("Camera Controls", this);
    QHBoxLayout* cameraLayout = new QHBoxLayout(cameraGroup);
    cameraLayout->addWidget(startCameraBtn);
    cameraLayout->addWidget(stopCameraBtn);
    cameraLayout->addWidget(captureBtn);
    cameraLayout->addWidget(sendFrameBtn);

    QGroupBox* logGroup = new QGroupBox("Log", this);
    QVBoxLayout* logLayout = new QVBoxLayout(logGroup);
    QTextEdit* logTextEdit = new QTextEdit(this);
    logTextEdit->setReadOnly(true);
    logLayout->addWidget(logTextEdit);

    controlLayout->addWidget(serverGroup);
    controlLayout->addWidget(cameraGroup);
    controlLayout->addWidget(logGroup);

    QWidget* videoPanel = new QWidget(this);
    QHBoxLayout* videoLayout = new QHBoxLayout(videoPanel);
    videoLayout->addWidget(localVideoLabel);
    videoLayout->addWidget(remoteVideoLabel);
    localVideoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    remoteVideoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    splitter->addWidget(videoPanel);
    splitter->addWidget(controlPanel);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(splitter);

    connect(connectBtn, &QPushButton::clicked, this, [this, hostEdit, portEdit]() {
        QString host = hostEdit->text();
        quint16 port = portEdit->text().toUShort();
        onConnectClicked(host, port);
    });
    connect(disconnectBtn, &QPushButton::clicked, this, &MainWindow::onDisconnectClicked);
    connect(startCameraBtn, &QPushButton::clicked, this, &MainWindow::onStartCameraClicked);
    connect(stopCameraBtn, &QPushButton::clicked, this, &MainWindow::onStopCameraClicked);
    connect(captureBtn, &QPushButton::clicked, this, &MainWindow::onCaptureClicked);
    connect(sendFrameBtn, &QPushButton::clicked, this, &MainWindow::onSendFrameClicked);

    // Store pointers in member variables
    this->localVideoLabel = localVideoLabel;
    this->remoteVideoLabel = remoteVideoLabel;
    this->logTextEdit = logTextEdit;
}

void MainWindow::onConnectClicked(const QString& host, quint16 port)
{
    if (tcpClient->connectToServer(host, port)) {
        updateLog(QString("Connected to server %1:%2").arg(host).arg(port));
    } else {
        updateLog(QString("Failed to connect to server %1:%2").arg(host).arg(port));
    }
}

void MainWindow::onDisconnectClicked()
{
    if (tcpClient->isConnected()) {
        tcpClient->disconnectFromServer();
        updateLog("Disconnected from server");
    }
}

void MainWindow::onStartCameraClicked()
{
    if (!isCameraRunning) {
        cameraThread->start();
        isCameraRunning = true;
        updateLog("Camera started");
    }
}

void MainWindow::onStopCameraClicked()
{
    if (isCameraRunning) {
        cameraThread->stop();
        cameraThread->wait();
        isCameraRunning = false;
        localVideoLabel->clear();
        updateLog("Camera stopped");
    }
}

void MainWindow::onCaptureClicked()
{
    if (isCameraRunning && !latestFrame.empty()) {
        QImage qImage = detector->matToQImage(latestFrame);
        if (saveCapture(qImage)) {
            updateLog("Image captured successfully");
        } else {
            updateLog("Failed to capture image");
        }
    } else if (!isCameraRunning) {
        updateLog("Camera is not running");
    }
}

void MainWindow::onSendFrameClicked()
{
    if (tcpClient->isConnected() && isCameraRunning && !latestFrame.empty()) {
        QImage qImage = detector->matToQImage(latestFrame);
        tcpClient->sendFrame(qImage);
        updateLog("Frame sent to server");
    } else if (!tcpClient->isConnected()) {
        updateLog("Not connected to server");
    } else if (!isCameraRunning) {
        updateLog("Camera is not running");
    }
}

void MainWindow::onServerConnected()
{
    updateLog("Successfully connected to server");
    tcpClient->sendMessage("Client connected successfully");
}

void MainWindow::onServerDisconnected()
{
    updateLog("Disconnected from server");
}

void MainWindow::onConnectionError(const QString& errorString)
{
    updateLog(QString("Connection error: %1").arg(errorString));
    QMessageBox::warning(this, "Connection Error", errorString);
}

void MainWindow::onServerMessageReceived(const QString& message)
{
    updateLog(QString("Server: %1").arg(message));
}

void MainWindow::onFrameReceived(const QImage& frame, const std::vector<DetectionResult>& results)
{
    QImage displayFrame = frame.copy();
    drawResultsOnFrame(displayFrame, results);
    remoteVideoLabel->setPixmap(QPixmap::fromImage(displayFrame).scaled(remoteVideoLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    for (const auto& result : results) {
        if (result.className == "person" || result.className == "car" || result.className == "truck") {
            updateLog(QString("ALERT from Server: %1 detected at (%2, %3)").arg(QString::fromStdString(result.className), QString::number(result.center.x), QString::number(result.center.y)));
        }
    }
}

void MainWindow::updateCameraFrame(const cv::Mat& frame)
{
    if (!frame.empty()) {
        latestFrame = frame.clone();  // 保存最新帧用于抓拍
        QImage qImage = detector->matToQImage(frame);
        localVideoLabel->setPixmap(QPixmap::fromImage(qImage).scaled(localVideoLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

        if (tcpClient->isConnected()) {
            std::vector<DetectionResult> results = detector->detect(frame);
            tcpClient->sendDetectionResults(results);
        }
    }
}

void MainWindow::updateLog(const QString& message)
{
    QString logEntry = QString("[%1] %2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"), message);
    logTextEdit->append(logEntry);
}

void MainWindow::drawResultsOnFrame(QImage& frame, const std::vector<DetectionResult>& results)
{
    cv::Mat mat = detector->qImageToMat(frame);
    
    for (const auto& result : results) {
        cv::rectangle(mat, result.bbox, cv::Scalar(0, 255, 0), 2);
        
        std::string label = cv::format("%s: %.2f", result.className.c_str(), result.confidence);
        int baseLine;
        cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
        
        int y = result.bbox.y - labelSize.height - 10;
        y = y > 0 ? y : result.bbox.y + labelSize.height + 10;
        
        cv::rectangle(mat, cv::Point(result.bbox.x, y - labelSize.height),
                      cv::Point(result.bbox.x + labelSize.width, y + baseLine),
                      cv::Scalar(255, 255, 255), cv::FILLED);
        cv::putText(mat, label, cv::Point(result.bbox.x, y),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
    }
    
    frame = detector->matToQImage(mat);
}

bool MainWindow::saveCapture(const QImage& frame)
{
    QDir dir("captures_client");
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString fileName = QString("captures_client/capture_%1.jpg").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));
    return frame.save(fileName, "JPG", 95);
}

//#endif
