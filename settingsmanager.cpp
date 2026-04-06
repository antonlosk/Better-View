#include "settingsmanager.h"
#include <QSettings>

void SettingsManager::saveTheme(const QString &theme)
{
    QSettings settings("BetterView", "BetterView");
    settings.setValue("theme", theme);
}

QString SettingsManager::loadTheme()
{
    QSettings settings("BetterView", "BetterView");
    return settings.value("theme", "system").toString();
}