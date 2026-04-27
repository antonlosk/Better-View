function Controller() {}

// Словарь переводов
var translations = {
    "ru": {
        "auto_uninstall_failed": "Не удалось автоматически удалить предыдущую версию из:\n{0}\nПожалуйста, удалите её вручную и повторите установку.",
        "maintenance_tool_missing": "Найдена папка предыдущей версии:\n{0}\nНо утилита удаления отсутствует. Установка невозможна."
    },
    "en": {
        "auto_uninstall_failed": "Failed to automatically remove the previous version from:\n{0}\nPlease remove it manually and restart the installation.",
        "maintenance_tool_missing": "Previous version folder found:\n{0}\nBut the uninstaller utility is missing. Installation cannot continue."
    }
};

// Функция перевода
function tr(key, param) {
    // Берём первые два символа языка системы (например, "ru", "en")
    var lang = installer.language().substring(0, 2);
    
    if (translations[lang] && translations[lang][key]) {
        var text = translations[lang][key];
        return param ? text.replace("{0}", param) : text;
    }
    
    // Если перевода нет, возвращаем английскую версию (или сам ключ)
    return (translations["en"] && translations["en"][key]) ? translations["en"][key] : key;
}

Controller.prototype.IntroductionPageCallback = function() {
    if (installer.isInstaller()) {
        // Ищем путь предыдущей установки
        var previousInstallPath = installer.findPath("Better View");

        if (previousInstallPath !== "") {
            var mtPath = previousInstallPath + "/maintenancetool.exe";

            if (installer.fileExists(mtPath)) {
                var result = installer.execute(mtPath, ["uninstall", "--silent"]);

                if (result !== 0) {
                    var widget = gui.currentPageWidget();
                    var errorMsg = tr("auto_uninstall_failed", previousInstallPath);
                    widget.ErrorLabel.setText("<font color='red'>" + errorMsg + "</font>");
                    
                    installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
                    installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
                    installer.setDefaultPageVisible(QInstaller.ReadyForInstallation, false);
                    installer.setDefaultPageVisible(QInstaller.PerformInstallation, false);
                    return;
                }
            } else {
                var widget = gui.currentPageWidget();
                var errorMsg = tr("maintenance_tool_missing", previousInstallPath);
                widget.ErrorLabel.setText("<font color='red'>" + errorMsg + "</font>");
                
                installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
                installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
                installer.setDefaultPageVisible(QInstaller.ReadyForInstallation, false);
                installer.setDefaultPageVisible(QInstaller.PerformInstallation, false);
                return;
            }
        }
        // Если путь не найден – установка продолжается
    }
}