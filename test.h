#ifndef TEST_H
#define TEST_H
#include <QObject>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <iostream>

class VideoPlayer1 : public QObject {
    Q_OBJECT
public:
    VideoPlayer1(QObject *parent = nullptr) : QObject(parent), timer(new QTimer(this)) {
        connect(timer, &QTimer::timeout, this, &VideoPlayer1::nextFrame);
    }

    void playVideo(const std::string& videoPath) {
        cap.open(videoPath);
        if (!cap.isOpened()) {
            std::cerr << "Error: Could not open video file " << videoPath << std::endl;
            return;
        }
        timer->start(30); // Cập nhật video mỗi 30ms
    }

private slots:
    void nextFrame() {
        cv::Mat frame;
        if (!cap.read(frame)) {
            timer->stop(); // Dừng khi hết video
            return;
        }
        cv::imshow("Video", frame);
        cv::waitKey(1);
    }

private:
    cv::VideoCapture cap;
    QTimer *timer; // Sử dụng con trỏ thay vì đối tượng trực tiếp
};

#endif // TEST_H
