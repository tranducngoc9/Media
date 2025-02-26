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
    ControllVideo();
    ~ ControllVideo();

    Q_INVOKABLE void writeCommand(const QString &command) ;
    int time() const;
    bool isstartvideo() const;

public slots:
    void settime();
    void setisstartvideo(bool isstartvideo);

signals:
    void timeChanged(int time);
    void isstartvideoChanged(bool isstartvideo);
};

#endif // CONTROLLVIDEO_H
