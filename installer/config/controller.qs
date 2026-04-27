function Controller() {
    this.translator = new QTranslator();
    var systemLanguage = installer.language();
    var shortLang = systemLanguage.substring(0, 2);

    if (shortLang === "ru") {
        var installerDir = installer.value("InstallerDir");
        var translationFile = installerDir + "/translations/BetterView_ru_RU.qm";
        if (installer.fileExists(translationFile)) {
            this.translator.load(translationFile);
            QCoreApplication.installTranslator(this.translator);
        }
    }
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
                        qsTranslate("Controller", "Failed to automatically remove the previous version from:\n%1\nPlease remove it manually and restart the installation.")
                        .arg(previousInstallPath) + "</font>");
                    installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
                    installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
                    installer.setDefaultPageVisible(QInstaller.ReadyForInstallation, false);
                    installer.setDefaultPageVisible(QInstaller.PerformInstallation, false);
                    return;
                }
            } else {
                var widget = gui.currentPageWidget();
                widget.ErrorLabel.setText("<font color='red'>" +
                    qsTranslate("Controller", "Previous version folder found:\n%1\nBut the uninstaller utility is missing. Installation cannot continue.")
                    .arg(previousInstallPath) + "</font>");
                installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
                installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
                installer.setDefaultPageVisible(QInstaller.ReadyForInstallation, false);
                installer.setDefaultPageVisible(QInstaller.PerformInstallation, false);
                return;
            }
        }
    }
}