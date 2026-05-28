#include "detectormodel.h"
#include <QDebug>
#include <fstream>
#include <QFile>

DetectorModel::DetectorModel(QObject *parent) : QObject(parent)
{
}

DetectorModel::~DetectorModel()
{
}

bool DetectorModel::loadModel(const std::string& modelPath, const std::string& classesPath)
{
    try {
        net = cv::dnn::readNetFromONNX(modelPath);
        if (net.empty()) {
            qDebug() << "Failed to load model:" << QString::fromStdString(modelPath);
            return false;
        }

        std::ifstream classFile(classesPath);
        if (!classFile.is_open()) {
            qDebug() << "Failed to open classes file:" << QString::fromStdString(classesPath);
            return false;
        }

        std::string line;
        while (std::getline(classFile, line)) {
            classes.push_back(line);
        }
        classFile.close();

        net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
        net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

        qDebug() << "Model loaded successfully. Classes:" << classes.size();
        return true;
    } catch (const std::exception& e) {
        qDebug() << "Error loading model:" << e.what();
        return false;
    }
}

std::vector<DetectionResult> DetectorModel::detect(const cv::Mat& frame)
{
    std::vector<DetectionResult> results;

    if (frame.empty() || net.empty()) {
        return results;
    }

    try {
        cv::Mat blob = cv::dnn::blobFromImage(frame, scaleFactor, cv::Size(inputWidth, inputHeight), mean, swapRB, false);
        net.setInput(blob);

        std::vector<cv::Mat> outputs;
        net.forward(outputs, net.getUnconnectedOutLayersNames());

        std::vector<int> classIds;
        std::vector<float> confidences;
        std::vector<cv::Rect> boxes;

        float xFactor = frame.cols / static_cast<float>(inputWidth);
        float yFactor = frame.rows / static_cast<float>(inputHeight);

        float* data = reinterpret_cast<float*>(outputs[0].data);
        const int rows = 8400;
        const int dimensions = 85;

        for (int i = 0; i < rows; ++i) {
            float confidence = data[4];
            if (confidence >= confidenceThreshold) {
                float* classesScores = data + 5;
                cv::Mat scores(1, classes.size(), CV_32FC1, classesScores);
                cv::Point classIdPoint;
                double maxClassScore;
                cv::minMaxLoc(scores, 0, &maxClassScore, 0, &classIdPoint);

                if (maxClassScore > confidenceThreshold) {
                    confidences.push_back(confidence);
                    classIds.push_back(classIdPoint.x);

                    float x = data[0];
                    float y = data[1];
                    float w = data[2];
                    float h = data[3];

                    int left = static_cast<int>((x - 0.5 * w) * xFactor);
                    int top = static_cast<int>((y - 0.5 * h) * yFactor);
                    int width = static_cast<int>(w * xFactor);
                    int height = static_cast<int>(h * yFactor);

                    boxes.push_back(cv::Rect(left, top, width, height));
                }
            }
            data += dimensions;
        }

        std::vector<int> indices;
        cv::dnn::NMSBoxes(boxes, confidences, confidenceThreshold, nmsThreshold, indices);

        for (int i = 0; i < indices.size(); ++i) {
            int idx = indices[i];
            DetectionResult result;
            result.classId = classIds[idx];
            result.className = (result.classId < classes.size()) ? classes[result.classId] : "Unknown";
            result.confidence = confidences[idx];
            result.bbox = boxes[idx];
            result.center = cv::Point(boxes[idx].x + boxes[idx].width / 2, boxes[idx].y + boxes[idx].height / 2);
            results.push_back(result);
        }
    } catch (const std::exception& e) {
        qDebug() << "Detection error:" << e.what();
    }

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
    if (image.format() == QImage::Format_RGB888) {
        return cv::Mat(image.height(), image.width(), CV_8UC3, const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();
    } else if (image.format() == QImage::Format_Grayscale8) {
        return cv::Mat(image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();
    } else if (image.format() == QImage::Format_RGBA8888) {
        return cv::Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();
    }
    return cv::Mat();
}

