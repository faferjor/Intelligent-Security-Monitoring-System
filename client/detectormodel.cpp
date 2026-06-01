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

        if (outputs.empty()) {
            return results;
        }

        std::vector<int> classIds;
        std::vector<float> confidences;
        std::vector<cv::Rect> boxes;

        float xFactor = frame.cols / static_cast<float>(inputWidth);
        float yFactor = frame.rows / static_cast<float>(inputHeight);

        const float* data = reinterpret_cast<float*>(outputs[0].data);
        const int numDetections = outputs[0].size[2];
        const int numClasses = outputs[0].size[1] - 4;

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

        std::vector<int> indices;
        cv::dnn::NMSBoxes(boxes, confidences, confidenceThreshold, nmsThreshold, indices);

        for (int idx : indices) {
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

