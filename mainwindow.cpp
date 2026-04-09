#include "mainwindow.h"
#include "imageviewer.h"
#include "gifviewer.h"
#include "imageloader.h"
#include "filemanager.h"
#include "settingsmanager.h"
#include "fileplaylist.h"
#include "zoomcontrol.h"
#include "thememanager.h"
#include "fullscreenmanager.h"
#include "statusbarmanager.h"
#include "version.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QLabel>
#include <QToolButton>
#include <QMenu>
#include <QApplication>
#include <QPalette>
#include <QStyle>
#include <QSettings>
#include <QDir>
#include <QSvgRenderer>
#include <QPainter>
#include <QtConcurrent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    openAction(nullptr),
    rotateLeftAction(nullptr),
    rotateRightAction(nullptr),
    deleteAction(nullptr),
    fullscreenAction(nullptr),
    prevAction(nullptr),
    nextAction(nullptr),
    settingsBtn(nullptr),
    stackedWidget(nullptr),
    imageViewer(nullptr),
    gifViewer(nullptr),
    fileNameLabel(nullptr),
    zoomControl(nullptr),
    isLoading(false),
    settings("BetterView", "BetterView")
{
    m_playlist = new FilePlaylist(this);
    m_themeManager = new ThemeManager(this);

    setupUI();
    setupToolbar();
    setupStatusBar();
    setupSettingsMenu();
    setAcceptDrops(true);
    resize(800, 600);
    enableControls(false);
    m_themeManager->loadAndApply();
    updateIconsFromTheme();

    m_fullscreenManager = new FullscreenManager(this, imageViewer, this);
    m_statusBarManager = new StatusBarManager(statusBar(), this);

    imageWatcher = new QFutureWatcher<QImage>(this);
    connect(imageWatcher, &QFutureWatcher<QImage>::finished, this, &MainWindow::onImageLoaded);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    stackedWidget = new QStackedWidget(this);
    imageViewer = new ImageViewer(this);
    gifViewer = new GifViewer(this);
    stackedWidget->addWidget(imageViewer);
    stackedWidget->addWidget(gifViewer);
    setCentralWidget(stackedWidget);
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::setupToolbar()
{
    QToolBar *toolbar = addToolBar(tr("Toolbar"));
    toolbar->setMovable(false);
    toolbar->setContextMenuPolicy(Qt::PreventContextMenu);

    openAction = new QAction(this);
    openAction->setToolTip(tr("Open (Ctrl+O)"));
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    toolbar->addAction(openAction);

    toolbar->addSeparator();

    rotateLeftAction = new QAction(tr("↺"), this);
    rotateLeftAction->setToolTip(tr("Rotate Left"));
    connect(rotateLeftAction, &QAction::triggered, this, &MainWindow::rotateLeft);
    toolbar->addAction(rotateLeftAction);

    rotateRightAction = new QAction(tr("↻"), this);
    rotateRightAction->setToolTip(tr("Rotate Right"));
    connect(rotateRightAction, &QAction::triggered, this, &MainWindow::rotateRight);
    toolbar->addAction(rotateRightAction);

    toolbar->addSeparator();

    deleteAction = new QAction(this);
    deleteAction->setToolTip(tr("Delete"));
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteCurrentFile);
    toolbar->addAction(deleteAction);

    toolbar->addSeparator();

    QWidget *spacerLeft = new QWidget();
    spacerLeft->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolbar->addWidget(spacerLeft);

    fileNameLabel = new QLabel(tr("No file"));
    fileNameLabel->setAlignment(Qt::AlignCenter);
    toolbar->addWidget(fileNameLabel);

    QWidget *spacerRight = new QWidget();
    spacerRight->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolbar->addWidget(spacerRight);
}

