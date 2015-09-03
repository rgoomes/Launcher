#ifndef GLOBALSHORTCUT_H
#define GLOBALSHORTCUT_H

#include <QObject>

#include "mainwindow.h"

#if defined(__linux) || defined(__unix)
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
#endif

class GlobalShortcut: public QObject {
    Q_OBJECT

    class MainWindow *w;
    QString shortcut;

#if defined(__linux) || defined(__unix)
    Display *dpy;
#endif

    public:
        GlobalShortcut(MainWindow *, QString );
        ~GlobalShortcut();

    public:
        void clean();

    public slots:
        void listenShortcuts();

    private:
        void globalShortcutX11();
};

#endif // GLOBALSHORTCUT_H
