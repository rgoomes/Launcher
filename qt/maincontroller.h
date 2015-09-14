#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "mainwindow.h"
#include "windowcontroller.h"
#include "stylecontainer.hpp"
#include "shadoweffect.h"
#include "utils.h"

#include <QApplication>

class MainController{

    class MainWindow *w;
    WindowController *wc;
    Container *ct;

    public:
        MainController(MainWindow *, WindowController *, Container *);

    public:
        double curDpi();
        void setResizeMargin(int );
        void storeWindowPosition();
        void setSboxBorderColor(std::string );
        void setBackgroundColor(QColor , bool );
        void setSboxBorderWidth(int );
        void setShadow(QColor , int , int , bool );
        void setSboxBorderRadius(int );
        void setBorderRadius(int , bool );
        void updateIcon(QString , QString );
        void setFont(QString , QString);
        void setFontColor(std::string );
        void updateSboxHeight(int = 0);
        void setMaxResults(int );
        void setSearchTime(int );
        void setSearchType(QString );
        void setHideOnApp(int );
        void setHideIcon(int );
        void setGlobalShortcut(QString );
        void setSelectionColor(QColor );
        void changeIconPos(bool );
        void showLauncher();
        void hideLauncher();
        bool hideOnApp();
        bool getHideIcon();
        int toDpi(QString );
        int toPx(int );
        int getResizeMargin();
        int getBackgroundAlpha();
        int sboxBorderWidth();
        int shadowAlpha();
        int shadowScale();
        int shadowBlurRadius();
        int getSboxBorderRadius();
        int getBorderRadius();
        int getMaxResults();
        int getSearchTime();
        int iconOnLeft();
        int getSelectionAlpha();
        QString getBackgroundColor();
        QString getSboxBorderColor();
        QString getIconTheme();
        QString getSearchType();
        QString getGlobalShortcut();
        QString getSboxText();
        QString getSelectionColor();
        std::vector<QString> getFont();

};

#endif // MAINCONTROLLER_H
