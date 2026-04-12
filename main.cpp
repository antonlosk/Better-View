#include <QApplication>
#include <QStyleFactory>
#include <QFileInfo>
#include <QTranslator>
#include <QLocale>
#include "mainwindow.h"
#include "settingsmanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("BetterView");
    app.setApplicationName("BetterView");
    app.setStyle(QStyleFactory::create("Fusion"));

    // Загрузка перевода
    QTranslator translator;
    QString lang = SettingsManager::loadLanguage();
    if (lang == "system") {
        QLocale systemLocale;
        if (systemLocale.language() == QLocale::Russian) {
            lang = "ru";
        } else {
            lang = "en";
        }
    }
    if (lang == "ru") {
        if (translator.load(":/translations/BetterView_ru_RU.qm")) {
            app.installTranslator(&translator);
        }
    }

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