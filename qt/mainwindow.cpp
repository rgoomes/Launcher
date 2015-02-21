#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

#include <QPixmap>
#include <QGraphicsScene>
#include <QScreen>
#include <QGuiApplication>

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint);

    ui->setupUi(this);
    inits();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent* event){
    mouse_x = event->x();
    mouse_y = event->y();

    qDebug() << event->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event){
    move(event->globalX() - mouse_x,
         event->globalY() - mouse_y);
}

void MainWindow::setShadow(QColor c, int offset, int blur_radius){
    effect->setColor(c);
    effect->setOffset(offset);
    effect->setBlurRadius(blur_radius);

    ui->frame->setGraphicsEffect(effect);
}

void MainWindow::setBorderRadius(int r){
    string tmp = string("border-radius: ") + to_string(r) + string("px; background-color: rgba(10,196,149,0.45);");

    ui->frame->setStyleSheet(tmp.c_str());
}

void MainWindow::setFullScreenMode(){
    setBorderRadius(0);
    setShadow(QColor(0,0,0,0), 0, 0);
    ui->centralWidget->layout()->setContentsMargins(0, 0, 0, 0);
    this->setWindowState(Qt::WindowFullScreen);
}

void MainWindow::setWindowMode(){
    // LATER  CHANGE WINDOW  DEFAULT VALUES TO
    // USER DEFAULTS THAT ARE STORED IN A FILE

    setBorderRadius(15);
    setShadow(QColor(0,0,0,255), 3, 15);
    ui->centralWidget->layout()->setContentsMargins(5, 5, 5, 5);
    this->setWindowState(windowState() ^ Qt::WindowFullScreen);
}

void MainWindow::inits(){
    effect = new QGraphicsDropShadowEffect();

    // DEFINE A DEFAULT SHADOW
    setShadow(QColor(0,0,0,255), 3, 15);

    //ui->frame->setGraphicsEffect(new QGraphicsBlurEffect);
}
