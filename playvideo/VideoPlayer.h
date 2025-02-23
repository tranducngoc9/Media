#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QObject>
#include <QThread>
#include <thread>
#include <QMutex>
#include <opencv2/opencv.hpp>

class VideoPlayer : public QObject {
    Q_OBJECT

public:
    explicit VideoPlayer(QObject *parent = nullptr);
    ~VideoPlayer();
    void playVideo();
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    void setSpeed(const int &value){
        speed = value;
    }
    void setTime(const int &value){
        time = value;
    }
    void setVideoPath(const std::string &value){
        videoPath = value;
    }
    std::string getVideoPath(){
        return videoPath;
    }

private:

    std::string videoPath;
    std::atomic<bool> isPlaying;
    std::atomic<int> speed{30};
    std::atomic<int> time{0};
    QThread videoThread;
    int currentTime = 0;
};

#endif // VIDEOPLAYER_H
