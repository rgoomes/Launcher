#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stylesheet.h"
#include "shadoweffect.h"

MainWindow::~MainWindow(){ delete ui; }
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    inits();
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
    ShadowEffect *shadow_body = new ShadowEffect();

    shadow_body->setColor(c);
    shadow_body->setDistance(scale);
    shadow_body->setBlurRadius(blur_radius);

    ui->frame->setGraphicsEffect(shadow_body);
}

void MainWindow::setBorderRadius(int r){
    ss->update_style("border-radius", QString::number(r) + "px;", true);
    ui->frame->setStyleSheet(ss->get_stylesheet());
}

void MainWindow::goFullScreenMode(){
    setBorderRadius(0);
    setShadow(QColor(0,0,0,0), 0, 0);
    ui->centralWidget->layout()->setContentsMargins(0, 0, 0, 0);
    this->setWindowState(Qt::WindowFullScreen);
}

void MainWindow::goWindowMode(){
    // LATER  CHANGE WINDOW  DEFAULT VALUES TO
    // USER DEFAULTS THAT ARE STORED IN A FILE

    setBorderRadius(15);
    setShadow(QColor(0,0,0,255), 3, 15);
    ui->centralWidget->layout()->setContentsMargins(5, 5, 5, 5);
    this->setWindowState(windowState() ^ Qt::WindowFullScreen);
}

void MainWindow::inits(){
    // LOCATE/MLOCATE → time locate file -e -l 10 -q

    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint);

    shadow = new QGraphicsDropShadowEffect();

    // DEFINE A DEFAULT SHADOW
    setShadow(QColor(0,0,0,255), 3, 15);

    //ui->frame->setGraphicsEffect(new QGraphicsBlurEffect);

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
