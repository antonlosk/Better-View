#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QStringList>

class FileManager
{
public:
    static QStringList getImageFilesInFolder(const QString &filePath);
    static QString getNextImage(const QString &currentFile);
    static QString getPreviousImage(const QString &currentFile);
    static bool deleteFile(const QString &filePath);
    static double getFileSizeMB(const QString &filePath);
    static QStringList getSupportedExtensions();
};

#endif