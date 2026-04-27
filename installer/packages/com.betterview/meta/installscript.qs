function Component()
{
    // default constructor
    // Подписываемся на событие старта установки только если запущен сам установщик
    if (installer.isInstaller()) {
        installer.installationStarted.connect(this, Component.prototype.onInstallationStarted);
    }
}

Component.prototype.onInstallationStarted = function()
{
    var appName = "BetterView.exe";
    
    // 1. Проверяем, запущена ли программа
    while (true) {
        // Вызываем системную утилиту tasklist для поиска процесса
        var res = installer.execute("tasklist", ["/FI", "IMAGENAME eq " + appName]);
        
        // Если процесс найден в выводе (приводим к нижнему регистру для надежности)
        if (res.length > 0 && res[0].toLowerCase().indexOf(appName.toLowerCase()) !== -1) {
            var btn = QMessageBox.warning("app.running", "Программа запущена", 
                "Программа " + appName + " сейчас работает.\nПожалуйста, закройте её и нажмите OK для продолжения, либо Cancel для отмены установки.", 
                QMessageBox.Ok | QMessageBox.Cancel);
                
            if (btn === QMessageBox.Cancel) {
                // Если пользователь нажал Отмена, прерываем установку
                throw new Error("Установка отменена: необходимо закрыть работающую программу.");
            }
        } else {
            break; // Программа не запущена, выходим из цикла
        }
    }

    // 2. Проверяем наличие установленной версии
    var targetDir = installer.value("TargetDir");
    var uninstallerPath = targetDir + "/maintenancetool.exe";
    
    if (installer.fileExists(uninstallerPath)) {
        var btn2 = QMessageBox.question("app.installed", "Обнаружена предыдущая версия", 
            "Найдена уже установленная версия программы.\nЕё необходимо удалить перед продолжением.\nЗапустить деинсталлятор?", 
            QMessageBox.Yes | QMessageBox.No);
            
        if (btn2 === QMessageBox.Yes) {
            // Запускаем деинсталлятор. Установщик будет ждать, пока он не закроется.
            installer.execute(uninstallerPath);
        } else {
            // Если пользователь отказывается удалять, прерываем установку
            throw new Error("Установка отменена: требуется удаление предыдущей версии.");
        }
    }
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/BetterView.exe", "@StartMenuDir@/Better View.lnk");
        component.addOperation("CreateShortcut", "@TargetDir@/BetterView.exe", "@DesktopDir@/Better View.lnk");
    }
}