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
    playvideoProcess->start( QString(APP_PATH) + "/playvideo/build/playvideo");  //excute playvideo binary

    timeThread = std::thread(&ControllVideo::settime, this);
    timeThread.detach();
}

/*   send to signal to playvideo app by writing video.txt file
VideoName= name of video
status= resume or pause
speed=10 or 15 or 30 or 60 or 120 (x3, x2, x1, x0.5, x0.25  )
time= miliseconds
*/
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

int ControllVideo::time() const
{
    return m_time;
}

bool ControllVideo::isstartvideo() const
{
    return m_isstartvideo;
}

//Get video time from playvideo app by using Sharememory
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
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Read every 100ms
    }
}

//notify start to playvideo and reset previous time
void ControllVideo::setisstartvideo(bool isstartvideo)
{
    if (m_isstartvideo == isstartvideo)
        return;

    m_isstartvideo = isstartvideo;
    m_time = 0;
    emit timeChanged(0);
    emit isstartvideoChanged(m_isstartvideo);
}
