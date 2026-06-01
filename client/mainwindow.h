#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QTextEdit>
#include <opencv2/opencv.hpp>
#include "tcpclient.h"
#include "detectormodel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class CameraThread : public QThread
{
    Q_OBJECT
public:
    explicit CameraThread(QObject *parent = nullptr);
    void run() override;
    void stop() { isRunning = false; }

signals:
    void frameCaptured(const cv::Mat& frame);

private:
    bool isRunning = false;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onConnectClicked(const QString& host, quint16 port);
    void onDisconnectClicked();
    void onStartCameraClicked();
    void onStopCameraClicked();
    void onCaptureClicked();
    void onSendFrameClicked();
    void onServerConnected();
    void onServerDisconnected();
    void onConnectionError(const QString& errorString);
    void onServerMessageReceived(const QString& message);
    void onFrameReceived(const QImage& frame, const std::vector<DetectionResult>& results);
    void updateCameraFrame(const cv::Mat& frame);
    void updateLog(const QString& message);

private:
    Ui::MainWindow *ui;
    TcpClient* tcpClient;
    CameraThread* cameraThread;
    QTimer* cameraTimer;
    cv::VideoCapture cap;
    bool isCameraRunning = false;
    cv::Mat latestFrame;  // 存储最新帧用于抓拍
    
    // UI Widgets
    QLabel* localVideoLabel;
    QLabel* remoteVideoLabel;
    QTextEdit* logTextEdit;

    void setupUI();
    void loadStyleSheet();
    void drawResultsOnFrame(QImage& frame, const std::vector<DetectionResult>& results);
    bool saveCapture(const QImage& frame);
};
#endif // MAINWINDOW_H
