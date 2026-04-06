#ifndef FILEPLAYLIST_H
#define FILEPLAYLIST_H

#include <QObject>
#include <QString>
#include <QStringList>

/**
 * @brief The FilePlaylist class manages a list of image files in a folder.
 *        It provides navigation (next/previous) and automatic refresh when the
 *        current file changes to a different folder.
 */
class FilePlaylist : public QObject
{
    Q_OBJECT

public:
    explicit FilePlaylist(QObject *parent = nullptr);

    // Set current file path; updates the internal list if folder changed
    void setCurrent(const QString &filePath);

    // Get next/previous image path; returns empty string if none
    QString next() const;
    QString previous() const;

    // Force refresh of the playlist (e.g., after file deletion)
    void refresh();

    // Returns the current file path
    QString current() const { return m_currentPath; }

    // Returns the list of all image files in the current folder
    QStringList fileList() const { return m_fileList; }

private:
    void updateList();   // rebuilds the file list from current folder

    QString m_currentPath;
    QStringList m_fileList;
    QString m_currentFolder;
};

#endif // FILEPLAYLIST_H