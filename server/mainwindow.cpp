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
#include <QListWidget>
#include <QTextEdit>
#include <QCoreApplication>
#include <QGroupBox>
#include <QMutexLocker>

DetectionThread::DetectionThread(QObject *parent) : QThread(parent)
{
}

void DetectionThread::setFrame(const cv::Mat& frame)
{
    QMutexLocker locker(&mutex);
    this->frame = frame.clone();
}

void DetectionThread::run()
{
    QMutexLocker locker(&mutex);
    m_isProcessing = true;
    processedFrame = frame.clone();
    
    if (detector && !frame.empty()) {
        std::vector<DetectionResult> results = detector->detect(frame);
        emit detectionCompleted(results, processedFrame);
    }
    
    m_isProcessing = false;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // ui->setupUi(this); // 不使用UI文件，完全手动创建界面
    setupUI();

    tcpServer = new TcpServer(this);
    detector = new DetectorModel(this);
    detectionThread = new DetectionThread(this);
    cameraTimer = new QTimer(this);

    // 获取应用程序运行目录
    QString modelPath =  "C:/Users/fafer/Desktop/protect/server/build/Desktop_Qt_6_5_3_MinGW_64_bit-Debug/debug/yolov8n.onnx";
    QString classesPath =   "C:/Users/fafer/Desktop/protect/server/build/Desktop_Qt_6_5_3_MinGW_64_bit-Debug/debug/coco.names";
    qDebug() << "Model Path:" << modelPath;
    qDebug() << "Classes Path:" << classesPath;

    // 尝试从运行目录加载
    bool modelLoaded = detector->loadModel(modelPath.toStdString(), classesPath.toStdString());
    
    // 如果失败，尝试从项目根目录加载
    if (!modelLoaded) {
        qDebug() << "Trying to load from project root directory...";
        modelPath = "C:/Users/fafer/Desktop/protect/server/build/Desktop_Qt_6_5_3_MinGW_64_bit-Debug/debug/yolov8n.onnx";
        classesPath =  "C:/Users/fafer/Desktop/protect/server/build/Desktop_Qt_6_5_3_MinGW_64_bit-Debug/debug/coco.names";
        qDebug() << "Model Path:" << modelPath;
        qDebug() << "Classes Path:" << classesPath;
        
        modelLoaded = detector->loadModel(modelPath.toStdString(), classesPath.toStdString());
    }

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

    detectionThread->setDetector(detector);

    connect(cameraTimer, &QTimer::timeout, this, &MainWindow::updateCameraFrame);
    connect(detectionThread, &DetectionThread::detectionCompleted, this, &MainWindow::onDetectionCompleted);
    connect(tcpServer, &TcpServer::clientConnected, this, &MainWindow::onClientConnected);
    connect(tcpServer, &TcpServer::clientDisconnected, this, &MainWindow::onClientDisconnected);
    connect(tcpServer, &TcpServer::messageReceived, this, &MainWindow::onMessageReceived);
    connect(tcpServer, &TcpServer::frameReceived, this, &MainWindow::onFrameReceived);

    updateLog("Smart Security Server initialized");
}

MainWindow::~MainWindow()
{
    if (cap.isOpened()) {
        cap.release();
    }
    if (isRecording) {
        stopRecording();
    }
    if (cameraTimer->isActive()) {
        cameraTimer->stop();
    }
    tcpServer->stopServer();
    detectionThread->quit();
    detectionThread->wait();
    delete ui;
}

