#ifndef CONTROLLVIDEO_H
#define CONTROLLVIDEO_H
#include <QProcess>
#include <QObject>
#include <QDebug>
#include "SharedMemory.h"
#include <thread>

class ControllVideo:  public QObject{
    Q_OBJECT
    Q_PROPERTY(int time READ time NOTIFY timeChanged)
    Q_PROPERTY(bool isstartvideo READ isstartvideo WRITE setisstartvideo NOTIFY isstartvideoChanged)
private:
    QProcess *playvideoProcess;
    SharedMemory shm;
    int m_time;
    std::thread timeThread;

    bool m_isstartvideo = false;

public:
    Q_INVOKABLE void writeCommand(const QString &command) ;
    ControllVideo();
    ~ ControllVideo();

    int time() const
    {
        return m_time;
    }
    bool isstartvideo() const
    {
        return m_isstartvideo;
    }

public slots:
    void settime();

    void setisstartvideo(bool isstartvideo);

signals:
    void timeChanged(int time);
    void isstartvideoChanged(bool isstartvideo);
};

#endif // CONTROLLVIDEO_H
