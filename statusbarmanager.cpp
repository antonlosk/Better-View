#include "statusbarmanager.h"
#include "filemanager.h"
#include "imageloader.h"
#include <QStatusBar>
#include <QLabel>
#include <QFileInfo>

StatusBarManager::StatusBarManager(QStatusBar *statusBar, QObject *parent)
    : QObject(parent)
    , m_statusBar(statusBar)
{
    if (!m_statusBar) return;

    m_resolutionLabel = new QLabel(tr("Resolution: -- x --"));
    m_fileSizeLabel = new QLabel(tr("Size: --"));
    m_statusBar->addWidget(m_resolutionLabel);
    m_statusBar->addWidget(m_fileSizeLabel);
}

StatusBarManager::~StatusBarManager() {}

void StatusBarManager::updateFileInfo(const QString &filePath, bool isGif, const QSize &gifSize)
{
    if (filePath.isEmpty()) {
        clear();
        return;
    }

    // Определяем размер файла
    QFileInfo fi(filePath);
    qint64 bytes = fi.size();
    if (bytes < 1024 * 1024) {
        double sizeKB = bytes / 1024.0;
        m_fileSizeLabel->setText(tr("Size: %1 KB").arg(sizeKB, 0, 'f', 2));
    } else {
        double sizeMB = bytes / (1024.0 * 1024.0);
        m_fileSizeLabel->setText(tr("Size: %1 MB").arg(sizeMB, 0, 'f', 2));
    }

    // Разрешение
    if (isGif) {
        if (gifSize.isValid()) {
            m_resolutionLabel->setText(tr("Resolution: %1 x %2 (GIF)").arg(gifSize.width()).arg(gifSize.height()));
        } else {
            m_resolutionLabel->setText(tr("Resolution: -- x -- (GIF)"));
        }
    } else {
        QSize sz = ImageLoader::getImageSize(filePath);
        if (sz.isValid()) {
            m_resolutionLabel->setText(tr("Resolution: %1 x %2").arg(sz.width()).arg(sz.height()));
        } else {
            m_resolutionLabel->setText(tr("Resolution: -- x --"));
        }
    }
}

void StatusBarManager::clear()
{
    m_resolutionLabel->setText(tr("Resolution: -- x --"));
    m_fileSizeLabel->setText(tr("Size: --"));
}