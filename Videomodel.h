#ifndef VIDEOMODEL_H
#define VIDEOMODEL_H
#include <QAbstractListModel>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

struct VideoInfo {
    QString name;
    int size;
};

class VideoModel : public QAbstractListModel {
    Q_OBJECT
private:
    QList<VideoInfo> videos;
    int pageSize;
    int currentPage;
public:
    Q_INVOKABLE int getSize(){
        return videos.count();
    }
    void getList(){
        for(auto i : videos){
            qDebug() << i.name << i.size;
        }
    }

    enum VideoRoles { NameRole = Qt::UserRole + 1, SizeRole };

    explicit VideoModel(QObject *parent = nullptr) : QAbstractListModel(parent), pageSize(10), currentPage(0) {
        qDebug()<< "VideoModel is constructed!";
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override ;


    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override ;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void loadVideos(const QString &folderPath = QString());

    Q_INVOKABLE void setFolderPath(const QString &path) ;

    Q_INVOKABLE void nextPage();

    Q_INVOKABLE void previousPage() ;

    Q_INVOKABLE void setPageSize(int size);
    Q_INVOKABLE QVariantList getPagedData();

};

#endif // VIDEOMODEL_H
