#include "Controller.h"

Controller::Controller(const QString &filePath, QObject *parent)
    : QObject(parent), filePath(filePath) {
    m_videoPlayer = std::make_shared<VideoPlayer>();
    connect(&watcher, &QFileSystemWatcher::fileChanged, this, &Controller::onFileChanged);
    watcher.addPath(filePath);
    qDebug() << "Watching file:" << filePath;
}

void Controller::onFileChanged()
{
    int currentTime = QDateTime::currentMSecsSinceEpoch();
    if (currentTime - m_lastEventTime < 100) {
        qDebug() << "Ignore double events from the operating system";
        return;
    }
    m_lastEventTime = currentTime;
    m_parsingFile = true;
    qDebug() << "File changed signal received!";
    QTimer::singleShot(100, this, &Controller::readFile);
}

void Controller::readFile()
{
    qDebug() << "read lfile";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        qDebug() << "processLine";
        QString line = in.readLine();
        processLine(line);
    }
    file.close();

    // Cập nhật watcher để tiếp tục theo dõi nếu file bị ghi đè
    watcher.removePath(filePath);
    watcher.addPath(filePath);
}

void Controller::processLine(const QString &line)
{
    qDebug() << line;
    QStringList parts = line.split('=');
    if (parts.size() != 2) return;

    QString key = parts[0].trimmed();
    QString value = parts[1].trimmed();

    if (key == "VideoName") {
        qDebug() << "Video file set to:" << value;
        m_videoPlayer->stop();
        m_videoPlayer->setVideoPath((QString(APP_PATH)+"/video/"+value).toStdString());
        m_videoPlayer->start();
    } else if (key == "status") {
        qDebug() << "Video status changed to:" << value;
    } else if (key == "speed") {
        qDebug() << "Playback speed changed to:" << value;
        m_videoPlayer->setSpeed(value.toInt());
    } else if (key == "time") {
        qDebug() << "Playback time set to:" << value.toInt();
        m_videoPlayer->stop();
        m_videoPlayer->setTime(value.toInt());
        m_videoPlayer->start();
    } else {
        qWarning() << "Unknown command:" << line;
    }
}
