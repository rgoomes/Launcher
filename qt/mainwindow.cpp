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

    // AUTOCOMPLETE
    if(event->key() == Qt::Key_Tab){;}
}

void MainWindow::storeWindowPosition(int win_gap){
    double dpi = controller->get_option("dpi").toDouble();

    controller->set_option("x", QString::number(this->x() + int(win_gap * dpi)));
    controller->set_option("y", QString::number(this->y() + int(win_gap * dpi)));
    controller->update_file();
}

void MainWindow::mousePressEvent(QMouseEvent* event){
    mouse_x = event->x();
    mouse_y = event->y();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event){
    if(!this->in_fullscreen())
        move(event->globalX() - mouse_x, event->globalY() - mouse_y);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event){
    QMainWindow::mouseReleaseEvent(event);
    this->storeWindowPosition(10);
}

void MainWindow::request_resize(){
    double dpi = controller->get_option("dpi").toDouble();

    controller->set_option("width",  QString::number(int(this->width()  / dpi)));
    controller->set_option("height", QString::number(int(this->height() / dpi)));
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

void MainWindow::setBorderRadius(int r){
    ss->set_style("border-radius", QString::number(r) + "px");
    ui->frame->setStyleSheet(ss->stylesheet("Frame"));
}

bool MainWindow::in_fullscreen(){
    return (windowState() == Qt::WindowFullScreen)
         | (controller->get_option("fullscreen").toInt()); // SANITY TEST
}

void MainWindow::goFullScreenMode(){
    setBorderRadius(0);
    setShadow(QColor(0,0,0,0), 0, 0);
    ui->centralWidget->layout()->setContentsMargins(0, 0, 0, 0);
    controller->set_option("fullscreen", "1");
    controller->update_file();

    this->showFullScreen();
}

void MainWindow::goWindowMode(){
    // RELOAD OLD USER STYLESHEET
    ss->load_user_preferences();
    ui->frame->setStyleSheet(ss->stylesheet("Frame"));

    setShadow(QColor(0,0,0,controller->get_option("shadow_alpha").toInt()), 3,
              controller->get_option("shadow_blur_radius").toInt());
    ui->centralWidget->layout()->setContentsMargins(5, 5, 5, 5);
    controller->set_option("fullscreen", "0");
    controller->update_file();

    this->showNormal();
}

void MainWindow::center_window(){
    QDesktopWidget widget;
    QRect screen = widget.availableGeometry(widget.primaryScreen());
    this->move(screen.width()/2  - this->width()/2,
               screen.height()/2 - this->height()/2);

    this->storeWindowPosition(10);
}

void MainWindow::text_changed(QString text){
    qDebug() << text;
}

void MainWindow::change_dpi(double new_dpi){
    controller->set_option("dpi", QString::number(new_dpi));

    this->resize(int(controller->get_option("width").toInt()  * new_dpi),
                 int(controller->get_option("height").toInt() * new_dpi));
    this->request_resize();

    ui->searchBox->setMinimumHeight(int(controller->get_option("search_height").toInt() * new_dpi));
    ui->searchBox->setFont(QFont(controller->get_option("font"), int(controller->get_option("font_size").toInt() * new_dpi)));
}

void MainWindow::inits(){
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    // FRAME CSS CLASS
    ui->frame->setObjectName("Frame");

    // WINDOW OPTIONS
    controller = new WindowController("../User/window.user");
    double dpi = controller->get_option("dpi").toDouble();
    this->move(controller->get_option("x").toInt(), controller->get_option("y").toInt());
    this->resize(int(controller->get_option("width").toInt() * dpi), int(controller->get_option("height").toInt() * dpi));
    ui->searchBox->setMinimumHeight(int(controller->get_option("search_height").toInt() * dpi));
    ui->searchBox->setFont(QFont(controller->get_option("font"), controller->get_option("font_size").toInt()));

    // DRAW SHADOW
    shadow = new ShadowEffect();
    setShadow(QColor(0,0,0,controller->get_option("shadow_alpha").toInt()), 3,
              controller->get_option("shadow_blur_radius").toInt());

    // HASH TABLE FOR STYLESHEET, POPULATE USER STYLES
    ss = new Style("../User/stylesheet.user");
    ui->frame->setStyleSheet(ss->stylesheet("Frame"));

    // STORED WINDOW STATE
    if(controller->get_option("fullscreen").toInt())
        goFullScreenMode();

    // LINE EDIT TEXT CHANGE
    connect(ui->searchBox, SIGNAL(textChanged(QString )), this, SLOT(text_changed(QString )));
}
