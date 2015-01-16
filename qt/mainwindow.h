#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMouseEvent>
#include <QDebug>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

    int mouse_x;
    int mouse_y;

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private:
        Ui::MainWindow *ui;

    protected:
        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);

    public:
        void generate_colour(char *s);
};

#endif // MAINWINDOW_H
