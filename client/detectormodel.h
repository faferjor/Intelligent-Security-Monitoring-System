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

class DetectorModel : public QObject
{
    Q_OBJECT
public:
    explicit DetectorModel(QObject *parent = nullptr);
    ~DetectorModel();

    bool loadModel(const std::string& modelPath, const std::string& classesPath);
    std::vector<DetectionResult> detect(const cv::Mat& frame);
    QImage matToQImage(const cv::Mat& mat);
    cv::Mat qImageToMat(const QImage& image);

    std::vector<std::string> getClasses() const { return classes; }
    float getConfidenceThreshold() const { return confidenceThreshold; }
    void setConfidenceThreshold(float threshold) { confidenceThreshold = threshold; }
    float getNMSThreshold() const { return nmsThreshold; }
    void setNMSThreshold(float threshold) { nmsThreshold = threshold; }

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
};

#endif // DETECTORMODEL_H