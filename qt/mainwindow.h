#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QGraphicsBlurEffect>
#include <QMouseEvent>
#include <QDebug>
#include <QPoint>
#include <QRect>
#include <QGraphicsScene>
#include <QResizeEvent>

#include "stylesheet.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

    int mouse_x;
    int mouse_y;

    QGraphicsDropShadowEffect* effect;

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    public:
        void inits();
        void setBorderRadius(int );
        void setShadow(QColor , int , int );
        void goFullScreenMode();
        void goWindowMode();

    private:
        Ui::MainWindow *ui;
        Style *ss;

    protected:
        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void resizeEvent(QResizeEvent* event);

};

#endif // MAINWINDOW_H
