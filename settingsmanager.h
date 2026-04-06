#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QString>

class SettingsManager
{
public:
    static void saveTheme(const QString &theme);
    static QString loadTheme();
};

#endif