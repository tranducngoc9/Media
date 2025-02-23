#include "ControllVideo.h"
#include <QTextStream>
#include <QFile>


void ControllVideo::writeCommand(const QString &command)
{
    QFile file(QString(APP_PATH) + "/playvideo/video.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << command;
        file.close();
    }
}

ControllVideo::ControllVideo()
{
    qDebug()<< APP_PATH;
    QProcess::execute(QString("pkill playvideo"));
    if (!QFile::exists(QString(APP_PATH) + "/playvideo/build/playvideo" )) {
        qDebug() << "Building app playvideo...";
        QProcess::execute(QString("qmake %1/playvideo/playvideo.pro -o %1/playvideo/build/Makefile ").arg(APP_PATH) );
        QProcess::execute(QString("make -C %1/playvideo/build").arg(APP_PATH));
    }
    qDebug() << "Running App playvideo...";
    playvideoProcess = new QProcess();
    playvideoProcess->start( QString(APP_PATH) + "/playvideo/build/playvideo");
}

ControllVideo::~ControllVideo()
{
    qDebug() << "Killing playvideo...";
    playvideoProcess->kill();  // Kill app playvideo when app Media exit
    playvideoProcess->waitForFinished();
    QProcess::execute(QString("pkill playvideo"));
    delete playvideoProcess;
}
