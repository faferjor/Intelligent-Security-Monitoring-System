#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
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
    cameraThread = new CameraThread(this);
    cameraTimer = new QTimer(this);

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

void MainWindow::loadStyleSheet()
{
    // 尝试从服务器端的样式文件加载
    QFile file("../server/styles.qss");
    if (file.exists()) {
        file.open(QFile::ReadOnly);
        QString styleSheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(styleSheet);
        file.close();
    }
}

void MainWindow::setupUI()
{
    this->setWindowTitle("Smart Security - Client");
    this->setMinimumSize(1000, 700);

    // 加载样式
    loadStyleSheet();

    QLabel* hostLabel = new QLabel("Server:", this);
    QLineEdit* hostEdit = new QLineEdit("127.0.0.1", this);
    QLabel* portLabel = new QLabel("Port:", this);
    QLineEdit* portEdit = new QLineEdit("8888", this);
    QPushButton* connectBtn = new QPushButton("Connect", this);
    QPushButton* disconnectBtn = new QPushButton("Disconnect", this);
    disconnectBtn->setObjectName("dangerBtn");

    QPushButton* startCameraBtn = new QPushButton("Start Camera", this);
    QPushButton* stopCameraBtn = new QPushButton("Stop Camera", this);
    stopCameraBtn->setObjectName("dangerBtn");
    QPushButton* captureBtn = new QPushButton("Capture", this);
    captureBtn->setObjectName("successBtn");

    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    QLabel* localVideoLabel = new QLabel(this);
    localVideoLabel->setAlignment(Qt::AlignCenter);
    localVideoLabel->setStyleSheet("background-color: black; border: 2px solid #333;");
    localVideoLabel->setMinimumSize(320, 240);
    QLabel* remoteVideoLabel = new QLabel(this);
    remoteVideoLabel->setAlignment(Qt::AlignCenter);
    remoteVideoLabel->setStyleSheet("background-color: #1a1a2e; border: 2px solid #333;");
    remoteVideoLabel->setMinimumSize(320, 240);

    QWidget* controlPanel = new QWidget(this);
    QVBoxLayout* controlLayout = new QVBoxLayout(controlPanel);
    QGroupBox* serverGroup = new QGroupBox("Server Connection", this);
    QVBoxLayout* serverLayout = new QVBoxLayout(serverGroup);
    
    QHBoxLayout* serverInputLayout = new QHBoxLayout();
    serverInputLayout->addWidget(hostLabel);
    serverInputLayout->addWidget(hostEdit);
    serverInputLayout->addWidget(portLabel);
    portEdit->setMaximumWidth(80);
    serverInputLayout->addWidget(portEdit);
    
    QHBoxLayout* serverBtnLayout = new QHBoxLayout();
    serverBtnLayout->addWidget(connectBtn);
    serverBtnLayout->addWidget(disconnectBtn);
    
    serverLayout->addLayout(serverInputLayout);
    serverLayout->addLayout(serverBtnLayout);

    QGroupBox* cameraGroup = new QGroupBox("Camera Controls", this);
    QVBoxLayout* cameraLayout = new QVBoxLayout(cameraGroup);
    
    QHBoxLayout* cameraBtnLayout = new QHBoxLayout();
    cameraBtnLayout->addWidget(startCameraBtn);
    cameraBtnLayout->addWidget(stopCameraBtn);
    cameraBtnLayout->addWidget(captureBtn);
    
    cameraLayout->addLayout(cameraBtnLayout);

    QGroupBox* logGroup = new QGroupBox("Log", this);
    QVBoxLayout* logLayout = new QVBoxLayout(logGroup);
    QTextEdit* logTextEdit = new QTextEdit(this);
    logTextEdit->setReadOnly(true);
    logTextEdit->setMaximumHeight(150);
    logLayout->addWidget(logTextEdit);

    controlLayout->addWidget(serverGroup);
    controlLayout->addWidget(cameraGroup);
    controlLayout->addWidget(logGroup);
    controlLayout->addStretch();

    QWidget* videoPanel = new QWidget(this);
    QVBoxLayout* videoPanelLayout = new QVBoxLayout(videoPanel);
    
    QGroupBox* videoGroup = new QGroupBox("Video Display", this);
    QHBoxLayout* videoLayout = new QHBoxLayout(videoGroup);
    
    QWidget* localVideoContainer = new QWidget(this);
    QVBoxLayout* localVideoContainerLayout = new QVBoxLayout(localVideoContainer);
    QLabel* localLabelTitle = new QLabel("Local Camera", this);
    localLabelTitle->setAlignment(Qt::AlignCenter);
    localVideoContainerLayout->addWidget(localLabelTitle);
    localVideoContainerLayout->addWidget(localVideoLabel);
    
    QWidget* remoteVideoContainer = new QWidget(this);
    QVBoxLayout* remoteVideoContainerLayout = new QVBoxLayout(remoteVideoContainer);
    QLabel* remoteLabelTitle = new QLabel("Server Detection", this);
    remoteLabelTitle->setAlignment(Qt::AlignCenter);
    remoteVideoContainerLayout->addWidget(remoteLabelTitle);
    remoteVideoContainerLayout->addWidget(remoteVideoLabel);
    
    videoLayout->addWidget(localVideoContainer);
    videoLayout->addWidget(remoteVideoContainer);
    
    videoPanelLayout->addWidget(videoGroup);
    
    localVideoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    remoteVideoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    splitter->addWidget(videoPanel);
    splitter->addWidget(controlPanel);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(12);
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
        // 直接转换cv::Mat到QImage
        QImage qImage = QImage(latestFrame.data, latestFrame.cols, latestFrame.rows, static_cast<int>(latestFrame.step), QImage::Format_RGB888).rgbSwapped();
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
        // 直接转换cv::Mat到QImage
        QImage qImage = QImage(latestFrame.data, latestFrame.cols, latestFrame.rows, static_cast<int>(latestFrame.step), QImage::Format_RGB888).rgbSwapped();
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
        
        // 转换为QImage显示
        QImage qImage = QImage(frame.data, frame.cols, frame.rows, static_cast<int>(frame.step), QImage::Format_RGB888).rgbSwapped();
        
        // 显示本地画面（原始画面，不检测）
        localVideoLabel->setPixmap(QPixmap::fromImage(qImage).scaled(localVideoLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        
        // 如果连接到服务器，发送画面到服务器进行检测
        if (tcpClient->isConnected()) {
            tcpClient->sendFrame(qImage);
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
    // 将QImage转换为cv::Mat
    cv::Mat mat;
    if (frame.format() == QImage::Format_RGB888) {
        mat = cv::Mat(frame.height(), frame.width(), CV_8UC3, const_cast<uchar*>(frame.bits()), frame.bytesPerLine()).clone();
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
    } else {
        return;
    }
    
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
    
    // 将cv::Mat转换回QImage
    cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
    frame = QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGB888).copy();
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
