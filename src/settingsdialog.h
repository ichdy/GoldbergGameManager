#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QLineEdit;
class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = nullptr);

public slots:
    void load();
    void save();

private slots:
    void onBrowseLoaderClicked();

private:
    QByteArray readFromFile(const QString &filename);
    void writeToFile(const QString &filename, const QByteArray &value);

private:
    QLineEdit *mLoader;
    QLineEdit *mAccountName;
    QLineEdit *mSteamId;
    QLineEdit *mLanguage;
    QString mGoldbergAccountNamePath;
    QString mGoldbergSteamIdPath;
    QString mGoldbergLanguagePath;
};

#endif // SETTINGSDIALOG_H
