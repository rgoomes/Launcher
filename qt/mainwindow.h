#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QRect>
#include <QToolButton>
#include <QThread>
#include <QIcon>
#include <QFontMetrics>
#include <QShortcut>

#include "settingswindow.h"
#include "stylesheet.h"
#include "stylecontainer.hpp"
#include "shadoweffect.h"
#include "windowcontroller.h"
#include "worker.h"
#include "resultwidget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

    const int FRAME = 0;
    const int SBOX  = 1;
    const int ICON  = 2;

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    public:
        void inits();
        void setBorderRadius(int , bool );
        void setShadow(QColor , int , int , bool);
        void setSboxBorderColor(std::string );
        void setSboxBorderRadius(int );
        void setBackgroundColor(QColor, bool);
        void updateSboxHeight(int );
        void goFullScreenMode();
        void goWindowMode();
        void goMode();
        void center_window();
        void storeWindowPosition();
        void change_dpi(double , bool);
        void setFontColor(std::string );
        void setFont(QString, QString);
        void changeIconPos(bool );
        void setSboxBorderWidth(int );
        void updateIcon(QString, QString );
        void setResizeMargin(int );
        void setSearchTime(int );
        void setSearchType(QString );
        void setHideOnApp(int );
        void setMaxResults(int );
        void setHideIcon(int );
        void setGlobalShortcut(QString );
        void request_resize();
        bool in_fullscreen();
        bool hideOnApp();
        bool getHideIcon();
        double curDpi();
        int getSboxBorderRadius();
        int getMaxResults();
        int getSearchTime();
        int getResizeMargin();
        int getBackgroundAlpha();
        int sboxBorderWidth();
        int shadowScale();
        int shadowBlurRadius();
        int getBorderRadius();
        int shadowAlpha();
        int iconOnLeft();
        int toDpi(QString );
        int toPx(int );
        void setupWorker();
        void setupCleaner();
        void setupGlobalShortcut();
        void signals_handler();
        void update();
        std::vector<QString> getFont();
        QString getGlobalShortcut();
        QString getSearchType();
        QString getBackgroundColor();
        QString getSboxBorderColor();
        QString getIconTheme();
        QString getSboxText();
        bool settingsOpened = false;

    public slots:
        void clear_trigged();
        void selection_changed();
        void text_changed(QString );
        void onFullscreenShortcut();

    private:
        class SettingsWindow *settingsWindow;
        class GlobalShortcut *globalshortcut;

        WindowController *ctrl;
        Ui::MainWindow *ui;
        Container *cc;
        Worker* worker;
        QToolButton *icon;
        QPoint mpos;

        bool resizing = false;
        bool scaling  = false;

    protected:
        bool eventFilter(QObject *, QEvent *);
        void mouseReleaseEvent(QMouseEvent *event);
        void mouseDoubleClickEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void resizeEvent(QResizeEvent* event);
        void keyPressEvent(QKeyEvent* event);

};

#endif // MAINWINDOW_H
