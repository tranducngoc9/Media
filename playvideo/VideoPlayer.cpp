#include "VideoPlayer.h"
#include <QDebug>
#include <QThread>

std::mutex m_mutex;

VideoPlayer::VideoPlayer(QObject *parent) : QObject(parent), isPlaying(false){
}

VideoPlayer::~VideoPlayer() {
    stop();
    qDebug() << "VideoPlayer is distructed!";
}

void VideoPlayer::start() {
    if (isPlaying) return;
    isPlaying = true;
    playVideo();
}

void VideoPlayer::stop() {
    isPlaying = false;
}

void VideoPlayer::playVideo() {
    cv::VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        std::cerr << "Error: Cannot open video file!" << std::endl;
        return;
    }
    //choose time position
    cap.set(cv::CAP_PROP_POS_MSEC, time);

    cv::Mat frame;
    while (isPlaying && cap.read(frame)) {
        currentTime = cap.get(cv::CAP_PROP_POS_MSEC);  //get current time
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
        if (cv::waitKey(speed) == 27) { // Press ESC to to exit
            isPlaying = false;
            stop();
        }
    }

    cap.release();
    cv::destroyAllWindows();
    isPlaying = false;
}
