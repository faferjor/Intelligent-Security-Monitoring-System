#include "detectormodel.h"
#include <QDebug>
#include <fstream>
#include <QFile>

DetectorModel::DetectorModel(QObject *parent) : QObject(parent)
{
    // 初始化背景减除器（MOG2算法）
    bgSubtractor = cv::createBackgroundSubtractorMOG2(500, 16, false);
}

DetectorModel::~DetectorModel()
{
}

bool DetectorModel::loadModel(const std::string& modelPath, const std::string& classesPath)
{
    try {
        // 检查文件是否存在
        QFile modelFile(QString::fromStdString(modelPath));
        if (!modelFile.exists()) {
            qDebug() << "Model file does not exist:" << QString::fromStdString(modelPath);
            return false;
        }

        QFile classFile(QString::fromStdString(classesPath));
        if (!classFile.exists()) {
            qDebug() << "Classes file does not exist:" << QString::fromStdString(classesPath);
            return false;
        }

        // 尝试加载模型
        net = cv::dnn::readNetFromONNX(modelPath);
        if (net.empty()) {
            qDebug() << "Failed to load model (empty net):" << QString::fromStdString(modelPath);
            return false;
        }

        // 加载类别文件
        std::ifstream classFileStream(classesPath);
        if (!classFileStream.is_open()) {
            qDebug() << "Failed to open classes file:" << QString::fromStdString(classesPath);
            return false;
        }

        std::string line;
        while (std::getline(classFileStream, line)) {
            classes.push_back(line);
        }
        classFileStream.close();

        // 设置后端和目标设备
        net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
        net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

        qDebug() << "Model loaded successfully. Classes:" << classes.size();
        return true;
    } catch (const std::exception& e) {
        qDebug() << "Exception loading model:" << e.what();
        return false;
    } catch (...) {
        qDebug() << "Unknown exception loading model";
        return false;
    }
}

std::vector<DetectionResult> DetectorModel::detect(const cv::Mat& frame)
{
    std::vector<DetectionResult> results;

    if (frame.empty()) {
        qDebug() << "Frame is empty!";
        return results;
    }

    if (net.empty()) {
        qDebug() << "Net is empty! Model not loaded properly!";
        return results;
    }

    qDebug() << "Starting detection on frame of size:" << frame.cols << "x" << frame.rows;

    try {
        cv::Mat blob = cv::dnn::blobFromImage(frame, scaleFactor, cv::Size(inputWidth, inputHeight), mean, swapRB, false);
        net.setInput(blob);

        std::vector<cv::Mat> outputs;
        net.forward(outputs, net.getUnconnectedOutLayersNames());

        if (outputs.empty()) {
            qDebug() << "Outputs empty after forward pass!";
            return results;
        }

        qDebug() << "Output shape:" << outputs[0].size[0] << outputs[0].size[1] << outputs[0].size[2];

        std::vector<int> classIds;
        std::vector<float> confidences;
        std::vector<cv::Rect> boxes;

        float xFactor = frame.cols / static_cast<float>(inputWidth);
        float yFactor = frame.rows / static_cast<float>(inputHeight);

        const float* data = reinterpret_cast<float*>(outputs[0].data);
        const int numDetections = outputs[0].size[2];
        const int numClasses = outputs[0].size[1] - 4;

        qDebug() << "numDetections:" << numDetections << "numClasses:" << numClasses;

        int potentialDetections = 0;
        for (int i = 0; i < numDetections; ++i) {
            float maxConf = 0;
            int classId = -1;
            for (int j = 0; j < numClasses; ++j) {
                float conf = data[(4 + j) * numDetections + i];
                if (conf > maxConf) {
                    maxConf = conf;
                    classId = j;
                }
            }

            if (maxConf > confidenceThreshold) {
                potentialDetections++;
                float cx = data[i];
                float cy = data[numDetections + i];
                float w = data[2 * numDetections + i];
                float h = data[3 * numDetections + i];

                int left = static_cast<int>((cx - w / 2) * xFactor);
                int top = static_cast<int>((cy - h / 2) * yFactor);
                int width = static_cast<int>(w * xFactor);
                int height = static_cast<int>(h * yFactor);

                classIds.push_back(classId);
                confidences.push_back(maxConf);
                boxes.push_back(cv::Rect(left, top, width, height));
            }
        }

        qDebug() << "Potential detections (before NMS):" << potentialDetections;

        std::vector<int> indices;
        cv::dnn::NMSBoxes(boxes, confidences, confidenceThreshold, nmsThreshold, indices);

        qDebug() << "Final detections (after NMS):" << indices.size();

        for (int idx : indices) {
            DetectionResult result;
            result.classId = classIds[idx];
            result.className = (result.classId < classes.size()) ? classes[result.classId] : "Unknown";
            result.confidence = confidences[idx];
            result.bbox = boxes[idx];
            result.center = cv::Point(boxes[idx].x + boxes[idx].width / 2, boxes[idx].y + boxes[idx].height / 2);
            results.push_back(result);
            qDebug() << "  -" << QString::fromStdString(result.className) << "(" << result.confidence << ")";
        }
    } catch (const std::exception& e) {
        qDebug() << "Detection error:" << e.what();
    }

    qDebug() << "Detection finished. Results:" << results.size();
    return results;
}

