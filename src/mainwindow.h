#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>

class QListView;
class QToolButton;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void start();

private slots:
    bool check();
    void addGame();
    void deleteGame();
    void runGame();
    void gameProperties();
    void settings();
    void about();

    void onCustomContextMenuRequested(const QPoint &pos);
    void onCurrentSelectionChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    void appendGame(const QVariantMap &game);

private:
    void load();
    void save();

    QListView *mGameView;
    QToolButton *mAddButton;
    QToolButton *mDeleteButton;
    QToolButton *mSettingsButton;
    QToolButton *mAboutButton;

    QMenu *mPopupMenu;
    QAction *mRunAction;
    QAction *mPropertiesAction;
};

#endif // WIDGET_H
