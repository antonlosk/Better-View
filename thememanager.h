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

    // Применяет тему (system/dark/light) и сохраняет её
    void applyTheme(const QString &theme);

    // Обновляет иконки для заданных действий и кнопки меню
    void updateIcons(const QList<QAction*> &actions, QToolButton *settingsBtn);

    // Возвращает текущую активную тему (system/dark/light)
    QString currentTheme() const { return m_currentTheme; }

    // Загружает сохранённую тему и применяет её
    void loadAndApply();

private:
    // Вспомогательные методы
    bool isSystemDarkTheme() const;
    QIcon loadAndColorIcon(const QString &fileName, const QColor &color);
    void setPaletteForTheme(const QString &theme);

    QHash<QString, QIcon> m_iconCache;
    QString m_currentTheme;
};

#endif // THEMEMANAGER_H