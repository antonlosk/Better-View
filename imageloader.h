#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QImage>
#include <QPixmap>
#include <QSize>

class ImageLoader
{
public:
    static QImage loadImageAsync(const QString &filePath);
    static QPixmap rotatePixmap(const QPixmap &src, int degrees);
    static QSize getImageSize(const QString &filePath);
    static bool isAnimatedGif(const QString &filePath);
};

#endif