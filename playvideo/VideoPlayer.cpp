#include "VideoPlayer.h"
#include <QDebug>
#include <QThread>

std::mutex m_mutex;

VideoPlayer::VideoPlayer(QObject *parent) : QObject(parent), isPlaying(false){
}

VideoPlayer::~VideoPlayer() {
//    stop();
    qDebug() << "VideoPlayer is distructed!";
}

void VideoPlayer::start(const QString &videoPath) {
    this->videoPath = videoPath.toStdString();
    if (isPlaying) return;
    isPlaying = true;
    playVideo();
}

void VideoPlayer::stop() {
    isPlaying = false;
//    if (videoThread.joinable()) {
//        videoThread.join();
//    }
}

void VideoPlayer::playVideo() {
    cv::VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        std::cerr << "Error: Cannot open video file!" << std::endl;
        return;
    }

    cv::Mat frame;
    while (isPlaying && cap.read(frame)) {
        int targetWidth = 640;
        int targetHeight = 480;
        double aspectRatio = (double)frame.cols / frame.rows;
        int newWidth = targetWidth;
        int newHeight = static_cast<int>(targetWidth / aspectRatio);
        if (newHeight > targetHeight) {
            newHeight = targetHeight;
            newWidth = static_cast<int>(targetHeight * aspectRatio);
        }
        cv::resize(frame, frame, cv::Size(newWidth, newHeight)); // Resize without distortion
        cv::imshow("Video Player", frame);
        if (cv::waitKey(30) == 27) { // Nhấn ESC để thoát
            isPlaying = false;
            stop();
        }
    }

    cap.release();
    cv::destroyAllWindows();
    isPlaying = false;
}
