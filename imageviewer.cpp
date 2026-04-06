#include "imageviewer.h"
#include <QWheelEvent>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QClipboard>

ImageViewer::ImageViewer(QWidget *parent)
    : QGraphicsView(parent), currentZoom(1.0), zoomEnabled(true), isFillMode(false)
{
    scene = new QGraphicsScene(this);
    pixmapItem = new QGraphicsPixmapItem;
    scene->addItem(pixmapItem);
    setScene(scene);
    setRenderHint(QPainter::SmoothPixmapTransform);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setAcceptDrops(false);
    viewport()->setAcceptDrops(false);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void ImageViewer::setPixmap(const QPixmap &pixmap)
{
    pixmapItem->setRotation(0);
    pixmapItem->setPixmap(pixmap);
    scene->setSceneRect(pixmap.rect());
    pixmapItem->setTransformOriginPoint(pixmapItem->boundingRect().center());
    resetZoom();
    if (isFillMode)
        fillToView();
    else
        fitToView();
}

void ImageViewer::clear()
{
    pixmapItem->setPixmap(QPixmap());
    scene->setSceneRect(0,0,0,0);
    currentZoom = 1.0;
    resetTransform();
    isFillMode = false;
}

void ImageViewer::zoomIn()
{
    if (isFillMode) isFillMode = false;
    updateZoom(ZOOM_STEP);
}

void ImageViewer::zoomOut()
{
    if (isFillMode) isFillMode = false;
    updateZoom(1.0 / ZOOM_STEP);
}

void ImageViewer::resetZoom()
{
    isFillMode = false;
    currentZoom = 1.0;
    applyTransform();
}

void ImageViewer::setZoom(qreal value)
{
    if (isFillMode) isFillMode = false;
    qreal newZoom = qBound(MIN_ZOOM, value, MAX_ZOOM);
    if (qFuzzyCompare(currentZoom, newZoom))
        return;
    currentZoom = newZoom;
    applyTransform();
    emit zoomChanged(currentZoom);
}

void ImageViewer::fitToView()
{
    if (pixmapItem->pixmap().isNull())
        return;
    isFillMode = false;
    fitInView(pixmapItem, Qt::KeepAspectRatio);
    currentZoom = transform().m11();
    emit zoomChanged(currentZoom);
}

void ImageViewer::fillToView()
{
    if (pixmapItem->pixmap().isNull())
        return;
    isFillMode = true;
    QSizeF imageSize = pixmapItem->boundingRect().size();
    QSizeF viewSize = viewport()->size();
    if (viewSize.isEmpty()) return;

    qreal scaleX = viewSize.width() / imageSize.width();
    qreal scaleY = viewSize.height() / imageSize.height();
    qreal scale = qMax(scaleX, scaleY);
    currentZoom = scale;
    applyTransform();
    centerOn(pixmapItem);
    emit zoomChanged(currentZoom);
}

void ImageViewer::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    if (isFillMode && !pixmapItem->pixmap().isNull()) {
        fillToView();
    }
}

void ImageViewer::rotate(qreal angle)
{
    if (isFillMode) isFillMode = false;
    pixmapItem->setRotation(pixmapItem->rotation() + angle);
}

void ImageViewer::updateZoom(qreal factor)
{
    if (!zoomEnabled) return;
    setZoom(currentZoom * factor);
}

void ImageViewer::applyTransform()
{
    setTransform(QTransform::fromScale(currentZoom, currentZoom));
}

void ImageViewer::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        fitToView();
        event->accept();
    } else {
        QGraphicsView::mouseDoubleClickEvent(event);
    }
}

void ImageViewer::contextMenuEvent(QContextMenuEvent *event)
{
    if (pixmapItem->pixmap().isNull()) {
        event->ignore();
        return;
    }

    QMenu menu;
    QAction *copyAction = menu.addAction(tr("Copy"));
    connect(copyAction, &QAction::triggered, this, [this]() {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setPixmap(pixmapItem->pixmap());
    });
    menu.exec(event->globalPos());
    event->accept();
}

void ImageViewer::wheelEvent(QWheelEvent *event)
{
    if (zoomEnabled) {
        if (event->angleDelta().y() > 0)
            zoomIn();
        else if (event->angleDelta().y() < 0)
            zoomOut();
        event->accept();
    } else {
        event->ignore();
    }
}

void ImageViewer::keyPressEvent(QKeyEvent *event)
{
    if (zoomEnabled && (event->key() == Qt::Key_Plus || event->key() == Qt::Key_Equal))
        zoomIn();
    else if (zoomEnabled && event->key() == Qt::Key_Minus)
        zoomOut();
    else if (zoomEnabled && event->key() == Qt::Key_0) {
        fitToView();
    } else
        QGraphicsView::keyPressEvent(event);
}