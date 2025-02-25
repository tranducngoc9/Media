#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <thread>
#include <iostream>
#include <atomic>
#include <opencv2/opencv.hpp>
#include <mutex>
#include "SharedMemory.h"
#include <condition_variable>

class VideoPlayer {
public:
    explicit VideoPlayer();
    ~VideoPlayer();
    void playVideo();
    void start();
    void stop();
    void pause();
    void resume();

    void setSpeed(const int &value) {
        speed.store(value);
    }
    void seekTo(int time);

    void setVideoPath(const std::string &value) {
        videoPath = value;
    }
    std::string getVideoPath() {
        return videoPath;
    }
    void sendTimeLoop();

private:
    std::string videoPath;
    std::atomic<bool> isPlaying;
    std::atomic<bool> isPaused;
    std::atomic<int> speed{30};
    std::atomic<int> seekTime {0};
    std::mutex seekMutex;
    SharedMemory shm;

    std::thread videoThread;
    std::thread timeThread;

    int currentTime = 0;

    std::mutex pauseMutex;
    std::condition_variable pauseCond;
};

#endif // VIDEOPLAYER_H
