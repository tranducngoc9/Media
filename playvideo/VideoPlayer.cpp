#include "VideoPlayer.h"

VideoPlayer::VideoPlayer() : isPlaying(false) {}

VideoPlayer::~VideoPlayer() {
    stop();
    std::cout << "VideoPlayer is destructed!" << std::endl;
}

//start to play video and send time value to media app
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

//stop play video
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

//pause video
void VideoPlayer::pause()
{
    if (isPlaying.load()) {
        isPaused.store(true);
    }
}

//resume video
void VideoPlayer::resume()
{
    if (isPlaying.load() && isPaused.load()) {
        isPaused.store(false);
        pauseCond.notify_one();  // Notify to continue to play
    }
}

//set time to start video
void VideoPlayer::seekTo(int time)
{
    std::lock_guard<std::mutex> lock(seekMutex);
    seekTime.store(time);
    pauseCond.notify_one(); // Make sure the playVideo thread doesn't get stuck on pause
}

//send time video to media app
void VideoPlayer::sendTimeLoop() {
    while (isPlaying.load()) {
        if (!isPaused.load()) {
            shm.writeTime(currentTime);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // update per 100ms
    }
}

//playvideo by while loop and opencv4
void VideoPlayer::playVideo() {
    std::cout << videoPath << std::endl;
    cv::VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        std::cerr << "Error: Cannot open video file!" << std::endl;
        return;
    }

    cv::Mat frame;
    while (isPlaying.load()) {
        { // Processing pause
            std::unique_lock<std::mutex> lock(pauseMutex);
            pauseCond.wait(lock, [this]() { return !isPaused.load() || !isPlaying.load(); });
            if (!isPlaying.load()) break;
        }

        { //  Check to see if seek is needed
            std::lock_guard<std::mutex> lock(seekMutex);
            int newSeekTime = seekTime.load();
            if (newSeekTime > 0) {
                cap.set(cv::CAP_PROP_POS_MSEC, newSeekTime);
                seekTime.store(0);
                std::cout << "Seek to: " << newSeekTime << " ms" << std::endl;
            }
        }

        if (!cap.read(frame)) break; // If the frame cannot be read, exit the loop

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
