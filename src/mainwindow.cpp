#include "mainwindow.h"

#include <QApplication>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QToolButton>
#include <QListView>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QSettings>
#include <QJsonDocument>
#include <QMessageBox>
#include <QFileIconProvider>
#include <QProcess>
#include <QDebug>

#include "define.h"
#include "staticdata.h"
#include "settingsdialog.h"
#include "gamedialog.h"
#include "gameitemdelegate.h"
#include "aboutdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mAddButton = new QToolButton;
    mDeleteButton = new QToolButton;
    mSettingsButton = new QToolButton;
    mAboutButton = new QToolButton;

    mAddButton->setText("Add Game");
    mDeleteButton->setText("Delete Game");
    mSettingsButton->setText("Settings");
    mAboutButton->setText("About");

    mAddButton->setToolTip("Add Game");
    mDeleteButton->setToolTip("Delete Game");
    mSettingsButton->setToolTip("Goldberg Settings");
    mAboutButton->setToolTip("About");

    mAddButton->setIcon(QIcon(":/images/add.png"));
    mDeleteButton->setIcon(QIcon(":/images/delete.png"));
    mSettingsButton->setIcon(QIcon(":/images/settings.png"));
    mAboutButton->setIcon(QIcon(":/images/info.png"));

    mAddButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mDeleteButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mSettingsButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mAboutButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    mDeleteButton->setEnabled(false);

    QToolBar *toolBar = new QToolBar;
    toolBar->setMovable(false);
    toolBar->addWidget(mAddButton);
    toolBar->addWidget(mDeleteButton);
    toolBar->addSeparator();
    toolBar->addWidget(mSettingsButton);
    toolBar->addWidget(mAboutButton);
    addToolBar(toolBar);

    mGameView = new QListView;
    mGameView->setSelectionMode(QAbstractItemView::SingleSelection);
    mGameView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mGameView->setItemDelegate(new GameItemDelegate(mGameView));
    mGameView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mGameView->setContextMenuPolicy(Qt::CustomContextMenu);

    QStandardItemModel *model = new QStandardItemModel;
    mGameView->setModel(model);

    mRunAction = new QAction("Run", this);
    mPropertiesAction = new QAction("Properties", this);

    mRunAction->setIcon(QIcon(":/images/run.png"));
    mPropertiesAction->setIcon(QIcon(":/images/properties.png"));

    mPopupMenu = new QMenu(this);
    mPopupMenu->addAction(mRunAction);
    mPopupMenu->addAction(mPropertiesAction);

    setCentralWidget(mGameView);
    setWindowTitle("Goldberg Game Manager");
    setWindowIcon(QIcon(":/images/logo.png"));
    resize(350, 600);

    connect(mAddButton, &QToolButton::clicked, this, &MainWindow::addGame);
    connect(mDeleteButton, &QToolButton::clicked, this, &MainWindow::deleteGame);
    connect(mSettingsButton, &QToolButton::clicked, this, &MainWindow::settings);
    connect(mAboutButton, &QToolButton::clicked, this, &MainWindow::about);

    connect(mRunAction, &QAction::triggered, this, &MainWindow::runGame);
    connect(mPropertiesAction, &QAction::triggered, this, &MainWindow::gameProperties);

    connect(mGameView, &QListView::customContextMenuRequested, this, &MainWindow::onCustomContextMenuRequested);
    connect(mGameView, &QListView::activated, this, &MainWindow::runGame);
    connect(mGameView->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::onCurrentSelectionChanged);
}

void MainWindow::start()
{
    show();
    load();
    check();
}

bool MainWindow::check()
{
    StaticData &data = StaticData::instance();

    if (data.loader.isEmpty() || !QFile::exists(data.loader)) {
        QMessageBox::information(this, "Information", "Please set Goldberg Loader Executable first!");
        QString filename = QFileDialog::getOpenFileName(this, "Browse Goldberg Loader", QString(), "Executable (*.exe)");
        if (filename.isEmpty())
            return false;

        data.loader = filename;
        save();
    }

    return true;
}

void MainWindow::load()
{
    QByteArray json;
    {
        QString appDirPath = qApp->applicationDirPath();
        QFile f(appDirPath + "/settings.json");
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        json = f.readAll();
        f.close();
    }

    QVariantMap settingMap = QJsonDocument::fromJson(json).toVariant().toMap();
    StaticData::instance().loader = settingMap["loader"].toString();
    QVariantList gameList = settingMap["games"].toList();
    for (int i=0; i<gameList.size(); i++)
        appendGame(gameList.at(i).toMap());
}

void MainWindow::save()
{
    QVariantMap settingsMap;
    settingsMap["loader"] = StaticData::instance().loader;

    QVariantList gameList;
    QStandardItemModel *model = static_cast<QStandardItemModel *>(mGameView->model());
    for (int i=0; i<model->rowCount(); i++) {
        QModelIndex index = model->index(i, 0);
        QVariantMap gameMap;
        gameMap["Title"] = index.data(GameTitleRole).toString();
        gameMap["Exe"] = index.data(GameExeRole).toString();
        gameMap["ExeRunDir"] = index.data(GameExeRunDirRole).toString();
        gameMap["ExeCommandLine"] = index.data(GameExeCommandLineRole).toString();
        gameMap["AppId"] = index.data(GameAppIdRole).toInt();
        gameList << gameMap;
    }

    settingsMap["games"] = gameList;

    QString appDirPath = qApp->applicationDirPath();
    QFile f(appDirPath + "/settings.json");
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to save settings.");
        return;
    }

    f.write(QJsonDocument::fromVariant(settingsMap).toJson());
    f.close();
}

