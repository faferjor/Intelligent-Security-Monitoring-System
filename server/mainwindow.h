#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTimer>
#include <QDateTime>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QListWidget>
#include <QTextEdit>
#include <QCheckBox>
#include <QFileInfoList>
#include <QListWidgetItem>
#include <QMutex>
#include <opencv2/opencv.hpp>
#include "tcpserver.h"
#include "detectormodel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class DetectionThread : public QThread
{
    Q_OBJECT
public:
    explicit DetectionThread(QObject *parent = nullptr);
    void setFrame(const cv::Mat& frame);
    void setDetector(DetectorModel* detector) { this->detector = detector; }
    void run() override;
    bool isProcessing() const { return m_isProcessing; }
    cv::Mat getProcessedFrame() const { return processedFrame; }

signals:
    void detectionCompleted(const std::vector<DetectionResult>& results, const cv::Mat& frame);

private:
    cv::Mat frame;
    cv::Mat processedFrame;
    DetectorModel* detector;
    bool m_isProcessing = false;
    QMutex mutex;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartServerClicked(quint16 port);
    void onStopServerClicked();
    void onStartCameraClicked();
    void onStopCameraClicked();
    void onCaptureClicked();
    void onRecordClicked();
    void onClientConnected(const QString& clientId);
    void onClientDisconnected(const QString& clientId);
    void onMessageReceived(const QString& clientId, const QString& message);
    void onFrameReceived(const QString& clientId, const QImage& frame);
    void updateCameraFrame();
    void onDetectionCompleted(const std::vector<DetectionResult>& results, const cv::Mat& frame);
    void updateLog(const QString& message);
    void sendFrameWithResultsToClient(const QString& clientId, const QImage& frame, const std::vector<DetectionResult>& results);

private:
    Ui::MainWindow *ui;
    TcpServer* tcpServer;
    DetectorModel* detector;
    cv::VideoCapture cap;
    QTimer* cameraTimer;
    DetectionThread* detectionThread;
    bool isRecording = false;
    cv::VideoWriter videoWriter;
    QDateTime startTime;
    bool isMotionDetectionEnabled = false;
    bool isZoneDetectionEnabled = false;
    std::vector<DetectionResult> lastResults;  // 保存本地摄像头的检测结果
    int frameCounter = 0;  // 帧计数器
    static const int DETECTION_INTERVAL = 3;  // 每3帧检测一次
    
    // 存储每个客户端的数据
    struct ClientData {
        QImage lastFrame;
        std::vector<DetectionResult> lastResults;
        QLabel* displayLabel;
    };
    QMap<QString, ClientData> clientDataMap;
    
    // 布局相关
    QWidget* videoWidget;
    QGridLayout* videoLayout;
    QList<QLabel*> availableLabels;
    
    // UI Widgets
    QLabel* videoLabel;
    QListWidget* clientListWidget;
    QTextEdit* logTextEdit;
    QPushButton* recordBtn;
    QCheckBox* motionDetectionCheckBox;
    QCheckBox* zoneDetectionCheckBox;
    QPushButton* addZoneBtn;
    QPushButton* clearZonesBtn;
    // 历史查询相关
    QPushButton* browseCapturesBtn;
    QPushButton* browseRecordingsBtn;
    QPushButton* deleteSelectedBtn;
    QListWidget* historyListWidget;
    QLabel* historyPreviewLabel;

    void setupUI();
    void updateClientList();
    void drawResultsOnFrame(cv::Mat& frame, const std::vector<DetectionResult>& results);
    void drawMotionBlobsOnFrame(cv::Mat& frame, const std::vector<MotionBlob>& blobs);
    void drawGuardZonesOnFrame(cv::Mat& frame);
    bool saveCapture(const cv::Mat& frame);
    bool startRecording(const cv::Mat& frame);
    void stopRecording();
    void addDefaultGuardZone();
    // 历史查询相关
    void browseHistoryFiles(const QString& directory, const QString& filter);
    void onHistoryItemSelected(QListWidgetItem* item);
    void deleteSelectedHistoryItem();
};
#endif // MAINWINDOW_H
