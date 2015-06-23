#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stylesheet.h"
#include "shadoweffect.h"

MainWindow::~MainWindow(){ delete ui; }
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    inits();
}

bool fullscreen = false;
void MainWindow::keyPressEvent(QKeyEvent *event){
    // QUIT FOR TESTING, LATER CHANGE TO this->hide()
    if(event->key() == Qt::Key_Escape)
        QApplication::quit();

    // FULLSCREEN TEST KEY F1
    if(event->key() == Qt::Key_F1){
        if(fullscreen)
            goWindowMode();
        else
            goFullScreenMode();

        fullscreen = !fullscreen;
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event){
    mouse_x = event->x();
    mouse_y = event->y();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event){
    move(event->globalX() - mouse_x,
         event->globalY() - mouse_y);
}

void MainWindow::resizeEvent(QResizeEvent* event){
    QMainWindow::resizeEvent(event);

    // TESTING
}

void MainWindow::setShadow(QColor c, int scale, int blur_radius){
    shadow->setColor(c);
    shadow->setDistance(scale);
    shadow->setBlurRadius(blur_radius);

    ui->frame->setGraphicsEffect(shadow);
}

void MainWindow::setBorderRadius(int r, bool to_update){
    ss->update_style("border-radius", QString::number(r) + "px", to_update);
    ui->frame->setStyleSheet(ss->get_stylesheet());
}

void MainWindow::goFullScreenMode(){
    setBorderRadius(0, false);
    setShadow(QColor(0,0,0,0), 0, 0);
    ui->centralWidget->layout()->setContentsMargins(0, 0, 0, 0);
    this->showFullScreen();
}

void MainWindow::goWindowMode(){
    // LATER  CHANGE WINDOW  DEFAULT VALUES TO
    // USER DEFAULTS THAT ARE STORED IN A FILE
    setShadow(QColor(0,0,0,255), 3, 15);
    ui->centralWidget->layout()->setContentsMargins(5, 5, 5, 5);
    this->showNormal();
}

void MainWindow::inits(){
    // LOCATE/MLOCATE → time locate file -e -l 10 -q

    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint);

    // DEFINE A DEFAULT SHADOW
    shadow = new ShadowEffect();
    setShadow(QColor(0,0,0,255), 3, 15);

    // CENTER WINDOW
    QDesktopWidget widget;
    QRect screen = widget.availableGeometry(widget.primaryScreen());
    this->move(screen.width()/2  - this->width()/2,
               screen.height()/2 - this->height()/2);

    // HASH TABLE FOR STYLESHEET
    ss = new Style();

    // POPULATE USER STYLES
    ui->frame->setStyleSheet(ss->get_stylesheet());
}
