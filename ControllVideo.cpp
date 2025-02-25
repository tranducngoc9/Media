#include "ControllVideo.h"
#include <QTextStream>
#include <QFile>


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

    timeThread = std::thread(&ControllVideo::settime, this);
    timeThread.detach();
}


void ControllVideo::writeCommand(const QString &command)
{
    QFile file(QString(APP_PATH) + "/playvideo/video.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << command;
        file.close();
    }
}


ControllVideo::~ControllVideo()
{
    qDebug() << "Killing playvideo...";
    playvideoProcess->kill();  // Kill app playvideo when app Media exit
    playvideoProcess->waitForFinished();
    QProcess::execute(QString("pkill playvideo"));
    shm.writeTime(0);
    delete playvideoProcess;
}

void ControllVideo::settime()
{
    qDebug() << "call set time funtion";
    while (true) {
        m_time = shm.readTime(); // Read the time value
        if(m_isstartvideo){
            shm.writeTime(0);
            m_time = 0;
            m_isstartvideo = false;
        }
        emit timeChanged(m_time);
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Read every 1000ms
    }
}

void ControllVideo::setisstartvideo(bool isstartvideo)
{
    if (m_isstartvideo == isstartvideo)
        return;

    m_isstartvideo = isstartvideo;
    m_time = 0;
    emit timeChanged(0);
    emit isstartvideoChanged(m_isstartvideo);
}
