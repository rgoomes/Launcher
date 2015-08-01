#include <QTimer>
#include <QThread>

#include "utils.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#define fmax(x, y) (((x) > (y)) ? (x) : (y))
#define fmin(x, y) (((x) < (y)) ? (x) : (y))

using namespace std;

#define MARGIN_SIZE 30
#define WAIT_TIME 100
#define GRIP_SIZE 5

MainWindow::~MainWindow(){ delete ui; }
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    inits();
    setupWorker();
}

void MainWindow::setupWorker(){
    QThread* thread = new QThread;
    worker = new Worker();
    worker->moveToThread(thread);
    connect(thread, &QThread::started, worker, &Worker::process );
    connect(worker, &Worker::finished, thread, &QThread::quit );
    connect(worker, &Worker::finished, worker, &Worker::deleteLater );
    connect(thread, &QThread::finished, thread, &QThread::deleteLater );
    thread->start();
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

    // SHOW SETTINGS
    if(event->key() == Qt::Key_F2){
        settingsWindow = new SettingsWindow();
        settingsWindow->show();
    }

    // AUTOCOMPLETE
    if(event->key() == Qt::Key_Tab){;}
}

int MainWindow::toDpi(QString px){ return int(px.toInt() * ctrl->get_option("dpi").toDouble()); }
int MainWindow::toPx(int px){  return int(px / ctrl->get_option("dpi").toDouble()); }

void MainWindow::storeWindowPosition(){
    QString win_gap = "0"; // TODO: CALCULATE WINDOW MANAGER RESIZE PX
    ctrl->set_option("x", QString::number(max(this->x(), 0) + toDpi(win_gap)));
    ctrl->set_option("y", QString::number(max(this->y(), 0) + toDpi(win_gap)));
    ctrl->update_file();
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
    this->storeWindowPosition();
}

void MainWindow::request_resize(){
    ctrl->set_option("width",  QString::number(toPx(this->width())));
    ctrl->set_option("height", QString::number(toPx(this->height())));
    this->storeWindowPosition();
    this->setSboxHeight(0);

    resizing = false;
}

void MainWindow::resizeEvent(QResizeEvent* event){
    QMainWindow::resizeEvent(event);

    if(!resizing && !this->in_fullscreen() && event->spontaneous()){
        resizing = true;
        QTimer::singleShot(WAIT_TIME, this, SLOT(request_resize()));
    }
}

double MainWindow::getBackgroundAlpha(){
    const char *color = cc->getStyle("background-color", FRAME).toStdString().c_str();

    double alpha;
    sscanf(color, "rgba(%*d,%*d,%*d,%lf)", &alpha);

    return alpha;
}

void MainWindow::setBackgroundColor(QColor color, bool random){
    QString c;

    if(random)
        c = rand_color(this->getBackgroundAlpha());
    else {
        c = "rgba("
          + QString::number(color.red())   + ","
          + QString::number(color.green()) + ","
          + QString::number(color.blue())  + ","
          + QString::number(color.alpha()) + ")";
    }

    cc->setStyle("background-color", c, FRAME);
    ui->frame->setStyleSheet(cc->getStylesheet("Frame", FRAME));
    cc->update_file();
}

void MainWindow::setBorderVisibility(){
    std::string color = cc->getStyle("border", SBOX).toStdString();
    std::string alpha = color.substr(color.find_last_of(" ")+1);
    QString border = !alpha.compare("solid") ? "1px transparent" : "1px solid";

    cc->setStyle("border", border, SBOX);
    ui->sbox->setStyleSheet(cc->getStylesheet("Sbox", SBOX));
    cc->update_file();
}

void MainWindow::setShadow(QColor c, int scale, int blur_radius, bool to_update){
    shadow->setColor(c);
    shadow->setDistance(scale);
    shadow->setBlurRadius(blur_radius);
    ui->frame->setGraphicsEffect(shadow);

    if(to_update){
        ctrl->set_option("shadow-scale", QString::number(scale));
        ctrl->set_option("shadow-alpha", QString::number(c.alpha()));
        ctrl->set_option("shadow-blur-radius", QString::number(blur_radius));
        ctrl->update_file();
    }
}

void MainWindow::setBorderRadius(int r, bool to_update){
    int old_radius = cc->getStyle("border-radius", FRAME).mid(0, 2).toInt();
    cc->setStyle("border-radius", QString::number(r) + "px", FRAME);
    ui->frame->setStyleSheet(cc->getStylesheet("Frame", FRAME));

    if(to_update)
        cc->update_file();
    else
        cc->setStyle("border-radius", QString::number(old_radius) + "px", FRAME);
}

bool MainWindow::in_fullscreen(){
    return (windowState() == Qt::WindowFullScreen)
         | (ctrl->get_option("fullscreen").toInt()); // SANITY TEST
}

void MainWindow::goFullScreenMode(){
    setBorderRadius(0, false);
    setShadow(QColor(0,0,0,0), 0, 0, false);
    ui->centralWidget->layout()->setContentsMargins(0, 0, 0, 0);
    ui->frameLayout->layout()->setContentsMargins(0, 15, 0, 0);
    ctrl->set_option("fullscreen", "1");
    ctrl->update_file();

    this->showFullScreen();
}

