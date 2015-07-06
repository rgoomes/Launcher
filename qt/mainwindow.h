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
#include "shadoweffect.h"
#include "windowcontroller.h"
#include "worker.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

    ShadowEffect* shadow;
    int mouse_x, mouse_y;
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
        void change_dpi(double );
        bool in_fullscreen();

        void setupWorker();

    public slots:
        void request_resize();
        void text_changed(QString );

    private:
        WindowController *controller;
        Ui::MainWindow *ui;
        SettingsWindow *settingsWindow;
        Style *ss;
        Worker* worker;

    protected:
        void mouseReleaseEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void resizeEvent(QResizeEvent* event);
        void keyPressEvent(QKeyEvent* event);

};

#endif // MAINWINDOW_H
