#include "fileplaylist.h"
#include "filemanager.h"
#include <QFileInfo>
#include <QDir>

FilePlaylist::FilePlaylist(QObject *parent)
    : QObject(parent)
{
}

void FilePlaylist::setCurrent(const QString &filePath)
{
    if (m_currentPath == filePath)
        return;

    m_currentPath = filePath;
    updateList();
}

void FilePlaylist::updateList()
{
    if (m_currentPath.isEmpty()) {
        m_fileList.clear();
        m_currentFolder.clear();
        return;
    }

    QFileInfo fi(m_currentPath);
    QString folder = fi.canonicalPath();
    if (folder != m_currentFolder) {
        m_currentFolder = folder;
        m_fileList = FileManager::getImageFilesInFolder(m_currentPath);
    } else {
        // Folder didn't change, but maybe files were added/deleted externally?
        // For simplicity, we trust the cache; call refresh() to force update.
    }
}

QString FilePlaylist::next() const
{
    if (m_currentPath.isEmpty())
        return QString();

    int idx = m_fileList.indexOf(m_currentPath);
    if (idx >= 0 && idx + 1 < m_fileList.size())
        return m_fileList[idx + 1];
    return QString();
}

QString FilePlaylist::previous() const
{
    if (m_currentPath.isEmpty())
        return QString();

    int idx = m_fileList.indexOf(m_currentPath);
    if (idx > 0)
        return m_fileList[idx - 1];
    return QString();
}

void FilePlaylist::refresh()
{
    // Force rebuild even if folder didn't change
    if (!m_currentPath.isEmpty()) {
        QFileInfo fi(m_currentPath);
        m_currentFolder = fi.canonicalPath();
        m_fileList = FileManager::getImageFilesInFolder(m_currentPath);
    }
}