#ifndef DEFINE_H
#define DEFINE_H

#include <qnamespace.h>

enum GameDataRole {
    GameUnknownRole = Qt::UserRole,
    GameExeRole,
    GameExeRunDirRole,
    GameExeCommandLineRole,
    GameAppIdRole,
    GameTitleRole,
    GameIconRole
};

#define IDD_VERSION         "0.1.0"

#endif // DEFINE_H
