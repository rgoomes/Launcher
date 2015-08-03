#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QDesktopWidget>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QRect>
#include <QTimer>

#include "settingswindow.h"
#include "stylesheet.h"
#include "stylecontainer.hpp"
#include "shadoweffect.h"
#include "windowcontroller.h"
#include "worker.h"

#include <string>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

    ShadowEffect* shadow;
    int mouse_x, mouse_y;
    QPoint mpos;

    bool resizing = false;
    bool scaling  = false;

    const int FRAME = 0;
    const int SBOX  = 1;

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    public:
        void inits();
        void setBorderRadius(int , bool );
        void setShadow(QColor , int , int , bool);
        void setBackgroundColor(QColor, bool);
        void setSboxHeight(double );
        void setBorderVisibility();
        void goFullScreenMode();
        void goWindowMode();
        void center_window();
        void storeWindowPosition();
        void change_dpi(double );
        void setFontColor(std::string );
        void setFont(QString, QString);
        bool in_fullscreen();
        bool eventFilter(QObject *, QEvent *);
        double getBackgroundAlpha();
        int toDpi(QString );
        int toPx(int );

        void setupWorker();

    public slots:
        void request_resize();
        void selection_changed();
        void text_changed(QString );

    private:
        WindowController *ctrl;
        Ui::MainWindow *ui;
        SettingsWindow *settingsWindow;
        Container *cc;
        Worker* worker;

    protected:
        void mouseReleaseEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void resizeEvent(QResizeEvent* event);
        void keyPressEvent(QKeyEvent* event);

};

#endif // MAINWINDOW_H