void MainWindow::setupStatusBar()
{
    QStatusBar *statusBar = this->statusBar();
    statusBar->setSizeGripEnabled(false);

    zoomControl = new ZoomControl(this);
    statusBar->addPermanentWidget(zoomControl);
    connect(zoomControl, &ZoomControl::zoomRequested, imageViewer, &ImageViewer::setZoom);
    connect(imageViewer, &ImageViewer::zoomChanged, zoomControl, &ZoomControl::setZoom);

    QAction *zoomOutAction = new QAction(tr("-"), this);
    zoomOutAction->setToolTip(tr("Zoom Out"));
    connect(zoomOutAction, &QAction::triggered, this, [this]() { imageViewer->zoomOut(); });
    QToolButton *zoomOutBtn = new QToolButton;
    zoomOutBtn->setDefaultAction(zoomOutAction);
    statusBar->addPermanentWidget(zoomOutBtn);

    QAction *zoomInAction = new QAction(tr("+"), this);
    zoomInAction->setToolTip(tr("Zoom In"));
    connect(zoomInAction, &QAction::triggered, this, [this]() { imageViewer->zoomIn(); });
    QToolButton *zoomInBtn = new QToolButton;
    zoomInBtn->setDefaultAction(zoomInAction);
    statusBar->addPermanentWidget(zoomInBtn);

    statusBar->addPermanentWidget(new QLabel("  "));

    fullscreenAction = new QAction(tr("⛶"), this);
    fullscreenAction->setToolTip(tr("Fullscreen (F11)"));
    connect(fullscreenAction, &QAction::triggered, this, &MainWindow::toggleFullscreen);
    QToolButton *fullscreenBtn = new QToolButton;
    fullscreenBtn->setDefaultAction(fullscreenAction);
    statusBar->addPermanentWidget(fullscreenBtn);
}

void MainWindow::setupSettingsMenu()
{
    QToolBar *toolbar = findChild<QToolBar*>();
    if (!toolbar) return;

    prevAction = new QAction(tr("◀"), this);
    prevAction->setToolTip(tr("Previous (Left Arrow)"));
    connect(prevAction, &QAction::triggered, this, &MainWindow::previousImage);
    toolbar->addAction(prevAction);

    nextAction = new QAction(tr("▶"), this);
    nextAction->setToolTip(tr("Next (Right Arrow)"));
    connect(nextAction, &QAction::triggered, this, &MainWindow::nextImage);
    toolbar->addAction(nextAction);

    toolbar->addSeparator();

    settingsBtn = new QToolButton(toolbar);
    settingsBtn->setText(tr("⋮"));
    QFont font = settingsBtn->font();
    font.setPointSize(font.pointSize() + 2);
    settingsBtn->setFont(font);
    settingsBtn->setToolTip(tr("Menu"));
    settingsBtn->setPopupMode(QToolButton::InstantPopup);

    QMenu *menu = new QMenu(settingsBtn);
    QMenu *themeMenu = menu->addMenu(tr("Theme"));
    QAction *themeSystem = new QAction(tr("System"), this);
    QAction *themeDark = new QAction(tr("Dark"), this);
    QAction *themeLight = new QAction(tr("Light"), this);
    connect(themeSystem, &QAction::triggered, this, [this]() { applyTheme("system"); });
    connect(themeDark, &QAction::triggered, this, [this]() { applyTheme("dark"); });
    connect(themeLight, &QAction::triggered, this, [this]() { applyTheme("light"); });
    themeMenu->addAction(themeSystem);
    themeMenu->addAction(themeDark);
    themeMenu->addAction(themeLight);
    menu->addSeparator();
    QAction *aboutAction = new QAction(tr("About"), this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
    menu->addAction(aboutAction);
    menu->addSeparator();
    QAction *exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &MainWindow::exitApp);
    menu->addAction(exitAction);

    settingsBtn->setMenu(menu);
    toolbar->addWidget(settingsBtn);
}

void MainWindow::applyTheme(const QString &theme)
{
    m_themeManager->applyTheme(theme);
    updateIconsFromTheme();
}

