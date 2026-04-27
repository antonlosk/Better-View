function Component() {
    // конструктор по умолчанию
}

Component.prototype.createOperations = function() {
    component.createOperations();

    // Попросить установщик закрыть BetterView.exe, если он запущен
    component.addStopProcessForUpdateRequest("BetterView.exe");

    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/BetterView.exe", "@StartMenuDir@/Better View.lnk");
        component.addOperation("CreateShortcut", "@TargetDir@/BetterView.exe", "@DesktopDir@/Better View.lnk");
    }
}