#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>
#include "Videomodel.h"
#include "ControllVideo.h"



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQuickView view;

    VideoModel videoModel;
    ControllVideo controllVideo;

    videoModel.setFolderPath(QString(APP_PATH)+"/playvideo/video");

    view.rootContext()->setContextProperty("APP_PATH", QString(APP_PATH));
    view.rootContext()->setContextProperty("videoModel", &videoModel);
    view.rootContext()->setContextProperty("ControllVideo", &controllVideo);

    view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    view.show();

    return app.exec();
}
