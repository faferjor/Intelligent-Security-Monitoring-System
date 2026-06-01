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

// 虚拟警戒线结构
struct TripWire {
    cv::Point p1;
    cv::Point p2;
    std::string name;
    bool isEnabled;
    cv::Scalar color;
    int id;
    enum class Direction {
        Bidirectional,  // 双向
        LeftToRight,    // 从左到右
        RightToLeft,    // 从右到左
        TopToBottom,    // 从上到下
        BottomToTop     // 从下到上
    };
    Direction direction;
};

// 越线警报结构
struct TripWireAlert {
    int wireId;
    std::string wireName;
    cv::Point crossingPoint;
    std::string objectType;
    float confidence;
    TripWire::Direction crossingDirection;
};

// 人员聚集警报结构
struct CrowdAlert {
    cv::Point alertCenter;
    int personCount;
    std::string zoneName;
};

// 跟踪目标的历史位置
struct TrackedObject {
    int objectId;
    std::vector<cv::Point> positionHistory;
    std::string className;
    cv::Rect currentBbox;
    int lastSeenFrame;
};

bool operator==(const TrackedObject& a, const TrackedObject& b);

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
    
    // 越线检测相关
    void setTripWireDetectionEnabled(bool enabled) { tripWireDetectionEnabled = enabled; }
    bool isTripWireDetectionEnabled() const { return tripWireDetectionEnabled; }
    void addTripWire(const TripWire& wire);
    void removeTripWire(int wireId);
    void clearTripWires();
    std::vector<TripWire> getTripWires() const { return tripWires; }
    void setTripWires(const std::vector<TripWire>& wires) { tripWires = wires; }
    std::vector<TripWireAlert> detectTripWireCrossing(const cv::Mat& frame, const std::vector<DetectionResult>& results);
    
    // 人员聚集检测相关
    void setCrowdDetectionEnabled(bool enabled) { crowdDetectionEnabled = enabled; }
    bool isCrowdDetectionEnabled() const { return crowdDetectionEnabled; }
    void setCrowdThreshold(int threshold) { crowdThreshold = threshold; }
    int getCrowdThreshold() const { return crowdThreshold; }
    void setCrowdZone(const std::vector<cv::Point>& zone);
    void clearCrowdZone();
    std::vector<cv::Point> getCrowdZone() const { return crowdZone; }
    std::vector<CrowdAlert> detectCrowd(const cv::Mat& frame, const std::vector<DetectionResult>& results);

private:
    cv::dnn::Net net;
    std::vector<std::string> classes;
    float confidenceThreshold = 0.10f;  // 进一步降低到0.1，更容易检测到目标
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
    
    // 越线检测相关
    bool tripWireDetectionEnabled = false;
    std::vector<TripWire> tripWires;
    int nextWireId = 1;
    std::vector<TrackedObject> trackedObjects;
    int nextTrackedObjectId = 1;
    int currentFrameCount = 0;
    
    // 人员聚集检测相关
    bool crowdDetectionEnabled = false;
    std::vector<cv::Point> crowdZone;
    int crowdThreshold = 3;  // 默认超过3人就触发报警
    
    void updateTrackedBlobs(const std::vector<cv::Rect>& newBlobs);
    double calculateIoU(const cv::Rect& a, const cv::Rect& b);
    bool isPointInPolygon(const cv::Point& point, const std::vector<cv::Point>& polygon);
    
    // 越线检测辅助函数
    void updateTrackedObjects(const std::vector<DetectionResult>& results);
    double pointToLineDistance(const cv::Point& point, const cv::Point& lineP1, const cv::Point& lineP2);
    bool hasCrossedLine(const cv::Point& prevPoint, const cv::Point& currPoint, 
                        const cv::Point& lineP1, const cv::Point& lineP2,
                        TripWire::Direction& crossingDirection);
    bool isDirectionMatching(TripWire::Direction wireDirection, TripWire::Direction crossingDirection);
};

#endif // DETECTORMODEL_H