void MainWindow::setupUI()
{
    this->setWindowTitle("Smart Security - Server");

    QPushButton* startServerBtn = new QPushButton("Start Server", this);
    QPushButton* stopServerBtn = new QPushButton("Stop Server", this);
    QPushButton* startCameraBtn = new QPushButton("Start Camera", this);
    QPushButton* stopCameraBtn = new QPushButton("Stop Camera", this);
    QPushButton* captureBtn = new QPushButton("Capture", this);
    recordBtn = new QPushButton("Record", this);
    motionDetectionCheckBox = new QCheckBox("Enable Motion Detection", this);
    zoneDetectionCheckBox = new QCheckBox("Enable Zone Detection", this);
    addZoneBtn = new QPushButton("Add Guard Zone", this);
    clearZonesBtn = new QPushButton("Clear Zones", this);

    QLineEdit* portEdit = new QLineEdit("8888", this);
    QLabel* portLabel = new QLabel("Port:", this);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    QHBoxLayout* serverLayout = new QHBoxLayout();
    serverLayout->addWidget(portLabel);
    serverLayout->addWidget(portEdit);
    serverLayout->addWidget(startServerBtn);
    serverLayout->addWidget(stopServerBtn);

    QHBoxLayout* cameraLayout = new QHBoxLayout();
    cameraLayout->addWidget(startCameraBtn);
    cameraLayout->addWidget(stopCameraBtn);
    cameraLayout->addWidget(captureBtn);
    cameraLayout->addWidget(recordBtn);
    cameraLayout->addWidget(motionDetectionCheckBox);
    cameraLayout->addWidget(zoneDetectionCheckBox);
    cameraLayout->addWidget(addZoneBtn);
    cameraLayout->addWidget(clearZonesBtn);

    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    QLabel* videoLabel = new QLabel(this);
    videoLabel->setAlignment(Qt::AlignCenter);
    videoLabel->setStyleSheet("background-color: black;");

    QWidget* rightPanel = new QWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    
    // 客户端列表
    QListWidget* clientListWidget = new QListWidget(this);
    rightLayout->addWidget(new QLabel("Connected Clients:"));
    rightLayout->addWidget(clientListWidget);
    
    // 历史记录查询
    QGroupBox* historyGroup = new QGroupBox("History", this);
    QVBoxLayout* historyLayout = new QVBoxLayout(historyGroup);
    
    QHBoxLayout* historyBtnLayout = new QHBoxLayout();
    browseCapturesBtn = new QPushButton("Browse Captures", this);
    browseRecordingsBtn = new QPushButton("Browse Recordings", this);
    deleteSelectedBtn = new QPushButton("Delete Selected", this);
    historyBtnLayout->addWidget(browseCapturesBtn);
    historyBtnLayout->addWidget(browseRecordingsBtn);
    historyBtnLayout->addWidget(deleteSelectedBtn);
    
    historyListWidget = new QListWidget(this);
    historyPreviewLabel = new QLabel(this);
    historyPreviewLabel->setAlignment(Qt::AlignCenter);
    historyPreviewLabel->setStyleSheet("background-color: #333; min-height: 100px;");
    
    historyLayout->addLayout(historyBtnLayout);
    historyLayout->addWidget(new QLabel("Files:"));
    historyLayout->addWidget(historyListWidget);
    historyLayout->addWidget(new QLabel("Preview:"));
    historyLayout->addWidget(historyPreviewLabel);
    
    rightLayout->addWidget(historyGroup);
    
    // 日志
    QTextEdit* logTextEdit = new QTextEdit(this);
    logTextEdit->setReadOnly(true);
    rightLayout->addWidget(new QLabel("Server Log:"));
    rightLayout->addWidget(logTextEdit);

    splitter->addWidget(videoLabel);
    splitter->addWidget(rightPanel);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);

    mainLayout->addLayout(serverLayout);
    mainLayout->addLayout(cameraLayout);
    mainLayout->addWidget(splitter);

    connect(startServerBtn, &QPushButton::clicked, this, [this, portEdit]() {
        quint16 port = portEdit->text().toUShort();
        onStartServerClicked(port);
    });
    connect(stopServerBtn, &QPushButton::clicked, this, &MainWindow::onStopServerClicked);
    connect(startCameraBtn, &QPushButton::clicked, this, &MainWindow::onStartCameraClicked);
    connect(stopCameraBtn, &QPushButton::clicked, this, &MainWindow::onStopCameraClicked);
    connect(captureBtn, &QPushButton::clicked, this, &MainWindow::onCaptureClicked);
    connect(recordBtn, &QPushButton::clicked, this, &MainWindow::onRecordClicked);
    connect(motionDetectionCheckBox, &QCheckBox::stateChanged, this, [this](int state) {
        isMotionDetectionEnabled = (state == Qt::Checked);
        detector->setMotionDetectionEnabled(isMotionDetectionEnabled);
        updateLog(isMotionDetectionEnabled ? "Motion detection enabled" : "Motion detection disabled");
    });
    connect(zoneDetectionCheckBox, &QCheckBox::stateChanged, this, [this](int state) {
        isZoneDetectionEnabled = (state == Qt::Checked);
        detector->setZoneDetectionEnabled(isZoneDetectionEnabled);
        updateLog(isZoneDetectionEnabled ? "Zone detection enabled" : "Zone detection disabled");
    });
    connect(addZoneBtn, &QPushButton::clicked, this, &MainWindow::addDefaultGuardZone);
    connect(clearZonesBtn, &QPushButton::clicked, this, [this]() {
        detector->clearGuardZones();
        updateLog("All guard zones cleared");
    });
    
    // 历史查询相关连接
    connect(browseCapturesBtn, &QPushButton::clicked, this, [this]() {
        browseHistoryFiles("captures", "*.jpg");
    });
    connect(browseRecordingsBtn, &QPushButton::clicked, this, [this]() {
        browseHistoryFiles("recordings", "*.avi");
    });
    connect(deleteSelectedBtn, &QPushButton::clicked, this, &MainWindow::deleteSelectedHistoryItem);
    connect(historyListWidget, &QListWidget::itemClicked, this, &MainWindow::onHistoryItemSelected);

    // Store pointers in member variables
    this->videoLabel = videoLabel;
    this->clientListWidget = clientListWidget;
    this->logTextEdit = logTextEdit;
}

