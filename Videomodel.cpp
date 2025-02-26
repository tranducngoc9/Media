#include "Videomodel.h"
#include <QProcess>

int VideoModel::rowCount(const QModelIndex &parent ) const {
    Q_UNUSED(parent)
    return videos.count();
}

QVariant VideoModel::data(const QModelIndex &index, int role ) const  {
    if (index.row() < 0 || index.row() >= videos.count()){
        qDebug() << "empty!";
        return {};
    }
    switch (role) {
    case NameRole:
        return videos[index.row()].name;
    case SizeRole:
        return videos[index.row()].size;
    case DurationRole:
        return videos[index.row()].duration;
    default:
        return {};
    }
}

QHash<int, QByteArray> VideoModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[SizeRole] = "size";
    roles[DurationRole] = "duration";
    return roles;
}

// Funtion to load videos information such as: name, size, duration
void VideoModel::loadVideos(const QString &folderPath) {
    beginResetModel();
    videos.clear();
    QDir dir(folderPath.isEmpty() ? QDir::homePath() : folderPath); //check if is empty
    QStringList filters;
    filters << "*.mp4" << "*.avi" << "*.mkv";
    QFileInfoList files = dir.entryInfoList(filters, QDir::Files); //choose files with mp4, avi, mkv format

    int index = videos.size();

    for (const QFileInfo &file : files) {
        double duration = getVideoDuration(file.absoluteFilePath());
        VideoInfo video{file.fileName(), static_cast<int>(file.size()), duration};

        // Send update request to main thread
        QMetaObject::invokeMethod(this, [this, index, video]() {
            beginInsertRows(QModelIndex(), index, index); //Notify QAbstractListModel that start to insert a new row at the index position
            videos.append(video);
            endInsertRows();
            emit dataChanged(this->index(index, 0), this->index(index, 0)); //this model has only 1 column
        }, Qt::QueuedConnection);

        ++index;
    }
}

//set folder that cotain videos to start load videos information
void VideoModel::setFolderPath(const QString &path) {
    loadListVideoThread = std::thread(&VideoModel::loadVideos, this, path);
    loadListVideoThread.detach();
}

//get duration each video by using FFmpeg lib
double VideoModel::getVideoDuration(const QString &filePath)
{
    QProcess process;
    process.setProgram("ffprobe");
    process.setArguments({"-v", "error", "-show_entries", "format=duration",
                          "-of", "default=noprint_wrappers=1:nokey=1", filePath});
    process.start();
    process.waitForFinished();

    QString output = process.readAllStandardOutput().trimmed();
    return output.toDouble();
}
