#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){

    // USE THIS LATER
    //setAttribute(Qt::WA_TranslucentBackground, true);

    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("*{background-color: #029777;}");
    setWindowOpacity(0.5);

    ui->setupUi(this);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent* event){
    mouse_x = event->x();
    mouse_y = event->y();


    char c[8], tmp[100] = "";
    generate_colour(c);

    strcpy(tmp,"*{background-color: #");
    strcat(tmp, c);
    strcat(tmp, ";}\0");

    setStyleSheet(tmp);

    qDebug() << event->pos() << c;

}

void MainWindow::mouseMoveEvent(QMouseEvent* event){
    move(event->globalX() - mouse_x,
         event->globalY() - mouse_y);
}

void MainWindow::generate_colour(char *s){
    char hex[] = "0123456789ABCDEF";

    for(int i = 0; i < 6; i++)
        *s++ = hex[rand() % 16];

    *s = '\0';
}
