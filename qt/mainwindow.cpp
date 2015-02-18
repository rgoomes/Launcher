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
    char rad[8];
    sprintf(rad, "%d", r);
    string tmp = string("border-radius: ") + rad + string("px; background-color: rgba(10,196,149,0.5);");

    ui->frame->setStyleSheet(tmp.c_str());
}

void MainWindow::inits(){
    effect = new QGraphicsDropShadowEffect();

    // DEFINE A DEFAULT SHADOW
    setShadow(QColor(0,0,0,255), 3, 10);

    //ui->frame->setGraphicsEffect(new QGraphicsBlurEffect);
}
