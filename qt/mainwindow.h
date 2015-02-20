#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QGraphicsBlurEffect>
#include <QMouseEvent>
#include <QDebug>
#include <QPoint>
#include <QGraphicsScene>

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
        void setBorderRadius(int r);
        void setShadow(QColor c, int offset, int blur_radius);
        void setFullScreenMode();
        void setWindowMode();

    private:
        Ui::MainWindow *ui;

    protected:
        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);

};

#endif // MAINWINDOW_H
