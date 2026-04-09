#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QSettings>
#include <QToolButton>
#include <QFutureWatcher>
#include <QImage>

class ImageViewer;
class GifViewer;
class FilePlaylist;
class ZoomControl;
class ThemeManager;
class FullscreenManager;
class StatusBarManager;          // добавлено
class QLabel;
class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void openFileFromCommandLine(const QString &filePath);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void openFile();
    void exitApp();
    void toggleFullscreen();
    void deleteCurrentFile();
    void rotateLeft();
    void rotateRight();
    void nextImage();
    void previousImage();
    void applyTheme(const QString &theme);
    void about();
    void onImageLoaded();

private:
    void setupUI();
    void setupToolbar();
    void setupStatusBar();
    void setupSettingsMenu();
    bool loadFile(const QString &filePath);
    void updateTitle();
    void enableControls(bool enabled);
    void updateNavigationButtons();
    void updateIconsFromTheme();

    QAction *openAction;
    QAction *rotateLeftAction;
    QAction *rotateRightAction;
    QAction *deleteAction;
    QAction *fullscreenAction;
    QAction *prevAction;
    QAction *nextAction;
    QToolButton *settingsBtn;

    QStackedWidget *stackedWidget;
    ImageViewer *imageViewer;
    GifViewer *gifViewer;
    QLabel *fileNameLabel;
    ZoomControl *zoomControl;

    QString currentFilePath;

    FilePlaylist *m_playlist;
    ThemeManager *m_themeManager;
    FullscreenManager *m_fullscreenManager;
    StatusBarManager *m_statusBarManager;   // добавлено

    QFutureWatcher<QImage> *imageWatcher;
    bool isLoading;

    QSettings settings;
};

#endif // MAINWINDOW_H