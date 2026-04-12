#include "settingsdialog.h"
#include "settingsmanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QApplication>
#include <QMessageBox>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    loadCurrentSettings();
    applyTranslations();
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::setupUI()
{
    setWindowTitle(tr("Settings"));
    setMinimumWidth(350);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Theme selection
    QHBoxLayout *themeLayout = new QHBoxLayout();
    QLabel *themeLabel = new QLabel(tr("Theme:"), this);
    themeCombo = new QComboBox(this);
    themeCombo->addItem(tr("System"), "system");
    themeCombo->addItem(tr("Light"), "light");
    themeCombo->addItem(tr("Dark"), "dark");
    themeLayout->addWidget(themeLabel);
    themeLayout->addWidget(themeCombo);
    mainLayout->addLayout(themeLayout);

    // Language selection
    QHBoxLayout *langLayout = new QHBoxLayout();
    QLabel *langLabel = new QLabel(tr("Language:"), this);
    languageCombo = new QComboBox(this);
    languageCombo->addItem(tr("System"), "system");
    languageCombo->addItem("English", "en");
    languageCombo->addItem("Русский", "ru");
    langLayout->addWidget(langLabel);
    langLayout->addWidget(languageCombo);
    mainLayout->addLayout(langLayout);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    okButton = new QPushButton(tr("OK"), this);
    cancelButton = new QPushButton(tr("Cancel"), this);
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &SettingsDialog::onOkClicked);
    connect(cancelButton, &QPushButton::clicked, this, &SettingsDialog::onCancelClicked);
}

void SettingsDialog::loadCurrentSettings()
{
    QString savedTheme = SettingsManager::loadTheme();
    int themeIndex = themeCombo->findData(savedTheme);
    if (themeIndex >= 0)
        themeCombo->setCurrentIndex(themeIndex);

    QString savedLang = SettingsManager::loadLanguage();
    int langIndex = languageCombo->findData(savedLang);
    if (langIndex >= 0)
        languageCombo->setCurrentIndex(langIndex);
}

void SettingsDialog::applyTranslations()
{
    setWindowTitle(tr("Settings"));
    // If needed, update combobox item texts when language changes dynamically.
    // Here we just rely on initial translation.
}

void SettingsDialog::onOkClicked()
{
    m_theme = themeCombo->currentData().toString();
    m_language = languageCombo->currentData().toString();

    // If language changed, inform user that restart is required
    QString currentLang = SettingsManager::loadLanguage();
    if (m_language != currentLang) {
        QMessageBox::information(this, tr("Language Changed"),
                                 tr("The language change will take effect after restarting the application."));
    }

    accept();
}

void SettingsDialog::onCancelClicked()
{
    reject();
}

QString SettingsDialog::selectedTheme() const
{
    return m_theme;
}

QString SettingsDialog::selectedLanguage() const
{
    return m_language;
}