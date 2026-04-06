#include "gifviewer.h"
#include <QResizeEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QClipboard>

GifViewer::GifViewer(QWidget *parent)
    : QLabel(parent), movie(nullptr)
{
    setAlignment(Qt::AlignCenter);
    setScaledContents(false);
    setAcceptDrops(false);
}

GifViewer::~GifViewer()
{
    stop();
}

bool GifViewer::loadGif(const QString &filePath)
{
    stop();

    movie = new QMovie(filePath);
    if (!movie->isValid()) {
        delete movie;
        movie = nullptr;
        return false;
    }

    originalSize = movie->frameRect().size();
    setMovie(movie);
    movie->start();
    updateScaledPixmap();
    return true;
}

void GifViewer::stop()
{
    if (movie) {
        movie->stop();
        delete movie;
        movie = nullptr;
    }
    clear();
}

void GifViewer::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);
    updateScaledPixmap();
}

void GifViewer::updateScaledPixmap()
{
    if (!movie || originalSize.isNull())
        return;

    QSize widgetSize = size();
    if (widgetSize.width() <= 0 || widgetSize.height() <= 0)
        return;

    QSize scaledSize = originalSize.scaled(widgetSize, Qt::KeepAspectRatio);
    movie->setScaledSize(scaledSize);
}

void GifViewer::contextMenuEvent(QContextMenuEvent *event)
{
    if (!movie || movie->currentPixmap().isNull()) {
        event->ignore();
        return;
    }

    QMenu menu;
    QAction *copyAction = menu.addAction(tr("Copy"));
    connect(copyAction, &QAction::triggered, this, [this]() {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setPixmap(movie->currentPixmap());
    });
    menu.exec(event->globalPos());
    event->accept();
}