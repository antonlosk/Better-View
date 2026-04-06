#include "imageloader.h"
#include <QImageReader>
#include <QTransform>

QImage ImageLoader::loadImageAsync(const QString &filePath)
{
    QImageReader reader(filePath);
    return reader.read();
}

QPixmap ImageLoader::rotatePixmap(const QPixmap &src, int degrees)
{
    if (degrees == 0) return src;
    QTransform transform;
    transform.rotate(degrees);
    return src.transformed(transform, Qt::SmoothTransformation);
}

QSize ImageLoader::getImageSize(const QString &filePath)
{
    QImageReader reader(filePath);
    return reader.size();
}

bool ImageLoader::isAnimatedGif(const QString &filePath)
{
    QImageReader reader(filePath);
    return reader.supportsAnimation() && reader.imageCount() > 1;
}