void MainWindow::goWindowMode(){
    // RELOAD OLD USER STYLESHEET
    cc->reload(FRAME);
    ui->frame->setStyleSheet(cc->getStylesheet("Frame", FRAME));

    setShadow(QColor(0, 0, 0, ctrl->get_option("shadow-alpha").toInt()),
              ctrl->get_option("shadow-scale").toInt(),
              ctrl->get_option("shadow-blur-radius").toInt(), false);

    ui->centralWidget->layout()->setContentsMargins(5, 5, 5, 5);
    ui->frameLayout->layout()->setContentsMargins(0, 0, 0, 0);
    ctrl->set_option("fullscreen", "0");
    ctrl->update_file();

    this->showNormal();
}

void MainWindow::center_window(){
    QDesktopWidget widget;
    QRect screen = widget.availableGeometry(widget.primaryScreen());
    this->move(screen.width()/2  - this->width()/2,
               screen.height()/2 - this->height()/2);

    this->storeWindowPosition();
}

void MainWindow::text_changed(QString text){
    qDebug() << text;
}

void MainWindow::change_dpi(double new_dpi){
    ctrl->set_option("dpi", QString::number(new_dpi));

    this->resize(toDpi(ctrl->get_option("width")), toDpi(ctrl->get_option("height")));
    this->request_resize();

    ui->sbox->setMinimumHeight(toDpi(ctrl->get_option("search-height")));
    this->setFont(ctrl->get_option("font"), ctrl->get_option("font-size"));
}

void MainWindow::setFont(QString font, QString size){
    ui->sbox->setFont(QFont(font, toDpi(size)));

    ctrl->set_option("font", font);
    ctrl->set_option("font-size", size);
    ctrl->update_file();
}

void MainWindow::setFontColor(string color){
    cc->setStyle("color", QString::fromStdString(color), SBOX);
    ui->sbox->setStyleSheet(cc->getStylesheet("Sbox", SBOX));
    cc->update_file();
}

void MainWindow::setSboxHeight(double diff){
    double height = fmax(ctrl->get_option("search-height").toInt() + diff,
                         ctrl->get_option("font-size").toInt() + MARGIN_SIZE);

    height = fmax(height, 0);
    height = fmin(height, ctrl->get_option("height").toInt() - MARGIN_SIZE);

    ui->sbox->setMinimumHeight(toDpi(QString::number(height)));
    ctrl->set_option("search-height", QString::number(height));
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event){
    QMainWindow::eventFilter(obj, event);
    QPoint cur = static_cast<const QMouseEvent*>(event)->pos();

    if(abs(ctrl->get_option("search-height").toInt() - cur.y()) <= GRIP_SIZE)
        QApplication::setOverrideCursor(Qt::SplitVCursor);
    else if(cur.y() && !scaling)
        QApplication::setOverrideCursor(Qt::IBeamCursor);
    if(event->type() == QEvent::MouseButtonRelease)
        if(abs(ctrl->get_option("search-height").toInt() - cur.y()) > GRIP_SIZE)
            QApplication::setOverrideCursor(Qt::IBeamCursor);
    if(event->type() == QEvent::Leave)
        QApplication::setOverrideCursor(Qt::ArrowCursor);

    /////////////////////////////////////////////////////
    /////////////////////////////////////////////////////

    if(scaling && event->type() == QEvent::MouseMove)
        setSboxHeight(abs(cur.y() - mpos.y()) * (cur.y() > mpos.y() ? 1 : -1));
    if(event->type() == QEvent::MouseMove)
        mpos = cur;
    if(event->type() == QEvent::MouseButtonPress)
        if(abs(ctrl->get_option("search-height").toInt() - cur.y()) <= GRIP_SIZE)
            scaling = true;
    if(event->type() == QEvent::MouseButtonRelease){
        ctrl->update_file();
        scaling = false;
    }

    return false;
}

void MainWindow::inits(){
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    // ALL CSS CLASS NAMES
    ui->frame->setObjectName("Frame");
    ui->sbox->setObjectName("Sbox");

    // HASH TABLE FOR STYLESHEET, POPULATE USER STYLES
    cc = new Container("../User/stylesheet.user");
    ui->sbox->setStyleSheet(cc->getStylesheet("Sbox", SBOX));
    ui->frame->setStyleSheet(cc->getStylesheet("Frame", FRAME));

    // WINDOW OPTIONS
    ctrl = new WindowController("../User/window.user");
    ui->sbox->setMinimumHeight(toDpi(ctrl->get_option("search-height")));
    this->move(ctrl->get_option("x").toInt(), ctrl->get_option("y").toInt());
    this->resize(toDpi(ctrl->get_option("width")), toDpi(ctrl->get_option("height")));
    this->setFont(ctrl->get_option("font"), ctrl->get_option("font-size"));
    this->setFontColor(cc->getStyle("color", SBOX).toUtf8().constData());

    // DRAW SHADOW
    shadow = new ShadowEffect();
    setShadow(QColor(0, 0, 0, ctrl->get_option("shadow-alpha").toInt()),
              ctrl->get_option("shadow-scale").toInt(),
              ctrl->get_option("shadow-blur-radius").toInt(), false);

    // STORED WINDOW STATE
    if(ctrl->get_option("fullscreen").toInt())
        goFullScreenMode();

    // LINE EDIT TEXT CHANGE
    connect(ui->sbox, SIGNAL(textChanged(QString )), this, SLOT(text_changed(QString )));

    // LIDE EDIT EVENT FILTER
    ui->sbox->installEventFilter(this);
}