void MainWindow::updateIconsFromTheme()
{
    QList<QAction*> actions;
    if (openAction) actions.append(openAction);
    if (rotateLeftAction) actions.append(rotateLeftAction);
    if (rotateRightAction) actions.append(rotateRightAction);
    if (deleteAction) actions.append(deleteAction);
    if (fullscreenAction) actions.append(fullscreenAction);
    if (prevAction) actions.append(prevAction);
    if (nextAction) actions.append(nextAction);
    m_themeManager->updateIcons(actions, settingsBtn);
}

void MainWindow::toggleFullscreen()
{
    m_fullscreenManager->toggle();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F11) {
        toggleFullscreen();
        event->accept();
    } else if (event->key() == Qt::Key_Escape && m_fullscreenManager->isFullscreen()) {
        toggleFullscreen();
        event->accept();
    } else if (event->key() == Qt::Key_Left) {
        previousImage();
        event->accept();
    } else if (event->key() == Qt::Key_Right) {
        nextImage();
        event->accept();
    } else if ((event->key() == Qt::Key_0) && (event->modifiers() & Qt::ControlModifier)) {
        imageViewer->fitToView();
        event->accept();
    } else if (event->key() == Qt::Key_0) {
        imageViewer->fitToView();
        event->accept();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Better View"),
                       tr("<h3>Better View</h3>"
                          "<p>Version: %1</p>"
                          "<p>Image Viewer for Windows 10</p>"
                          "<p>Author: <b>antonlosk</b></p>"
                          "<p>GitHub: <a href=\"https://github.com/antonlosk/Better-View\">"
                          "https://github.com/antonlosk/Better-View</a></p>").arg(VERSION_STRING));
}

void MainWindow::onImageLoaded()
{
    isLoading = false;
    QImage img = imageWatcher->result();
    if (!img.isNull()) {
        QPixmap pixmap = QPixmap::fromImage(img);
        imageViewer->setPixmap(pixmap);
        stackedWidget->setCurrentWidget(imageViewer);
        m_statusBarManager->updateFileInfo(currentFilePath, false, QSize());
        enableControls(true);
        updateNavigationButtons();
        QFileInfo fi(currentFilePath);
        if (fileNameLabel->text() != fi.fileName())
            fileNameLabel->setText(fi.fileName());
        updateTitle();
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Cannot load file:\n%1").arg(currentFilePath));
        fileNameLabel->setText(tr("No file"));
        updateTitle();
        enableControls(false);
    }
}

void MainWindow::openFileFromCommandLine(const QString &filePath)
{
    loadFile(filePath);
}

bool MainWindow::loadFile(const QString &filePath)
{
    if (isLoading) return false;

    QFileInfo fi(filePath);
    if (!fi.exists()) return false;

    QString canonicalPath = fi.canonicalFilePath();
    if (canonicalPath.isEmpty()) canonicalPath = fi.absoluteFilePath();
    currentFilePath = canonicalPath;
    m_playlist->setCurrent(currentFilePath);

    QString suffix = fi.suffix().toLower();

    if (suffix == "gif") {
        if (ImageLoader::isAnimatedGif(filePath)) {
            if (gifViewer->loadGif(filePath)) {
                stackedWidget->setCurrentWidget(gifViewer);
                updateTitle();
                fileNameLabel->setText(fi.fileName());
                QSize gifSize = gifViewer->getOriginalSize();
                m_statusBarManager->updateFileInfo(currentFilePath, true, gifSize);
                enableControls(false);
                updateNavigationButtons();
                return true;
            } else {
                QMessageBox::warning(this, tr("Error"), tr("Cannot load GIF file:\n%1").arg(filePath));
                return false;
            }
        }
    }

    isLoading = true;
    updateTitle();

    QFuture<QImage> future = QtConcurrent::run([filePath]() {
        return ImageLoader::loadImageAsync(filePath);
    });
    imageWatcher->setFuture(future);
    return true;
}

