#include "VideoPlayer.h"

VideoPlayer::VideoPlayer() : isPlaying(false) {}

VideoPlayer::~VideoPlayer() {
    stop();
    std::cout << "VideoPlayer is destructed!" << std::endl;
}

void VideoPlayer::start() {
    std::cout << "============== Start ===============" << std::endl;
    if (isPlaying.load()) return;

    isPlaying.store(true);
    isPaused.store(false);

    if (videoThread.joinable()) {
        videoThread.join();
    }
    timeThread  = std::thread(&VideoPlayer::sendTimeLoop, this);
    videoThread = std::thread(&VideoPlayer::playVideo, this);
}

void VideoPlayer::stop() {
    isPlaying.store(false);
    isPaused.store(false);
    pauseCond.notify_all();  // Make sure the thread doesn't get stuck while paused

    if (videoThread.joinable()) {
        videoThread.join();
    }
    if (timeThread.joinable()){
        timeThread.join();
    }
}

void VideoPlayer::pause()
{
    if (isPlaying.load()) {
        isPaused.store(true);
    }
}

void VideoPlayer::resume()
{
    if (isPlaying.load() && isPaused.load()) {
        isPaused.store(false);
        pauseCond.notify_one();  // Notify to continue to play
    }
}

void VideoPlayer::seekTo(int time)
{
    std::lock_guard<std::mutex> lock(seekMutex);
    seekTime.store(time);
    pauseCond.notify_one(); // Đảm bảo luồng playVideo không bị kẹt khi pause
}

void VideoPlayer::sendTimeLoop() {
    while (isPlaying.load()) {
        if (!isPaused.load()) {
            shm.writeTime(currentTime);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // update per 500ms
    }
}

void VideoPlayer::playVideo() {
    std::cout << videoPath << std::endl;
    cv::VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        std::cerr << "Error: Cannot open video file!" << std::endl;
        return;
    }

    cv::Mat frame;
    while (isPlaying.load()) {
        { // Xử lý tạm dừng
            std::unique_lock<std::mutex> lock(pauseMutex);
            pauseCond.wait(lock, [this]() { return !isPaused.load() || !isPlaying.load(); });
            if (!isPlaying.load()) break;
        }

        { // Kiểm tra xem có cần seek không
            std::lock_guard<std::mutex> lock(seekMutex);
            int newSeekTime = seekTime.load();
            if (newSeekTime > 0) {
                cap.set(cv::CAP_PROP_POS_MSEC, newSeekTime);
                seekTime.store(0);
                std::cout << "Seek to: " << newSeekTime << " ms" << std::endl;
            }
        }

        if (!cap.read(frame)) break; // Nếu không đọc được frame, thoát vòng lặp

        currentTime = cap.get(cv::CAP_PROP_POS_MSEC);

        int targetWidth = 640, targetHeight = 480;
        double aspectRatio = (double)frame.cols / frame.rows;
        int newWidth = targetWidth, newHeight = static_cast<int>(targetWidth / aspectRatio);
        if (newHeight > targetHeight) {
            newHeight = targetHeight;
            newWidth = static_cast<int>(targetHeight * aspectRatio);
        }

        cv::resize(frame, frame, cv::Size(newWidth, newHeight));
        cv::imshow("Video Player", frame);

        int key = cv::waitKey(speed.load());
        if (key == 27 || !isPlaying.load()) break;
    }

    currentTime = 0;
    shm.writeTime(currentTime);
    cap.release();
    cv::destroyAllWindows();
    isPlaying.store(false);
    std::cout << "^^^^^^^^^^^^^^End playVideo()^^^^^^^^^^^^^^" << std::endl;
}
