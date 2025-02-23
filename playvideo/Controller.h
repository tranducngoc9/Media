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
    explicit Controller(const QString &filePath, QObject *parent = nullptr);

private slots:
    void onFileChanged();

    void readFile();

    void processLine(const QString &line);
};

#endif // CONTROLLER_H
