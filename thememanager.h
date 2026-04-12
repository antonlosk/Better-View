#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QHash>
#include <QIcon>
#include <QColor>

class QAction;
class QToolButton;

class ThemeManager : public QObject
{
    Q_OBJECT

public:
    explicit ThemeManager(QObject *parent = nullptr);

    void applyTheme(const QString &theme);
    void updateIcons(QAction *openAction,
                     QAction *rotateLeftAction,
                     QAction *rotateRightAction,
                     QAction *deleteAction,
                     QAction *fullscreenAction,
                     QAction *prevAction,
                     QAction *nextAction,
                     QToolButton *settingsBtn);
    QString currentTheme() const { return m_currentTheme; }
    void loadAndApply();

private:
    bool isSystemDarkTheme() const;
    QIcon loadAndColorIcon(const QString &fileName, const QColor &color);
    void setPaletteForTheme(const QString &theme);

    QHash<QString, QIcon> m_iconCache;
    QString m_currentTheme;
};

#endif // THEMEMANAGER_H