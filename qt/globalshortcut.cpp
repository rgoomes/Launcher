#include "globalshortcut.h"

GlobalShortcut::~GlobalShortcut(){}
GlobalShortcut::GlobalShortcut(MainWindow *w, QString shortcut){ this->w = w; this->shortcut = shortcut; }

void showWindow(MainWindow *w){
    if(!w->isVisible())
        w->show();
}

int getModifiers(QString shortcut){
    #if defined(__linux) || defined(__unix)
        QStringList tokens = shortcut.toLower().simplified().replace(" ", "").split(QRegExp("[+]"));

        int modifiers = 0;
        for(QString t : tokens){
            if(!t.compare("ctrl"))
                modifiers |= ControlMask;
            else if(!t.compare("shift"))
                modifiers |= ShiftMask;
            else if(!t.compare("alt"))
                modifiers |= Mod1Mask;
            else if(!t.compare("super"))
                modifiers |= Mod4Mask;
        }

        return modifiers;
    #else
        return 0;
    #endif
}

int getKeyCode(Display *display, QString shortcut){
    #if defined(__linux) || defined(__unix)
        QStringList tokens = shortcut.simplified().replace(" ", "").split(QRegExp("[+]"));
        return XKeysymToKeycode(display, XStringToKeysym((*--tokens.end()).toStdString().c_str()));
    #else
        return 0;
    #endif
}

void GlobalShortcut::globalShortcutX11(){

#if defined(__linux) || defined(__unix)
    this->dpy     = XOpenDisplay(0);
    Window root   = DefaultRootWindow(dpy);
    XEvent event;

    int keycode   = getKeyCode(dpy, shortcut);
    int modifiers = getModifiers(shortcut);

    XGrabKey(dpy, keycode, modifiers, root, False, GrabModeAsync, GrabModeAsync);
    XSelectInput(dpy, root, KeyPressMask);

    while(true){
        XNextEvent(dpy, &event);
        if(event.type == KeyPress)
            showWindow(w);
    }
#endif

}

void GlobalShortcut::listenShortcuts(){

#if defined(__linux) || defined(__unix)
    globalShortcutX11();
#endif

}
