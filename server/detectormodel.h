#ifndef DETECTORMODEL_H
#define DETECTORMODEL_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <vector>
#include <string>
#include <QImage>

struct DetectionResult {
    int classId;
    std::string className;
    float confidence;
    cv::Rect bbox;
    cv::Point center;
};

struct MotionBlob {
    cv::Rect boundingBox;
    cv::Point center;
    int id;
    int frameCount;
    bool isActive;
};

struct GuardZone {
    std::vector<cv::Point> polygon;
    std::string name;
    bool isEnabled;
    cv::Scalar color;
    int id;
};

struct ZoneAlert {
    int zoneId;
    std::string zoneName;
    cv::Point intrusionPoint;
    std::string objectType;
    float confidence;
};

class DetectorModel : public QObject
{
    Q_OBJECT
public:
    explicit DetectorModel(QObject *parent = nullptr);
    ~DetectorModel();

    bool loadModel(const std::string& modelPath, const std::string& classesPath);
    std::vector<DetectionResult> detect(const cv::Mat& frame);
    std::vector<MotionBlob> detectMotion(const cv::Mat& frame);
    std::vector<ZoneAlert> detectZoneIntrusion(const cv::Mat& frame, const std::vector<DetectionResult>& results);
    QImage matToQImage(const cv::Mat& mat);
    cv::Mat qImageToMat(const QImage& image);

    std::vector<std::string> getClasses() const { return classes; }
    float getConfidenceThreshold() const { return confidenceThreshold; }
    void setConfidenceThreshold(float threshold) { confidenceThreshold = threshold; }
    float getNMSThreshold() const { return nmsThreshold; }
    void setNMSThreshold(float threshold) { nmsThreshold = threshold; }
    
    void setMotionDetectionEnabled(bool enabled) { motionDetectionEnabled = enabled; }
    bool isMotionDetectionEnabled() const { return motionDetectionEnabled; }
    void setMotionSensitivity(double sensitivity) { motionSensitivity = sensitivity; }
    double getMotionSensitivity() const { return motionSensitivity; }
    
    void setZoneDetectionEnabled(bool enabled) { zoneDetectionEnabled = enabled; }
    bool isZoneDetectionEnabled() const { return zoneDetectionEnabled; }
    void addGuardZone(const GuardZone& zone);
    void removeGuardZone(int zoneId);
    void clearGuardZones();
    std::vector<GuardZone> getGuardZones() const { return guardZones; }
    void setGuardZones(const std::vector<GuardZone>& zones) { guardZones = zones; }

private:
    cv::dnn::Net net;
    std::vector<std::string> classes;
    float confidenceThreshold = 0.25f;
    float nmsThreshold = 0.45f;
    int inputWidth = 640;
    int inputHeight = 640;
    float scaleFactor = 1.0f / 255.0f;
    cv::Scalar mean = cv::Scalar(0, 0, 0);
    bool swapRB = true;
    
    // 运动检测相关
    cv::Ptr<cv::BackgroundSubtractorMOG2> bgSubtractor;
    bool motionDetectionEnabled = false;
    double motionSensitivity = 500.0;  // 运动灵敏度（像素面积阈值）
    int nextBlobId = 1;
    std::vector<MotionBlob> trackedBlobs;
    
    // 区域入侵检测相关
    bool zoneDetectionEnabled = false;
    std::vector<GuardZone> guardZones;
    int nextZoneId = 1;
    
    void updateTrackedBlobs(const std::vector<cv::Rect>& newBlobs);
    double calculateIoU(const cv::Rect& a, const cv::Rect& b);
    bool isPointInPolygon(const cv::Point& point, const std::vector<cv::Point>& polygon);
};

#endif // DETECTORMODEL_H