void MainWindow::appendGame(const QVariantMap &gameMap)
{
    QFileIconProvider provider;

    QStandardItem *item = new QStandardItem;
    item->setData(gameMap["Title"], GameTitleRole);
    item->setData(gameMap["Exe"], GameExeRole);
    item->setData(gameMap["ExeRunDir"], GameExeRunDirRole);
    item->setData(gameMap["ExeCommandLine"], GameExeCommandLineRole);
    item->setData(gameMap["AppId"], GameAppIdRole);
    item->setData(provider.icon(QFileInfo(gameMap["Exe"].toString())), GameIconRole);

    QStandardItemModel *model = static_cast<QStandardItemModel *>(mGameView->model());
    model->appendRow(item);
}

void MainWindow::addGame()
{
    QString filename = QFileDialog::getOpenFileName(this, "Browse Game Executable", QString(), "Executable (*.exe;*.bat;*.cmd)");
    if (filename.isEmpty())
        return;

    QFileInfo info(filename);
    GameDialog dialog(this);
    dialog.setTitle(info.fileName().section(".",  0, -2));
    dialog.setExe(info.canonicalFilePath());
    dialog.setExeRunDir(info.canonicalPath());

    if (dialog.exec() == QDialog::Rejected)
        return;

    QVariantMap gameMap;
    gameMap["Title"] = dialog.Title();
    gameMap["Exe"] = dialog.Exe();
    gameMap["ExeRunDir"] = dialog.ExeRunDir();
    gameMap["ExeCommandLine"] = dialog.ExeCommandLine();
    gameMap["AppId"] = dialog.AppId();
    appendGame(gameMap);

    save();
}

void MainWindow::deleteGame()
{
    QModelIndex index = mGameView->currentIndex();
    if (!index.isValid())
        return;

    int res = QMessageBox::question(this, "Confirmation", "Do you really want to delete this game?");
    if (res != QMessageBox::Yes)
        return;

    QStandardItemModel *model = static_cast<QStandardItemModel *>(mGameView->model());
    model->removeRow(index.row());

    save();
}

void MainWindow::gameProperties()
{
    QModelIndex index = mGameView->currentIndex();
    if (!index.isValid())
        return;

    QStandardItemModel *model = static_cast<QStandardItemModel *>(mGameView->model());
    QStandardItem *item = model->item(index.row());

    GameDialog dialog(this);
    dialog.setTitle(item->data(GameTitleRole).toString());
    dialog.setExe(item->data(GameExeRole).toString());
    dialog.setExeRunDir(item->data(GameExeRunDirRole).toString());
    dialog.setExeCommandLine(item->data(GameExeCommandLineRole).toString());
    dialog.setAppId(item->data(GameAppIdRole).toInt());
    if (dialog.exec() == QDialog::Rejected)
        return;

    item->setData(dialog.Title(), GameTitleRole);
    item->setData(dialog.Exe(), GameExeRole);
    item->setData(dialog.ExeRunDir(), GameExeRunDirRole);
    item->setData(dialog.ExeCommandLine(), GameExeCommandLineRole);
    item->setData(dialog.AppId(), GameAppIdRole);

    save();
}

void MainWindow::runGame()
{
    QModelIndex index = mGameView->currentIndex();
    if (!index.isValid())
        return;

    if (!check())
        return;

    QFileInfo loaderInfo(StaticData::instance().loader);
    {
        QSettings coldSettings(loaderInfo.canonicalPath() + "/ColdClientLoader.ini", QSettings::IniFormat);
        coldSettings.setValue("SteamClient/Exe", index.data(GameExeRole));
        coldSettings.setValue("SteamClient/ExeRunDir", index.data(GameExeRunDirRole));
        coldSettings.setValue("SteamClient/ExeCommandLine", index.data(GameExeCommandLineRole));
        coldSettings.setValue("SteamClient/AppId", index.data(GameAppIdRole));

        if (coldSettings.value("SteamClient/SteamClientDll").isNull())
            coldSettings.setValue("SteamClient/SteamClientDll", "steamclient.dll");

        if (coldSettings.value("SteamClient/SteamClient64Dll").isNull())
            coldSettings.setValue("SteamClient/SteamClient64Dll", "steamclient64.dll");
    }

    QProcess::startDetached(loaderInfo.canonicalFilePath(), QStringList(), loaderInfo.canonicalPath());
}

void MainWindow::settings()
{
    SettingsDialog dialog(this);
    dialog.exec();
}

void MainWindow::about()
{
    AboutDialog dialog(this);
    dialog.exec();
}

void MainWindow::onCustomContextMenuRequested(const QPoint &pos)
{
    QPoint gPos = mGameView->mapToGlobal(pos);
    mPopupMenu->popup(gPos);
}

void MainWindow::onCurrentSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    (void) current;
    (void) previous;

    QModelIndex index = mGameView->currentIndex();
    mDeleteButton->setEnabled(index.isValid());
}
