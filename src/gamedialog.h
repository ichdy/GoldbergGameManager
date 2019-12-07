#ifndef GAMEDIALOG_H
#define GAMEDIALOG_H

#include <QDialog>

class QLineEdit;
class GameDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GameDialog(QWidget *parent = nullptr);

    QString Title();
    QString Exe();
    QString ExeRunDir();
    QString ExeCommandLine();
    int AppId();

    void setTitle(const QString &title);
    void setExe(const QString &filename);
    void setExeRunDir(const QString &dirpath);
    void setExeCommandLine(const QString &args);
    void setAppId(int id);

private:
    QLineEdit *mTitle;
    QLineEdit *mExe;
    QLineEdit *mExeRunDir;
    QLineEdit *mExeCommandLine;
    QLineEdit *mAppId;
};

#endif // GAMEDIALOG_H
