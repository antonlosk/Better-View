#include <QApplication>
#include <QStyleFactory>
#include <QFileInfo>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("BetterView");
    app.setApplicationName("BetterView");
    app.setStyle(QStyleFactory::create("Fusion"));

    MainWindow w;
    w.show();

    if (argc > 1) {
        QString filePath = QString::fromLocal8Bit(argv[1]);
        QFileInfo fi(filePath);
        if (fi.exists()) {
            w.openFileFromCommandLine(fi.canonicalFilePath());
        }
    }

    return app.exec();
}