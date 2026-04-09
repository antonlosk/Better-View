#ifndef STATUSBARMANAGER_H
#define STATUSBARMANAGER_H

#include <QObject>

class QStatusBar;
class QLabel;

class StatusBarManager : public QObject
{
    Q_OBJECT

public:
    explicit StatusBarManager(QStatusBar *statusBar, QObject *parent = nullptr);
    ~StatusBarManager();

    void updateFileInfo(const QString &filePath, bool isGif, const QSize &gifSize);
    void clear();

private:
    QStatusBar *m_statusBar;
    QLabel *m_resolutionLabel;
    QLabel *m_fileSizeLabel;
};

#endif // STATUSBARMANAGER_H