#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QComboBox;
class QPushButton;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    QString selectedTheme() const;
    QString selectedLanguage() const;

private slots:
    void onOkClicked();
    void onCancelClicked();

private:
    void setupUI();
    void loadCurrentSettings();
    void applyTranslations();

    QComboBox *themeCombo;
    QComboBox *languageCombo;
    QPushButton *okButton;
    QPushButton *cancelButton;

    QString m_theme;
    QString m_language;
};

#endif // SETTINGSDIALOG_H