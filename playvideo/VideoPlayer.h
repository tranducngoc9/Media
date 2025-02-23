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
    Q_INVOKABLE void start(const QString &videoPath);
    Q_INVOKABLE void stop();

private:

    std::string videoPath;
    std::atomic<bool> isPlaying;
    QThread videoThread;
};

#endif // VIDEOPLAYER_H
