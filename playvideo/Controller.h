#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QFileSystemWatcher>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include <memory>
#include <QDateTime>
#include "VideoPlayer.h"

class Controller : public QObject {
    Q_OBJECT
private:
    QString filePath;
    QFileSystemWatcher watcher;
    std::shared_ptr<VideoPlayer> m_videoPlayer;
    bool m_parsingFile = false;
    int m_lastEventTime;
public:
    explicit Controller(const QString &filePath, QObject *parent = nullptr)
        : QObject(parent), filePath(filePath) {
        m_videoPlayer = std::make_shared<VideoPlayer>();
        connect(&watcher, &QFileSystemWatcher::fileChanged, this, &Controller::onFileChanged);
        watcher.addPath(filePath);
        qDebug() << "Watching file:" << filePath;
    }

private slots:
    void onFileChanged() {
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

    void readFile() {
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

    void processLine(const QString &line) {
        qDebug() << line;
        QStringList parts = line.split('=');
        if (parts.size() != 2) return;

        QString key = parts[0].trimmed();
        QString value = parts[1].trimmed();

        if (key == "VideoName") {
            qDebug() << "Video file set to:" << value;
            m_videoPlayer->start(QString(APP_PATH)+"/video/"+value);
        } else if (key == "status") {
            qDebug() << "Video status changed to:" << value;
        } else if (key == "speed") {
            qDebug() << "Playback speed changed to:" << value;
        } else if (key == "time") {
            qDebug() << "Playback time set to:" << value.toInt();
        } else {
            qWarning() << "Unknown command:" << line;
        }
    }
};

#endif // CONTROLLER_H
