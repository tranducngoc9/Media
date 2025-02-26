#ifndef VIDEOMODEL_H
#define VIDEOMODEL_H
#include <QAbstractListModel>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <thread>

struct VideoInfo {
    QString name;
    int size;
    double duration;
};


class VideoModel : public QAbstractListModel {
    Q_OBJECT
private:
    QList<VideoInfo> videos;
    int pageSize;
    int currentPage;
    std::thread loadListVideoThread;
public:

    enum VideoRoles { NameRole = Qt::UserRole + 1, SizeRole, DurationRole };

    explicit VideoModel(QObject *parent = nullptr) : QAbstractListModel(parent), pageSize(10), currentPage(0) {
        qDebug()<< "VideoModel is constructed!";
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override ;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override ;

    QHash<int, QByteArray> roleNames() const override;

    void loadVideos(const QString &folderPath = QString());

    void setFolderPath(const QString &path) ;

    double getVideoDuration(const QString &filePath) ;


};

#endif // VIDEOMODEL_H
