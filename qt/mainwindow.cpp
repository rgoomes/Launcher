#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

#include <QPixmap>
#include <QGraphicsScene>
#include <QScreen>
#include <QGuiApplication>
#include <QDesktopWidget>

#include "stylesheet.h"

using namespace std;

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

void MainWindow::setShadow(QColor c, int offset, int blur_radius){
    effect->setColor(c);
    effect->setOffset(offset);
    effect->setBlurRadius(blur_radius);

    ui->frame->setGraphicsEffect(effect);
}

void MainWindow::setBorderRadius(int r){
    ss->update_style("border-radius", QString::number(r) + "px;");
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

    effect = new QGraphicsDropShadowEffect();

    // DEFINE A DEFAULT SHADOW
    setShadow(QColor(0,0,0,255), 3, 15);

    //ui->frame->setGraphicsEffect(new QGraphicsBlurEffect);

    // CENTER WINDOW
    QDesktopWidget widget;
    QRect screen = widget.availableGeometry(widget.primaryScreen());
    this->move(screen.width()/2  - this->width()/2,
               screen.height()/2 - this->height()/2);

    // POPULATE STYLES
    ss = new Style();
    QStringList style_list = ui->frame->styleSheet().split(QRegExp("[\n]"),QString::SkipEmptyParts);
    for(auto s : style_list){
        QStringList s_str = s.split(QRegExp("[:]"), QString::SkipEmptyParts);
        ss->update_style(*s_str.begin(), *++s_str.begin());
    }
}
