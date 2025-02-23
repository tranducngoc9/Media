#ifndef CONTROLLVIDEO_H
#define CONTROLLVIDEO_H
#include <QProcess>
#include <QObject>
#include <QDebug>
class ControllVideo:  public QObject{
    Q_OBJECT

private:
    QProcess *playvideoProcess;
public:
    Q_INVOKABLE void writeCommand(const QString &command) ;
    ControllVideo();
    ~ ControllVideo();
};

#endif // CONTROLLVIDEO_H
