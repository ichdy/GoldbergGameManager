QT       += core gui widgets

CONFIG += c++11

SOURCES += \
    src/aboutdialog.cpp \
    src/gamedialog.cpp \
    src/gameitemdelegate.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/settingsdialog.cpp

HEADERS += \
    src/aboutdialog.h \
    src/define.h \
    src/gamedialog.h \
    src/gameitemdelegate.h \
    src/mainwindow.h \
    src/settingsdialog.h \
    src/staticdata.h

RESOURCES += \
    GoldbergGameManager.qrc

RC_FILE = \
    GoldbergGameManager.rc