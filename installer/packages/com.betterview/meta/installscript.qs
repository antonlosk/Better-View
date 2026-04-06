function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/BetterView.exe", "@StartMenuDir@/Better View.lnk");
        component.addOperation("CreateShortcut", "@TargetDir@/BetterView.exe", "@DesktopDir@/Better View.lnk");
    }
}
