#include <QTimer>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stylesheet.h"
#include "shadoweffect.h"

#define RESIZE_TIMEINTERVAL 500

MainWindow::~MainWindow(){ delete ui; }
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    inits();
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    // QUIT FOR TESTING, LATER CHANGE TO this->hide()
    if(event->key() == Qt::Key_Escape)
        QApplication::quit();

    // FULLSCREEN TEST KEY F1
    if(event->key() == Qt::Key_F1){
        if(this->in_fullscreen())
            goWindowMode();
        else
            goFullScreenMode();
    }
}

void MainWindow::storeWindowPosition(int win_gap){
    double dpi = 1.0; // TODO FIX

    controller->set_option("x", QString::number(this->x() + int(win_gap * dpi)), false);
    controller->set_option("y", QString::number(this->y() + int(win_gap * dpi)), true);
}

void MainWindow::mousePressEvent(QMouseEvent* event){
    mouse_x = event->x();
    mouse_y = event->y();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event){
    move(event->globalX() - mouse_x,
         event->globalY() - mouse_y);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event){
    QMainWindow::mouseReleaseEvent(event);
    this->storeWindowPosition(10);
}

void MainWindow::request_resize(){
    controller->set_option("width", QString::number(this->width()), false);
    controller->set_option("height", QString::number(this->height()), false);
    this->storeWindowPosition(10);

    resizing = false;
}

void MainWindow::resizeEvent(QResizeEvent* event){
    QMainWindow::resizeEvent(event);

    if(!resizing && !this->in_fullscreen() && event->spontaneous()){
        resizing = true;
        QTimer::singleShot(RESIZE_TIMEINTERVAL, this, SLOT(request_resize()));
    }
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

bool MainWindow::in_fullscreen(){
    return (windowState() == Qt::WindowFullScreen)
         | (controller->get_option("fullscreen").toInt()); // SANITY TEST
}

void MainWindow::goFullScreenMode(){
    setBorderRadius(0, false);
    setShadow(QColor(0,0,0,0), 0, 0);
    ui->centralWidget->layout()->setContentsMargins(0, 0, 0, 0);
    controller->set_option("fullscreen", "1", true);

    this->showFullScreen();
}

void MainWindow::goWindowMode(){
    // RELOAD OLD USER STYLESHEET
    ss->load_user_preferences();
    ui->frame->setStyleSheet(ss->get_stylesheet());

    setShadow(QColor(0,0,0,controller->get_option("shadow_alpha").toInt()), 3,
              controller->get_option("shadow_blur_radius").toInt());
    ui->centralWidget->layout()->setContentsMargins(5, 5, 5, 5);
    controller->set_option("fullscreen", "0", true);

    this->showNormal();
}

void MainWindow::center_window(){
    QDesktopWidget widget;
    QRect screen = widget.availableGeometry(widget.primaryScreen());
    this->move(screen.width()/2  - this->width()/2,
               screen.height()/2 - this->height()/2);

    this->storeWindowPosition(10);
}

void MainWindow::inits(){
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    // WINDOW OPTIONS
    controller = new WindowController();
    this->move(controller->get_option("x").toInt(), controller->get_option("y").toInt());
    this->resize(controller->get_option("width").toInt(), controller->get_option("height").toInt());

    // DRAW SHADOW
    shadow = new ShadowEffect();
    setShadow(QColor(0,0,0,controller->get_option("shadow_alpha").toInt()), 3,
              controller->get_option("shadow_blur_radius").toInt());
    layout()->addWidget(ui->lineEdit);

    // HASH TABLE FOR STYLESHEET, POPULATE USER STYLES
    ss = new Style();
    ui->frame->setStyleSheet(ss->get_stylesheet());

    // STORED WINDOW STATE
    if(controller->get_option("fullscreen").toInt())
        goFullScreenMode();
}
