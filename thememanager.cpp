#include "thememanager.h"
#include "settingsmanager.h"
#include <QApplication>
#include <QPalette>
#include <QStyle>
#include <QSettings>
#include <QSvgRenderer>
#include <QPainter>
#include <QAction>
#include <QToolButton>

// Вспомогательная функция для поиска действия по tooltip (должна быть определена до использования)
static QAction* findActionByToolTip(const QList<QAction*> &actions, const QString &toolTipPart)
{
    for (QAction *a : actions) {
        if (a && a->toolTip().contains(toolTipPart))
            return a;
    }
    return nullptr;
}

ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent)
{
}

void ThemeManager::applyTheme(const QString &theme)
{
    m_currentTheme = theme;
    QString actualTheme = theme;
    if (theme == "system") {
        actualTheme = isSystemDarkTheme() ? "dark" : "light";
    }

    setPaletteForTheme(actualTheme);
    SettingsManager::saveTheme(theme);
}

void ThemeManager::loadAndApply()
{
    QString savedTheme = SettingsManager::loadTheme();
    applyTheme(savedTheme);
}

bool ThemeManager::isSystemDarkTheme() const
{
#ifdef Q_OS_WIN
    QSettings settings(R"(HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Themes\Personalize)", QSettings::NativeFormat);
    bool useLight = settings.value("AppsUseLightTheme", 1).toInt();
    return !useLight;
#else
    return qApp->styleHints()->colorScheme() == Qt::ColorScheme::Dark;
#endif
}

void ThemeManager::setPaletteForTheme(const QString &theme)
{
    QPalette palette;
    if (theme == "dark") {
        palette.setColor(QPalette::Window, QColor(32, 32, 32));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Base, QColor(25, 25, 25));
        palette.setColor(QPalette::AlternateBase, QColor(45, 45, 45));
        palette.setColor(QPalette::ToolTipBase, QColor(32, 32, 32));
        palette.setColor(QPalette::ToolTipText, Qt::white);
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Button, QColor(45, 45, 45));
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::BrightText, Qt::red);
        palette.setColor(QPalette::Link, QColor(42, 130, 218));
        palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        palette.setColor(QPalette::HighlightedText, Qt::white);
        palette.setColor(QPalette::Disabled, QPalette::Text, QColor(128, 128, 128));
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(128, 128, 128));
    } else {
        palette.setColor(QPalette::Window, QColor(240, 240, 240));
        palette.setColor(QPalette::WindowText, Qt::black);
        palette.setColor(QPalette::Base, Qt::white);
        palette.setColor(QPalette::AlternateBase, QColor(245, 245, 245));
        palette.setColor(QPalette::ToolTipBase, Qt::white);
        palette.setColor(QPalette::ToolTipText, Qt::black);
        palette.setColor(QPalette::Text, Qt::black);
        palette.setColor(QPalette::Button, QColor(240, 240, 240));
        palette.setColor(QPalette::ButtonText, Qt::black);
        palette.setColor(QPalette::BrightText, Qt::red);
        palette.setColor(QPalette::Link, QColor(0, 0, 255));
        palette.setColor(QPalette::Highlight, QColor(0, 120, 215));
        palette.setColor(QPalette::HighlightedText, Qt::white);
        palette.setColor(QPalette::Disabled, QPalette::Text, QColor(128, 128, 128));
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(128, 128, 128));
    }
    qApp->setPalette(palette);
}

QIcon ThemeManager::loadAndColorIcon(const QString &fileName, const QColor &color)
{
    QString key = fileName + QString::number(color.rgb());
    if (m_iconCache.contains(key))
        return m_iconCache[key];

    QString path = QString(":/icons/%1").arg(fileName);
    QSvgRenderer renderer(path);
    if (!renderer.isValid()) {
        return QIcon();
    }
    QPixmap pixmap(renderer.defaultSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    renderer.render(&painter);
    painter.end();

    QPixmap coloredPixmap(pixmap.size());
    coloredPixmap.fill(Qt::transparent);
    QPainter colorPainter(&coloredPixmap);
    colorPainter.setCompositionMode(QPainter::CompositionMode_Source);
    colorPainter.drawPixmap(0, 0, pixmap);
    colorPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    colorPainter.fillRect(coloredPixmap.rect(), color);
    colorPainter.end();

    QIcon result(coloredPixmap);
    m_iconCache[key] = result;
    return result;
}

void ThemeManager::updateIcons(const QList<QAction*> &actions, QToolButton *settingsBtn)
{
    QColor iconColor;
    QPalette pal = qApp->palette();
    QColor windowColor = pal.color(QPalette::Window);
    bool isDarkTheme = (windowColor.lightness() < 128);
    iconColor = isDarkTheme ? Qt::white : Qt::black;

    auto setIconForAction = [&](QAction *action, const QString &fileName) {
        if (action)
            action->setIcon(loadAndColorIcon(fileName, iconColor));
    };

    setIconForAction(findActionByToolTip(actions, "Open"), "open.svg");
    setIconForAction(findActionByToolTip(actions, "Rotate Left"), "rotate-left.svg");
    setIconForAction(findActionByToolTip(actions, "Rotate Right"), "rotate-right.svg");
    setIconForAction(findActionByToolTip(actions, "Delete"), "delete.svg");
    setIconForAction(findActionByToolTip(actions, "Fullscreen"), "fullscreen.svg");
    setIconForAction(findActionByToolTip(actions, "Previous"), "chevron-left.svg");
    setIconForAction(findActionByToolTip(actions, "Next"), "chevron-right.svg");

    if (settingsBtn)
        settingsBtn->setIcon(loadAndColorIcon("menu.svg", iconColor));
}