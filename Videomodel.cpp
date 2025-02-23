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
        return roles;
}

void VideoModel::loadVideos(const QString &folderPath) {
    beginResetModel();
    videos.clear();
    QDir dir(folderPath.isEmpty() ? QDir::homePath() : folderPath);
    QStringList filters;
    filters << "*.mp4" << "*.avi" << "*.mkv";
    QFileInfoList files = dir.entryInfoList(filters, QDir::Files);

    for (const QFileInfo &file : files) {
        double duration = getVideoDuration(file.absoluteFilePath());
        videos.append({file.fileName(), static_cast<int>(file.size()), duration});
    }
    currentPage = 0;
    emit dataChanged(index(0), index(rowCount() - 1));
}


void VideoModel::setFolderPath(const QString &path) {
    loadVideos(path);
}

void VideoModel::nextPage() {
    if ((currentPage + 1) * pageSize < videos.size()) {
        ++currentPage;
        emit dataChanged(index(0), index(rowCount() - 1));
    }
}

void VideoModel::previousPage() {
    if (currentPage > 0) {
        --currentPage;
        emit dataChanged(index(0), index(rowCount() - 1));
    }
}

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


QVariantList VideoModel::getPagedData() {
    QVariantList pageData;
    int start = currentPage * pageSize;
    int end = qMin(start + pageSize, videos.size());

    for (int i = start; i < end; ++i) {
        QVariantMap item;
        item["name"] = videos[i].name;
        item["size"] = videos[i].size;
        item["duration"] = videos[i].duration;
        pageData.append(item);
    }
    return pageData;
}

