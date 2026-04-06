#include "filemanager.h"
#include <QDir>
#include <QFileInfo>

QStringList FileManager::getSupportedExtensions()
{
    return { "jpg", "jpeg", "png", "gif", "webp" };
}

QStringList FileManager::getImageFilesInFolder(const QString &filePath)
{
    if (filePath.isEmpty())
        return QStringList();
    QFileInfo fi(filePath);
    QDir dir(fi.canonicalPath());
    if (!dir.exists()) return QStringList();

    QStringList filters;
    for (const QString &ext : getSupportedExtensions()) {
        filters << "*." + ext;
    }
    QStringList files = dir.entryList(filters, QDir::Files | QDir::Readable, QDir::Name);
    QStringList fullPaths;
    for (const QString &file : files) {
        fullPaths.append(dir.absoluteFilePath(file));
    }
    return fullPaths;
}

QString FileManager::getNextImage(const QString &currentFile)
{
    QStringList images = getImageFilesInFolder(currentFile);
    int idx = images.indexOf(currentFile);
    if (idx >= 0 && idx + 1 < images.size())
        return images[idx + 1];
    return QString();
}

QString FileManager::getPreviousImage(const QString &currentFile)
{
    QStringList images = getImageFilesInFolder(currentFile);
    int idx = images.indexOf(currentFile);
    if (idx > 0)
        return images[idx - 1];
    return QString();
}

bool FileManager::deleteFile(const QString &filePath)
{
    return QFile::remove(filePath);
}

double FileManager::getFileSizeMB(const QString &filePath)
{
    QFileInfo fi(filePath);
    return fi.size() / (1024.0 * 1024.0);
}