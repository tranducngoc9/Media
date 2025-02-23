#include <QGuiApplication>
//#include <VideoPlayer.h>
#include <Controller.h>

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    Controller Controller(QString(APP_PATH)+"/video.txt");

//    VideoPlayer videoPlayer;


    return app.exec();
}
