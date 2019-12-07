#include "settingsdialog.h"

#include <QDir>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QSpacerItem>
#include <QStandardPaths>
#include <QFileDialog>
#include <QDebug>

#include "staticdata.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    mLoader = new QLineEdit;
    mLoader->setEnabled(false);
    mLoader->setMinimumWidth(300);

    QPushButton *browseLoaderButton = new QPushButton("Browse");

    QHBoxLayout *loaderPathLayout = new QHBoxLayout;
    loaderPathLayout->addWidget(new QLabel("Executable Path"));
    loaderPathLayout->addWidget(mLoader);
    loaderPathLayout->addWidget(browseLoaderButton);

    QVBoxLayout *loaderLayout = new QVBoxLayout;
    loaderLayout->addLayout(loaderPathLayout);

    QGroupBox *loaderBox = new QGroupBox("Loader");
    loaderBox->setLayout(loaderLayout);

    mAccountName = new QLineEdit;
    mSteamId = new QLineEdit;
    mLanguage = new QLineEdit;

    mAccountName->setMaximumWidth(200);
    mSteamId->setMaximumWidth(200);
    mLanguage->setMaximumWidth(100);

    QHBoxLayout *settingsUsernameLayout = new QHBoxLayout;
    settingsUsernameLayout->addWidget(mAccountName);
    settingsUsernameLayout->addStretch();

    QHBoxLayout *settingsSteamIdLayout = new QHBoxLayout;
    settingsSteamIdLayout->addWidget(mSteamId);
    settingsSteamIdLayout->addStretch();

    QHBoxLayout *settingsLanguageLayout = new QHBoxLayout;
    settingsLanguageLayout->addWidget(mLanguage);
    settingsLanguageLayout->addStretch();

    QGridLayout *settingsLayout = new QGridLayout;
    settingsLayout->addWidget(new QLabel("Account Name"), 0, 0);
    settingsLayout->addWidget(new QLabel("Steam ID"), 1, 0);
    settingsLayout->addWidget(new QLabel("Language"), 2, 0);
    settingsLayout->addLayout(settingsUsernameLayout, 0, 1);
    settingsLayout->addLayout(settingsSteamIdLayout, 1, 1);
    settingsLayout->addLayout(settingsLanguageLayout, 2, 1);

    QGroupBox *settingBox = new QGroupBox("Settings");
    settingBox->setLayout(settingsLayout);

    QPushButton *okButton = new QPushButton("Save");
    QPushButton *cancelButton = new QPushButton("Cancel");

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(loaderBox);
    layout->addWidget(settingBox);
    layout->addLayout(buttonLayout);

    setLayout(layout);
    setWindowTitle("Goldberg Settings");
    setWindowIcon(QIcon(":/images/settings.png"));

    connect(browseLoaderButton, &QPushButton::clicked, this, &SettingsDialog::onBrowseLoaderClicked);
    connect(okButton, &QPushButton::clicked, this, &SettingsDialog::save);
    connect(cancelButton, &QPushButton::clicked, this, &SettingsDialog::reject);

    load();
}

QByteArray SettingsDialog::readFromFile(const QString &filename)
{
    QByteArray buf;
    QFile i(filename);
    if (i.open(QIODevice::ReadOnly | QIODevice::Text)) {
        buf = i.readAll().trimmed();
        i.close();
    }

    return buf;
}

void SettingsDialog::writeToFile(const QString &filename, const QByteArray &value)
{
    QFile i(filename);
    if (i.open(QIODevice::WriteOnly | QIODevice::Text)) {
        i.write(value);
        i.close();
    }
}

void SettingsDialog::load()
{
    QDir appDataDir(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first());
    appDataDir.cdUp();

    QString goldbergDataPath = appDataDir.path() + "/Goldberg SteamEmu Saves";
    mGoldbergAccountNamePath = goldbergDataPath + "/settings/account_name.txt";
    mGoldbergSteamIdPath = goldbergDataPath + "/settings/user_steam_id.txt";
    mGoldbergLanguagePath = goldbergDataPath + "/settings/language.txt";

    QString accountName = readFromFile(mGoldbergAccountNamePath);
    QString language = readFromFile(mGoldbergLanguagePath);

    if (accountName.isEmpty())
        accountName = "Goldberg";

    if (language.isEmpty())
        language = "english";

    mLoader->setText(QDir::toNativeSeparators(StaticData::instance().loader));
    mAccountName->setText(accountName);
    mSteamId->setText(readFromFile(mGoldbergSteamIdPath));
    mLanguage->setText(language);
}

void SettingsDialog::save()
{
    StaticData::instance().loader = QDir::fromNativeSeparators(mLoader->text());
    writeToFile(mGoldbergAccountNamePath, mAccountName->text().toUtf8().trimmed());
    writeToFile(mGoldbergSteamIdPath, mSteamId->text().toUtf8().trimmed());
    writeToFile(mGoldbergLanguagePath, mLanguage->text().toUtf8().trimmed());

    accept();
}

void SettingsDialog::onBrowseLoaderClicked()
{
    QFileInfo info(StaticData::instance().loader);
    QString filename = QFileDialog::getOpenFileName(this, "Browse Goldberg Loader", info.canonicalPath(), "Executable (*.exe)");
    if (filename.isEmpty())
        return;

    mLoader->setText(QDir::toNativeSeparators(filename));
}
