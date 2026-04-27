#ifndef GIFVIEWER_H
#define GIFVIEWER_H

#include <QLabel>
#include <QMovie>

class GifViewer : public QLabel
{
    Q_OBJECT

public:
    explicit GifViewer(QWidget *parent = nullptr);
    ~GifViewer();
    bool loadGif(const QString &filePath);
    void stop();
    QSize getOriginalSize() const { return originalSize; }

signals:
    void propertyRequested();   // новый сигнал

protected:
    void resizeEvent(QResizeEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    void updateScaledPixmap();

    QMovie *movie;
    QSize originalSize;
};

#endif // GIFVIEWER_H