void MainWindow::onStartServerClicked(quint16 port)
{
    if (tcpServer->startServer(port)) {
        updateLog(QString("Server started on port %1").arg(port));
    } else {
        updateLog("Failed to start server");
        QMessageBox::warning(this, "Error", "Failed to start server! Please check if the port is in use or available.");
    }
}

void MainWindow::onStopServerClicked()
{
    tcpServer->stopServer();
    updateLog("Server stopped");
    clientListWidget->clear();
}

void MainWindow::onStartCameraClicked()
{
    if (!cap.open(0)) {
        QMessageBox::warning(this, "Error", "Failed to open camera!");
        return;
    }
     cameraTimer->start(30);
    updateLog("Camera started");
}

void MainWindow::onStopCameraClicked()
{
    if (cap.isOpened()) {
        cap.release();
    }
    if (cameraTimer->isActive()) {
        cameraTimer->stop();
    }
    videoLabel->clear();
    updateLog("Camera stopped");
}

void MainWindow::onCaptureClicked()
{
    if (cap.isOpened()) {
        cv::Mat frame;
        cap >> frame;
        if (!frame.empty()) {
            if (saveCapture(frame)) {
                updateLog("Image captured successfully");
            } else {
                updateLog("Failed to capture image");
            }
        }
    }
}

void MainWindow::onRecordClicked()
{
    if (!isRecording) {
        if (cap.isOpened()) {
            cv::Mat frame;
            cap >> frame;
            if (!frame.empty()) {
                if (startRecording(frame)) {
                    recordBtn->setText("Stop Recording");
                    updateLog("Recording started");
                }
            }
        }
    } else {
        stopRecording();
        recordBtn->setText("Record");
        updateLog("Recording stopped");
    }
}

void MainWindow::onClientConnected(const QString& clientId)
{
    updateClientList();
    updateLog(QString("Client connected: %1").arg(clientId));
}

void MainWindow::onClientDisconnected(const QString& clientId)
{
    updateClientList();
    updateLog(QString("Client disconnected: %1").arg(clientId));
}

void MainWindow::onMessageReceived(const QString& clientId, const QString& message)
{
    updateLog(QString("From %1: %2").arg(clientId, message));
    tcpServer->sendMessageToAllClients(QString("%1: %2").arg(clientId, message));
}

void MainWindow::onFrameReceived(const QString& clientId, const QImage& frame)
{
    if (frame.isNull()) {
        return;
    }

    cv::Mat matFrame = detector->qImageToMat(frame);
    if (matFrame.empty()) {
        return;
    }

    // 检查线程是否已完成（可以重新启动）
    if (detectionThread->isFinished()) {
        detectionThread->setFrame(matFrame);
        detectionThread->start();
    }
}