QImage DetectorModel::matToQImage(const cv::Mat& mat)
{
    if (mat.type() == CV_8UC3) {
        return QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGB888).rgbSwapped();
    } else if (mat.type() == CV_8UC1) {
        return QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_Grayscale8);
    } else if (mat.type() == CV_8UC4) {
        return QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGBA8888);
    }
    return QImage();
}

cv::Mat DetectorModel::qImageToMat(const QImage& image)
{
    if (image.isNull()) {
        return cv::Mat();
    }

    QImage img = image.convertToFormat(QImage::Format_RGB888);
    cv::Mat mat(img.height(), img.width(), CV_8UC3, const_cast<uchar*>(img.bits()), img.bytesPerLine());
    cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
    return mat.clone();
}

std::vector<MotionBlob> DetectorModel::detectMotion(const cv::Mat& frame)
{
    std::vector<MotionBlob> result;
    
    if (frame.empty()) {
        return result;
    }

    // 如果未启用运动检测，返回空结果
    if (!motionDetectionEnabled) {
        return result;
    }

    // 检查背景减除器是否初始化
    if (!bgSubtractor) {
        qDebug() << "Error: bgSubtractor not initialized!";
        return result;
    }

    try {
        cv::Mat fgMask;
        cv::Mat grayFrame;
        
        // 转换为灰度图像
        if (frame.channels() == 3) {
            cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
        } else {
            grayFrame = frame.clone();
        }

        // 使用MOG2算法获取前景掩码
        bgSubtractor->apply(grayFrame, fgMask);

        // 形态学操作去除噪声
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
        cv::morphologyEx(fgMask, fgMask, cv::MORPH_OPEN, kernel);
        cv::morphologyEx(fgMask, fgMask, cv::MORPH_CLOSE, kernel);

        // 查找轮廓
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(fgMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        std::vector<cv::Rect> newBlobs;
        for (const auto& contour : contours) {
            double area = cv::contourArea(contour);
            // 根据灵敏度过滤小的运动区域
            if (area > motionSensitivity) {
                cv::Rect rect = cv::boundingRect(contour);
                newBlobs.push_back(rect);
            }
        }

        // 更新跟踪的运动目标
        updateTrackedBlobs(newBlobs);

        // 返回活跃的运动目标
        for (auto& blob : trackedBlobs) {
            if (blob.isActive) {
                result.push_back(blob);
            }
        }

    } catch (const std::exception& e) {
        qDebug() << "Motion detection error:" << e.what();
    } catch (...) {
        qDebug() << "Motion detection unknown error";
    }

    return result;
}

void DetectorModel::updateTrackedBlobs(const std::vector<cv::Rect>& newBlobs)
{
    // 标记所有现有blob为非活跃
    for (auto& blob : trackedBlobs) {
        blob.isActive = false;
    }

    // 匹配新检测到的blob与现有blob
    for (const auto& newRect : newBlobs) {
        cv::Point newCenter(newRect.x + newRect.width / 2, newRect.y + newRect.height / 2);
        bool matched = false;

        for (auto& blob : trackedBlobs) {
            double iou = calculateIoU(blob.boundingBox, newRect);
            if (iou > 0.3) {  // IoU阈值为0.3
                // 更新现有blob
                blob.boundingBox = newRect;
                blob.center = newCenter;
                blob.frameCount++;
                blob.isActive = true;
                matched = true;
                break;
            }
        }

        // 如果没有匹配到现有blob，创建新blob
        if (!matched) {
            MotionBlob newBlob;
            newBlob.boundingBox = newRect;
            newBlob.center = newCenter;
            newBlob.id = nextBlobId++;
            newBlob.frameCount = 1;
            newBlob.isActive = true;
            trackedBlobs.push_back(newBlob);
        }
    }

    // 清理长时间不活跃的blob（超过30帧）
    trackedBlobs.erase(std::remove_if(trackedBlobs.begin(), trackedBlobs.end(),
        [](const MotionBlob& b) { return !b.isActive && b.frameCount > 30; }),
        trackedBlobs.end());
}

double DetectorModel::calculateIoU(const cv::Rect& a, const cv::Rect& b)
{
    cv::Rect intersection = a & b;
    cv::Rect unionRect = a | b;

    if (unionRect.area() == 0) {
        return 0.0;
    }

    return static_cast<double>(intersection.area()) / static_cast<double>(unionRect.area());
}

std::vector<ZoneAlert> DetectorModel::detectZoneIntrusion(const cv::Mat& frame, const std::vector<DetectionResult>& results)
{
    std::vector<ZoneAlert> alerts;

    if (!zoneDetectionEnabled || guardZones.empty() || results.empty()) {
        return alerts;
    }

    try {
        for (const auto& zone : guardZones) {
            if (!zone.isEnabled) {
                continue;
            }

            for (const auto& result : results) {
                // 检查目标中心点是否在警戒区域内
                if (isPointInPolygon(result.center, zone.polygon)) {
                    ZoneAlert alert;
                    alert.zoneId = zone.id;
                    alert.zoneName = zone.name;
                    alert.intrusionPoint = result.center;
                    alert.objectType = result.className;
                    alert.confidence = result.confidence;
                    alerts.push_back(alert);
                }
            }
        }
    } catch (const std::exception& e) {
        qDebug() << "Zone intrusion detection error:" << e.what();
    }

    return alerts;
}

void DetectorModel::addGuardZone(const GuardZone& zone)
{
    GuardZone newZone = zone;
    if (newZone.id == 0) {
        newZone.id = nextZoneId++;
    }
    guardZones.push_back(newZone);
}

void DetectorModel::removeGuardZone(int zoneId)
{
    auto it = std::remove_if(guardZones.begin(), guardZones.end(),
        [zoneId](const GuardZone& z) { return z.id == zoneId; });
    guardZones.erase(it, guardZones.end());
}

void DetectorModel::clearGuardZones()
{
    guardZones.clear();
    nextZoneId = 1;
}

bool DetectorModel::isPointInPolygon(const cv::Point& point, const std::vector<cv::Point>& polygon)
{
    if (polygon.size() < 3) {
        return false;
    }

    bool inside = false;
    int n = polygon.size();

    for (int i = 0, j = n - 1; i < n; j = i++) {
        cv::Point pi = polygon[i];
        cv::Point pj = polygon[j];

        if (((pi.y > point.y) != (pj.y > point.y)) &&
            (point.x < (pj.x - pi.x) * (point.y - pi.y) / (pj.y - pi.y) + pi.x)) {
            inside = !inside;
        }
    }

    return inside;
}

