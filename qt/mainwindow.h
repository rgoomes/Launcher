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

#include <string>

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
        void storeWindowPosition();
        void change_dpi(double );
        void setFontColor(std::string );
        void setFont(QString, QString);
        bool in_fullscreen();
        int toDpi(QString );
        int toPx(int );

        void setupWorker();

    public slots:
        void request_resize();
        void text_changed(QString );

    private:
        WindowController *ctrl;
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
