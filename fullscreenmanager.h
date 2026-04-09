#ifndef FULLSCREENMANAGER_H
#define FULLSCREENMANAGER_H

#include <QObject>

class QMainWindow;
class ImageViewer;

class FullscreenManager : public QObject
{
    Q_OBJECT

public:
    explicit FullscreenManager(QMainWindow *window, ImageViewer *viewer, QObject *parent = nullptr);
    ~FullscreenManager();

    void toggle();
    bool isFullscreen() const { return m_isFullscreen; }

private:
    void enterFullscreen();
    void exitFullscreen();

    QMainWindow *m_window;
    ImageViewer *m_viewer;
    bool m_isFullscreen;
    bool m_wasMaximized;
    qreal m_savedZoom;
};

#endif // FULLSCREENMANAGER_H