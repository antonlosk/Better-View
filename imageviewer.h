#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

class ImageViewer : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);
    void setPixmap(const QPixmap &pixmap);
    void clear();
    void zoomIn();
    void zoomOut();
    void resetZoom();
    void setZoom(qreal value);
    qreal getZoom() const { return currentZoom; }
    void fitToView();
    void fillToView();
    void setZoomEnabled(bool enabled) { zoomEnabled = enabled; }
    void rotate(qreal angle);

    static constexpr qreal MIN_ZOOM = 0.05;
    static constexpr qreal MAX_ZOOM = 10.0;
    static constexpr qreal ZOOM_STEP = 1.2;

signals:
    void zoomChanged(qreal value);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;  // новое

private:
    void updateZoom(qreal factor);
    void applyTransform();

    QGraphicsScene *scene;
    QGraphicsPixmapItem *pixmapItem;
    qreal currentZoom;
    bool zoomEnabled;
    bool isFillMode;
};

#endif // IMAGEVIEWER_H