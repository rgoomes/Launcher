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

#include "stylesheet.h"
#include "shadoweffect.h"
#include "windowcontroller.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

    int mouse_x;
    int mouse_y;

    ShadowEffect* shadow;

    bool resizing = false;

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    public:
        void inits();
        void setBorderRadius(int );
        void setShadow(QColor , int , int );
        void goFullScreenMode();
        void goWindowMode();
        void center_window();
        void storeWindowPosition(int );
        bool in_fullscreen();

    public slots:
        void request_resize();
        void text_changed(QString );

    private:
        WindowController *controller;
        Ui::MainWindow *ui;
        Style *ss;

    protected:
        void mouseReleaseEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void resizeEvent(QResizeEvent* event);
        void keyPressEvent(QKeyEvent* event);

};

#endif // MAINWINDOW_H
