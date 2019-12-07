#include "aboutdialog.h"

#include <QLabel>
#include <QGridLayout>
#include <QTabWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QFile>

#include "define.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint)
{
    QTabWidget *tab = new QTabWidget;

    QWidget *aboutWidget = new QWidget;
    {
        QLabel *logoLabel = new QLabel;
        logoLabel->setPixmap(QPixmap(QString(":/images/logo.png")));

        QLabel *appNameLabel = new QLabel(QString("\n%1 v%2").arg("GoldbergGameManager").arg(IDD_VERSION));

        QFont titleFont;
        titleFont.setPointSize(11);
        titleFont.setWeight(75);
        appNameLabel->setFont(titleFont);

        QLabel *buildLabel = new QLabel(QString("Built on %1 at %2\n").arg(__DATE__, __TIME__));
        QLabel *authorLabel = new QLabel(QString("Author:\n%1\n").arg("renkinjutsu"));
        authorLabel->setAlignment(Qt::AlignCenter);
        QLabel *projectLabel = new QLabel(QString("Project:\n%1").arg("https://gitlab.com/ichdy/GoldbergGameManager"));
        projectLabel->setOpenExternalLinks(true);
        projectLabel->setAlignment(Qt::AlignCenter);

        QGridLayout *aboutLayout = new QGridLayout;
        aboutLayout->addWidget(logoLabel, 0, 0, 1, 1, Qt::AlignCenter);
        aboutLayout->addWidget(appNameLabel, 1, 0, 1, 1, Qt::AlignCenter);
        aboutLayout->addWidget(buildLabel, 2, 0, 1, 1, Qt::AlignCenter);
        aboutLayout->addWidget(authorLabel, 3, 0, 1, 1, Qt::AlignCenter);
        aboutLayout->addWidget(projectLabel, 4, 0, 1, 1, Qt::AlignCenter);

        aboutWidget->setLayout(aboutLayout);
    }

    tab->addTab(aboutWidget, "App");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(tab);

    setLayout(layout);
    setWindowTitle(QString("About"));
    resize(300, 250);
}