void MainWindow::updateTitle()
{
    setWindowTitle("Better View");
}

void MainWindow::enableControls(bool enabled)
{
    QList<QAction*> actions = findChildren<QAction*>();
    for (QAction *a : actions) {
        if (a->toolTip().contains("Rotate") || a->toolTip().contains("Delete") ||
            a->toolTip().contains("Zoom") || a->text() == "+" || a->text() == "-") {
            a->setEnabled(enabled);
        }
    }
    if (zoomControl) zoomControl->setEnabled(enabled);
    updateNavigationButtons();
}

void MainWindow::updateNavigationButtons()
{
    if (!prevAction || !nextAction) return;
    if (currentFilePath.isEmpty()) {
        prevAction->setEnabled(false);
        nextAction->setEnabled(false);
        return;
    }
    bool hasNext = !m_playlist->next().isEmpty();
    bool hasPrev = !m_playlist->previous().isEmpty();
    prevAction->setEnabled(hasPrev);
    nextAction->setEnabled(hasNext);
}

void MainWindow::openFile()
{
    QString filter = tr("Images (*.jpg *.jpeg *.jpe *.png *.gif *.webp *.bmp *.ico *.tiff *.tif *.svg *.svgz *.heif)");
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Image"), QString(), filter);
    if (!filePath.isEmpty()) {
        QFileInfo fi(filePath);
        fileNameLabel->setText(fi.fileName());
        loadFile(filePath);
    }
}

void MainWindow::exitApp() { close(); }

void MainWindow::deleteCurrentFile()
{
    if (currentFilePath.isEmpty() || isLoading) return;
    if (QMessageBox::question(this, tr("Delete"), tr("Delete file?"), QMessageBox::Yes|QMessageBox::No) != QMessageBox::Yes)
        return;

    if (stackedWidget->currentWidget() == gifViewer) {
        gifViewer->stop();
    } else {
        imageViewer->clear();
    }

    if (FileManager::deleteFile(currentFilePath)) {
        m_playlist->refresh();
        QString next = m_playlist->next();
        if (next.isEmpty()) next = m_playlist->previous();
        if (!next.isEmpty()) {
            loadFile(next);
        } else {
            currentFilePath.clear();
            updateTitle();
            fileNameLabel->setText(tr("No file"));
            m_statusBarManager->clear();
            enableControls(false);
        }
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Cannot delete file."));
    }
}

void MainWindow::rotateLeft()
{
    if (stackedWidget->currentWidget() == imageViewer)
        imageViewer->rotate(-90.0);
}

void MainWindow::rotateRight()
{
    if (stackedWidget->currentWidget() == imageViewer)
        imageViewer->rotate(90.0);
}

void MainWindow::nextImage()
{
    if (isLoading) return;
    QString next = m_playlist->next();
    if (!next.isEmpty()) {
        QFileInfo fi(next);
        fileNameLabel->setText(fi.fileName());
        loadFile(next);
    }
}

void MainWindow::previousImage()
{
    if (isLoading) return;
    QString prev = m_playlist->previous();
    if (!prev.isEmpty()) {
        QFileInfo fi(prev);
        fileNameLabel->setText(fi.fileName());
        loadFile(prev);
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) event->acceptProposedAction();
    else event->ignore();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) return;
    QString filePath = urls.first().toLocalFile();
    QFileInfo fi(filePath);
    QStringList supportedExt = { "jpg", "jpeg", "jpe", "png", "gif", "webp", "bmp", "ico", "tiff", "tif", "svg", "svgz", "heif" };
    if (supportedExt.contains(fi.suffix().toLower())) {
        fileNameLabel->setText(fi.fileName());
        loadFile(filePath);
    } else {
        QMessageBox::information(this, tr("Not supported"), tr("File format not supported."));
    }
}