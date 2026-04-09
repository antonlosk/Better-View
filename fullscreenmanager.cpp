#include "fullscreenmanager.h"
#include "imageviewer.h"
#include <QMainWindow>
#include <QToolBar>
#include <QStatusBar>

FullscreenManager::FullscreenManager(QMainWindow *window, ImageViewer *viewer, QObject *parent)
    : QObject(parent)
    , m_window(window)
    , m_viewer(viewer)
    , m_isFullscreen(false)
    , m_wasMaximized(false)
    , m_savedZoom(1.0)
{
}

FullscreenManager::~FullscreenManager()
{
}

void FullscreenManager::toggle()
{
    if (m_isFullscreen)
        exitFullscreen();
    else
        enterFullscreen();
}

void FullscreenManager::enterFullscreen()
{
    if (m_isFullscreen) return;

    m_wasMaximized = m_window->isMaximized();
    m_savedZoom = m_viewer->getZoom();

    m_window->showFullScreen();
    m_viewer->fillToView();
    m_viewer->setZoomEnabled(false);

    QWidget *toolbar = m_window->findChild<QToolBar*>();
    if (toolbar) toolbar->hide();
    QStatusBar *statusBar = m_window->statusBar();
    if (statusBar) statusBar->hide();

    m_isFullscreen = true;
}

void FullscreenManager::exitFullscreen()
{
    if (!m_isFullscreen) return;

    if (m_wasMaximized)
        m_window->showMaximized();
    else
        m_window->showNormal();

    QWidget *toolbar = m_window->findChild<QToolBar*>();
    if (toolbar) toolbar->show();
    QStatusBar *statusBar = m_window->statusBar();
    if (statusBar) statusBar->show();

    m_viewer->setZoomEnabled(true);
    m_viewer->setZoom(m_savedZoom);

    m_isFullscreen = false;
}