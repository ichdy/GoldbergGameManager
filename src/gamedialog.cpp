#include "gamedialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QIntValidator>
#include <QDir>

GameDialog::GameDialog(QWidget *parent) :
    QDialog(parent)
{
    mTitle = new QLineEdit;
    mExe = new QLineEdit;
    mExeRunDir = new QLineEdit;
    mExeCommandLine = new QLineEdit;
    mAppId = new QLineEdit;

    mAppId->setValidator(new QIntValidator(mAppId));
    mAppId->setAlignment(Qt::AlignCenter);

    mExe->setEnabled(false);
    mExeRunDir->setEnabled(false);

    mExe->setMinimumWidth(300);
    mAppId->setMaximumWidth(80);

    QPushButton *okButton = new QPushButton("Save");
    QPushButton *cancelButton = new QPushButton("Cancel");

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new QLabel("Title"), 0, 0);
    layout->addWidget(new QLabel("Exe"), 1, 0);
    layout->addWidget(new QLabel("ExeRunDir"), 2, 0);
    layout->addWidget(new QLabel("ExeCommandLine"), 3, 0);
    layout->addWidget(new QLabel("AppId"), 4, 0);

    layout->addWidget(mTitle, 0, 1);
    layout->addWidget(mExe, 1, 1);
    layout->addWidget(mExeRunDir, 2, 1);
    layout->addWidget(mExeCommandLine, 3, 1);
    layout->addWidget(mAppId, 4, 1);

    layout->addLayout(buttonLayout, 5, 0, 1, 2);

    setLayout(layout);
    setWindowTitle("Game Properties");
    setWindowIcon(QIcon(":/images/properties.png"));

    connect(okButton, &QPushButton::clicked, this, &GameDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &GameDialog::reject);
}

void GameDialog::setTitle(const QString &title)
{
    mTitle->setText(title);
}

void GameDialog::setExe(const QString &filename)
{
    mExe->setText(QDir::toNativeSeparators(filename));
}

void GameDialog::setExeRunDir(const QString &dirpath)
{
    mExeRunDir->setText(QDir::toNativeSeparators(dirpath));
}

void GameDialog::setExeCommandLine(const QString &args)
{
    mExeCommandLine->setText(args);
}

void GameDialog::setAppId(int id)
{
    mAppId->setText(QString::number(id));
}

QString GameDialog::Title()
{
    return mTitle->text();
}

QString GameDialog::Exe()
{
    return QDir::fromNativeSeparators(mExe->text());
}

QString GameDialog::ExeRunDir()
{
    return QDir::fromNativeSeparators(mExeRunDir->text());
}

QString GameDialog::ExeCommandLine()
{
    return mExeCommandLine->text();
}

int GameDialog::AppId()
{
    return mAppId->text().toInt();
}