void MainWindow::updateCameraFrame()
{
    if (!cap.isOpened()) {
        return;
    }

    cv::Mat frame;
    cap >> frame;
    if (frame.empty()) {
        return;
    }

    if (isRecording) {
        videoWriter.write(frame);
    }

    // 执行运动检测
    std::vector<MotionBlob> motionBlobs = detector->detectMotion(frame);
    if (isMotionDetectionEnabled && !motionBlobs.empty()) {
        drawMotionBlobsOnFrame(frame, motionBlobs);
        for (const auto& blob : motionBlobs) {
            updateLog(QString("MOTION DETECTED: Object %1 at (%2, %3)").arg(blob.id).arg(blob.center.x).arg(blob.center.y));
        }
    }

    // 绘制警戒区域
    drawGuardZonesOnFrame(frame);

    // 直接显示当前帧
    QImage qImage = detector->matToQImage(frame);
    videoLabel->setPixmap(QPixmap::fromImage(qImage).scaled(videoLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // 执行目标检测（异步）
    if (detectionThread->isFinished()) {
        detectionThread->setFrame(frame.clone());
        detectionThread->start();
    }
}

void MainWindow::onDetectionCompleted(const std::vector<DetectionResult>& results, const cv::Mat& frame)
{
    if (frame.empty()) {
        return;
    }

    // 只有当检测到目标时才更新显示，避免覆盖实时画面
    if (!results.empty()) {
        cv::Mat displayFrame = frame.clone();
        
        // 绘制警戒区域
        drawGuardZonesOnFrame(displayFrame);
        
        // 绘制检测结果
        drawResultsOnFrame(displayFrame, results);
        
        // 区域入侵检测
        if (isZoneDetectionEnabled) {
            std::vector<ZoneAlert> zoneAlerts = detector->detectZoneIntrusion(displayFrame, results);
            for (const auto& alert : zoneAlerts) {
                updateLog(QString("ZONE ALERT: %1 detected in zone '%2' at (%3, %4)").arg(
                    QString::fromStdString(alert.objectType),
                    QString::fromStdString(alert.zoneName),
                    QString::number(alert.intrusionPoint.x),
                    QString::number(alert.intrusionPoint.y)));
            }
        }

        QImage qImage = detector->matToQImage(displayFrame);
        videoLabel->setPixmap(QPixmap::fromImage(qImage).scaled(videoLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

        tcpServer->sendFrameToAllClients(qImage, results);
    }

    for (const auto& result : results) {
        if (result.className == "person" || result.className == "car" || result.className == "truck") {
            updateLog(QString("ALERT: %1 detected at (%2, %3)").arg(QString::fromStdString(result.className), QString::number(result.center.x), QString::number(result.center.y)));
        }
    }
}

void MainWindow::updateLog(const QString& message)
{
    QString logEntry = QString("[%1] %2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"), message);
    logTextEdit->append(logEntry);
}

void MainWindow::updateClientList()
{
    clientListWidget->clear();
    for (const auto& client : tcpServer->getConnectedClients()) {
        clientListWidget->addItem(QString("%1 - %2").arg(client.clientId, client.connectTime.toString("HH:mm:ss")));
    }
}

void MainWindow::drawResultsOnFrame(cv::Mat& frame, const std::vector<DetectionResult>& results)
{
    for (const auto& result : results) {
        cv::rectangle(frame, result.bbox, cv::Scalar(0, 255, 0), 2);
        
        std::string label = cv::format("%s: %.2f", result.className.c_str(), result.confidence);
        int baseLine;
        cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
        
        int y = result.bbox.y - labelSize.height - 10;
        y = y > 0 ? y : result.bbox.y + labelSize.height + 10;
        
        cv::rectangle(frame, cv::Point(result.bbox.x, y - labelSize.height),
                      cv::Point(result.bbox.x + labelSize.width, y + baseLine),
                      cv::Scalar(255, 255, 255), cv::FILLED);
        cv::putText(frame, label, cv::Point(result.bbox.x, y),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
    }
}

void MainWindow::drawMotionBlobsOnFrame(cv::Mat& frame, const std::vector<MotionBlob>& blobs)
{
    for (const auto& blob : blobs) {
        // 绘制运动目标的边界框（红色）
        cv::rectangle(frame, blob.boundingBox, cv::Scalar(0, 0, 255), 2);
        
        // 绘制运动目标的ID标签
        std::string label = cv::format("Motion #%d", blob.id);
        int baseLine;
        cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
        
        int y = blob.boundingBox.y - labelSize.height - 10;
        y = y > 0 ? y : blob.boundingBox.y + labelSize.height + 10;
        
        cv::rectangle(frame, cv::Point(blob.boundingBox.x, y - labelSize.height),
                      cv::Point(blob.boundingBox.x + labelSize.width, y + baseLine),
                      cv::Scalar(0, 0, 255), cv::FILLED);
        cv::putText(frame, label, cv::Point(blob.boundingBox.x, y),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
        
        // 绘制中心点
        cv::circle(frame, blob.center, 4, cv::Scalar(255, 0, 0), cv::FILLED);
    }
}

void MainWindow::drawGuardZonesOnFrame(cv::Mat& frame)
{
    std::vector<GuardZone> zones = detector->getGuardZones();
    for (const auto& zone : zones) {
        if (!zone.isEnabled || zone.polygon.size() < 3) {
            continue;
        }
        
        // 绘制多边形边界
        cv::polylines(frame, zone.polygon, true, zone.color, 2);
        
        // 绘制半透明填充
        std::vector<std::vector<cv::Point>> contours = {zone.polygon};
        cv::Mat overlay = frame.clone();
        cv::fillPoly(overlay, contours, zone.color);
        double alpha = 0.2; // 透明度
        cv::addWeighted(overlay, alpha, frame, 1 - alpha, 0, frame);
        
        // 绘制区域名称
        if (!zone.name.empty()) {
            cv::Point textPos(zone.polygon[0].x, zone.polygon[0].y - 10);
            cv::putText(frame, zone.name, textPos, cv::FONT_HERSHEY_SIMPLEX, 0.5, zone.color, 1);
        }
    }
}

void MainWindow::addDefaultGuardZone()
{
    GuardZone zone;
    zone.name = QString("Zone %1").arg(detector->getGuardZones().size() + 1).toStdString();
    zone.isEnabled = true;
    zone.color = cv::Scalar(0, 255, 255); // 黄色
    
    // 创建一个默认的四边形警戒区域（位于画面中央偏左）
    std::vector<cv::Point> polygon;
    polygon.push_back(cv::Point(100, 100));
    polygon.push_back(cv::Point(300, 100));
    polygon.push_back(cv::Point(300, 300));
    polygon.push_back(cv::Point(100, 300));
    zone.polygon = polygon;
    
    detector->addGuardZone(zone);
    updateLog(QString("Guard zone '%1' added").arg(QString::fromStdString(zone.name)));
}

bool MainWindow::saveCapture(const cv::Mat& frame)
{
    QDir dir("captures");
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString fileName = QString("captures/capture_%1.jpg").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));
    return cv::imwrite(fileName.toStdString(), frame);
}

bool MainWindow::startRecording(const cv::Mat& frame)
{
    QDir dir("recordings");
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString fileName = QString("recordings/recording_%1.avi").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));
    int codec = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
    double fps = 25.0;
    
    videoWriter.open(fileName.toStdString(), codec, fps, frame.size());
    isRecording = videoWriter.isOpened();
    return isRecording;
}

void MainWindow::stopRecording()
{
    if (isRecording) {
        isRecording = false;
        videoWriter.release();
    }
}

void MainWindow::browseHistoryFiles(const QString& directory, const QString& filter)
{
    QDir dir(directory);
    if (!dir.exists()) {
        updateLog(QString("Directory '%1' does not exist").arg(directory));
        return;
    }

    QFileInfoList files = dir.entryInfoList(QStringList(filter), QDir::Files, QDir::Time);
    historyListWidget->clear();

    if (files.isEmpty()) {
        historyListWidget->addItem("No files found");
        historyPreviewLabel->clear();
        return;
    }

    for (const QFileInfo& file : files) {
        QListWidgetItem* item = new QListWidgetItem(file.fileName());
        item->setData(Qt::UserRole, file.absoluteFilePath());
        historyListWidget->addItem(item);
    }

    updateLog(QString("Loaded %1 files from %2").arg(files.size()).arg(directory));
}

void MainWindow::onHistoryItemSelected(QListWidgetItem* item)
{
    QString filePath = item->data(Qt::UserRole).toString();
    if (filePath.isEmpty()) {
        return;
    }

    QFileInfo fileInfo(filePath);
    if (fileInfo.suffix().toLower() == "jpg") {
        QImage image(filePath);
        if (!image.isNull()) {
            historyPreviewLabel->setPixmap(QPixmap::fromImage(image).scaled(historyPreviewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            historyPreviewLabel->setText("Cannot preview image");
        }
    } else if (fileInfo.suffix().toLower() == "avi") {
        historyPreviewLabel->setText(QString("Video: %1\nDuration: N/A").arg(fileInfo.fileName()));
    }

    updateLog(QString("Selected: %1").arg(fileInfo.fileName()));
}

void MainWindow::deleteSelectedHistoryItem()
{
    QListWidgetItem* item = historyListWidget->currentItem();
    if (!item) {
        updateLog("No item selected");
        return;
    }

    QString filePath = item->data(Qt::UserRole).toString();
    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (file.remove()) {
        updateLog(QString("Deleted: %1").arg(QFileInfo(filePath).fileName()));
        delete item;
        historyPreviewLabel->clear();
    } else {
        updateLog(QString("Failed to delete: %1").arg(QFileInfo(filePath).fileName()));
    }
}
