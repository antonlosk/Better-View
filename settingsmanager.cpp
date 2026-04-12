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

void SettingsManager::saveLanguage(const QString &lang)
{
    QSettings settings("BetterView", "BetterView");
    settings.setValue("language", lang);
}

QString SettingsManager::loadLanguage()
{
    QSettings settings("BetterView", "BetterView");
    return settings.value("language", "system").toString();
}