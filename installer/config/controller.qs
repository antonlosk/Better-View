function Controller() {
    this.translator = new QTranslator();
    var systemLanguage = installer.language(); // "ru_RU", "en_US" и т.д.
    var shortLang = systemLanguage.substring(0, 2); // "ru", "en" ...
    
    // Загружаем перевод только для русского языка (имя файла фиксированное)
    if (shortLang === "ru") {
        var translationFile = "translations/BetterView_ru_RU.qm";
        if (installer.fileExists(translationFile)) {
            this.translator.load(translationFile);
            QCoreApplication.installTranslator(this.translator);
        }
    }
    // Для всех остальных языков останутся исходные английские строки в qsTr()
}

Controller.prototype.IntroductionPageCallback = function() {
    if (installer.isInstaller()) {
        var previousInstallPath = installer.findPath("Better View");

        if (previousInstallPath !== "") {
            var mtPath = previousInstallPath + "/maintenancetool.exe";

            if (installer.fileExists(mtPath)) {
                var result = installer.execute(mtPath, ["uninstall", "--silent"]);

                if (result !== 0) {
                    var widget = gui.currentPageWidget();
                    widget.ErrorLabel.setText("<font color='red'>" +
                        qsTr("Failed to automatically remove the previous version from:\n%1\nPlease remove it manually and restart the installation.")
                        .arg(previousInstallPath) +
                        "</font>");
                    installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
                    installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
                    installer.setDefaultPageVisible(QInstaller.ReadyForInstallation, false);
                    installer.setDefaultPageVisible(QInstaller.PerformInstallation, false);
                    return;
                }
            } else {
                var widget = gui.currentPageWidget();
                widget.ErrorLabel.setText("<font color='red'>" +
                    qsTr("Previous version folder found:\n%1\nBut the uninstaller utility is missing. Installation cannot continue.")
                    .arg(previousInstallPath) +
                    "</font>");
                installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
                installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
                installer.setDefaultPageVisible(QInstaller.ReadyForInstallation, false);
                installer.setDefaultPageVisible(QInstaller.PerformInstallation, false);
                return;
            }
        }
    }
}