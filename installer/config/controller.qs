function Controller() {}

// Словарь переводов
var translations = {
    "ru": {
        "maintenance_tool_missing": "Найдена папка предыдущей версии:\n%1\nНо утилита удаления отсутствует. Установка будет продолжена, однако старые файлы могут остаться.",
        "manual_uninstall_notice": "Будет запущена программа удаления предыдущей версии. Пожалуйста, нажмите «Удалить» в появившемся окне, чтобы полностью удалить старую версию перед установкой новой."
    },
    "en": {
        "maintenance_tool_missing": "Previous version folder found:\n%1\nUninstaller utility is missing. Installation will continue, but old files may remain.",
        "manual_uninstall_notice": "The uninstaller for the previous version will now start. Please click «Remove» in the window that appears to completely remove the old version before installing the new one."
    }
};

function tr(key, param) {
    var lang = installer.language().substring(0, 2);
    if (translations[lang] && translations[lang][key]) {
        var text = translations[lang][key];
        return param ? text.replace("%1", param) : text;
    }
    return (translations["en"] && translations["en"][key]) ? translations["en"][key].replace("%1", param || "") : key;
}

Controller.prototype.IntroductionPageCallback = function() {
    if (installer.isInstaller()) {
        var previousInstallPath = installer.findPath("Better View");

        if (previousInstallPath !== "") {
            var mtPath = previousInstallPath + "/maintenancetool.exe";

            if (installer.fileExists(mtPath)) {
                // Показываем информационное сообщение, чтобы пользователь знал, что сейчас будет
                QMessageBox.information(
                    "Better View",
                    tr("manual_uninstall_notice"),
                    QMessageBox.Ok
                );

                // Запускаем утилиту обслуживания БЕЗ аргументов – откроется окно,
                // где можно выбрать «Удалить». Ждём завершения.
                installer.execute(mtPath);

                // После того как окно удаления закрыто, продолжаем установку.
                // Если пользователь передумал и не удалил – старая версия останется,
                // новая всё равно установится поверх (файлы перезапишутся).
            } else {
                QMessageBox.warning(
                    "Better View",
                    tr("maintenance_tool_missing", previousInstallPath),
                    QMessageBox.Ok
                );
            }
        }
    }
}