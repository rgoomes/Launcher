#include <QTimer>
#include <QThread>
#include <QIcon>
#include <QToolButton>
#include <mutex>

#include "utils.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cleaner.h"

#if defined(__linux) || defined(__unix)
    #include <signal.h>
    #define SIGNALS
#endif

#define fmax(x, y) (((x) > (y)) ? (x) : (y))
#define fmin(x, y) (((x) < (y)) ? (x) : (y))

using namespace std;

#define MARGIN_SIZE 30
#define WAIT_TIME 100
#define GRIP_SIZE 5
#define PADDING 5

std::mutex mtx;

MainWindow::~MainWindow(){ delete ui; }
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    mtx.lock();
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

void MainWindow::setupCleaner(){
    QThread *thread = new QThread;
    Cleaner *cleaner = new Cleaner(&mtx, ctrl, cc);
    cleaner->moveToThread(thread);
    connect(thread, &QThread::started, cleaner, &Cleaner::cleanExit);
    thread->start();
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    // QUIT FOR TESTING, LATER CHANGE TO this->hide()
    if(event->key() == Qt::Key_Escape)
        mtx.unlock(); // FREE LOCK IN CLEANER

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
    QString win_gap = "0"; // TODO: CALCULATE OS THEME RESIZE MARGIN PX

    ctrl->set_option("x", QString::number(max(this->x(), 0) + toDpi(win_gap)));
    ctrl->set_option("y", QString::number(max(this->y(), 0) + toDpi(win_gap)));
}

void MainWindow::mousePressEvent(QMouseEvent* event){
    mpos = event->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event){
    if(!this->in_fullscreen() && !scaling)
        move(event->globalPos() - mpos);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event){
    QMainWindow::mouseReleaseEvent(event);

    if(!this->in_fullscreen())
        storeWindowPosition();
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
}

void MainWindow::setBorderVisibility(){
    std::string color = cc->getStyle("border", SBOX).toStdString();
    std::string alpha = color.substr(color.find_last_of(" ")+1);
    QString border = !alpha.compare("solid") ? "1px transparent" : "1px solid";

    cc->setStyle("border", border, SBOX);
    ui->sbox->setStyleSheet(cc->getStylesheet("Sbox", SBOX));
}

void MainWindow::setShadow(QColor c, int scale, int blur_radius, bool fullscreen_on){
    ShadowEffect* shadow = new ShadowEffect();

    shadow->setColor(c);
    shadow->setDistance(scale);
    shadow->setBlurRadius(blur_radius);
    ui->frame->setGraphicsEffect(shadow);

    if(fullscreen_on){
        ctrl->set_option("shadow-scale", QString::number(scale));
        ctrl->set_option("shadow-alpha", QString::number(c.alpha()));
        ctrl->set_option("shadow-blur-radius", QString::number(blur_radius));
    }
}

void MainWindow::setBorderRadius(int r, bool going_fullscreen){
    int old_radius = cc->getStyle("border-radius", FRAME).mid(0, 2).toInt();
    cc->setStyle("border-radius", QString::number(r) + "px", FRAME);
    ui->frame->setStyleSheet(cc->getStylesheet("Frame", FRAME));

    if(going_fullscreen)
        cc->setStyle("border-radius", QString::number(old_radius) + "px", FRAME);
}

bool MainWindow::in_fullscreen(){
    return (windowState() == Qt::WindowFullScreen)
         | (ctrl->get_option("fullscreen").toInt()); // SANITY TEST
}

void MainWindow::goFullScreenMode(){
    setBorderRadius(0, true);
    setShadow(QColor(0,0,0,0), 0, 0, false);
    ui->centralWidget->layout()->setContentsMargins(0, 0, 0, 0);
    ui->frameLayout->layout()->setContentsMargins(0, 15, 0, 0);
    ctrl->set_option("fullscreen", "1");

    this->showFullScreen();
}

void MainWindow::goWindowMode(){
    // RELOAD OLD USER STYLESHEET
    cc->reload(FRAME);
    ui->frame->setStyleSheet(cc->getStylesheet("Frame", FRAME));

    setShadow(QColor(0, 0, 0, ctrl->get_option("shadow-alpha").toInt()),
              ctrl->get_option("shadow-scale").toInt(),
              ctrl->get_option("shadow-blur-radius").toInt(), true);

    ui->centralWidget->layout()->setContentsMargins(5, 5, 5, 5);
    ui->frameLayout->layout()->setContentsMargins(0, 0, 0, 0);
    ctrl->set_option("fullscreen", "0");

    this->showNormal();

    // CHANGING DPI'S IN FULLSCREEN WILL NOT AFFECT MANY THINGS LIKE WINDOW SIZE
    // BECAUSE WE'RE IN FULLSCREEN, SO WHEN WE CHANGE THE WINDOW STATE TO WINDOW
    // MODE WE NEED TO RESTORE THOSE THINGS TO THE CORRECT VALUES, TO MATCH THE
    // CURRENT DPI'S. SOLUTION: CALL AGAIN CHANGE_DPI WHEN WE GO WINDOW MODE
    change_dpi(ctrl->get_option("dpi").toDouble(), false);
}

void MainWindow::center_window(){
    if(this->in_fullscreen())
        return;

    QRect screen = QApplication::desktop()->screenGeometry();
    this->move(screen.width()/2  - this->width()/2,
               screen.height()/2 - this->height()/2);

    storeWindowPosition();
}

void MainWindow::text_changed(QString text){
    QPixmap pixmap(text.compare("") ? "../qt/icons/clear.svg" : "../qt/icons/search.svg");
    icon->setIcon(QIcon(pixmap));

    qDebug() << text;
}

void MainWindow::change_dpi(double new_dpi, bool fullscreen_on){
    ctrl->set_option("dpi", QString::number(new_dpi));

    this->setSboxHeight(0);
    this->setFont(ctrl->get_option("font"), ctrl->get_option("font-size"));

    if(fullscreen_on)
        return;

    this->move(ctrl->get_option("x").toInt(), ctrl->get_option("y").toInt());
    this->resize(toDpi(ctrl->get_option("width")), toDpi(ctrl->get_option("height")));
    this->request_resize();
}

void MainWindow::setFont(QString font, QString size){
    ui->sbox->setFont(QFont(font, toDpi(size)));

    ctrl->set_option("font", font);
    ctrl->set_option("font-size", size);
}

void MainWindow::setFontColor(string color){
    cc->setStyle("color", QString::fromStdString(color), SBOX);
    ui->sbox->setStyleSheet(cc->getStylesheet("Sbox", SBOX));
}

void MainWindow::changeIconPos(bool keep){
    int on_left = ctrl->get_option("search-icon-pos").toInt();

    if(!keep){
        cc->setStyle("padding-left", on_left ? "0px" : "30px", SBOX);
        cc->setStyle("padding-right", on_left ? "30px" : "0px", SBOX);
        ctrl->set_option("search-icon-pos", QString::number(!on_left));
    }

    int icon_width = icon->iconSize().width();
    int width = (on_left ^ !keep) ? PADDING : ctrl->get_option("width").toInt() - MARGIN_SIZE-PADDING*2 - icon_width;
    icon->move(width, toDpi(ctrl->get_option("search-height"))/2 - icon_width/2);
    ui->sbox->setStyleSheet(cc->getStylesheet("Sbox", SBOX));
}

void MainWindow::setSboxHeight(double diff){
    double height = fmax(ctrl->get_option("search-height").toInt() + toPx(diff),
                         ctrl->get_option("font-size").toInt() + toPx(MARGIN_SIZE/2));

    height = fmin(height, -toPx(MARGIN_SIZE) + (!in_fullscreen() ? ctrl->get_option("height").toInt()
           : QApplication::desktop()->screenGeometry().height()));
    height = fmax(height, 0);

    ui->sbox->setMinimumHeight(toDpi(QString::number(height)));
    ctrl->set_option("search-height", QString::number(height));

    changeIconPos(true);
}

void MainWindow::selection_changed(){
    if(scaling){
        ui->sbox->deselect();
        ui->sbox->setCursorPosition(ui->sbox->text().length());
    }
}

void MainWindow::clear_trigged(){
    ui->sbox->setText("");
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event){
    QMainWindow::eventFilter(obj, event);
    QPoint cur = static_cast<const QMouseEvent*>(event)->pos();

    if(abs(toDpi(ctrl->get_option("search-height")) - cur.y()) <= GRIP_SIZE)
        QApplication::setOverrideCursor(Qt::SplitVCursor);
    else if(cur.y() && !scaling)
        QApplication::setOverrideCursor(Qt::IBeamCursor);
    if(event->type() == QEvent::MouseButtonRelease)
        if(abs(toDpi(ctrl->get_option("search-height")) - cur.y()) > GRIP_SIZE)
            QApplication::setOverrideCursor(Qt::IBeamCursor);
    if(event->type() == QEvent::Leave)
        QApplication::setOverrideCursor(Qt::ArrowCursor);

    /////////////////////////////////////////////////////
    /////////////////////////////////////////////////////

    if(scaling && event->type() == QEvent::MouseMove)
        setSboxHeight(cur.y() - mpos.y());
    if(event->type() == QEvent::MouseMove)
        mpos = cur;
    if(event->type() == QEvent::MouseButtonPress)
        if(abs(toDpi(ctrl->get_option("search-height")) - cur.y()) <= GRIP_SIZE)
            scaling = true;
    if(event->type() == QEvent::MouseButtonRelease)
        scaling = false;

    return false;
}

void MainWindow::signals_handler(){
    #ifdef SIGNALS
        std::vector<int> quit_signals = {SIGINT, SIGQUIT, SIGTERM};
    #else
        std::vector<int> quit_signals = std::vector<int>();
    #endif

    for(int sig : quit_signals)
        signal(sig, [](int ) { mtx.unlock(); });
}

void MainWindow::inits(){
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    // LINE EDIT DEFAULT ICON
    QPixmap pixmap("../qt/icons/search.svg");
    icon = new QToolButton(ui->sbox);
    icon->setIcon(QIcon(pixmap));
    icon->setIconSize(pixmap.size());

    // ALL CSS CLASS NAMES
    ui->frame->setObjectName("Frame");
    ui->sbox->setObjectName("Sbox");
    icon->setObjectName("Icon");

    // HASH TABLE FOR STYLESHEET, POPULATE USER STYLES
    cc = new Container("../User/stylesheet.user");
    ui->sbox->setStyleSheet(cc->getStylesheet("Sbox", SBOX));
    ui->frame->setStyleSheet(cc->getStylesheet("Frame", FRAME));
    icon->setStyleSheet(cc->getStylesheet("Icon", ICON));

    // WINDOW OPTIONS
    ctrl = new WindowController("../User/window.user");
    this->change_dpi(ctrl->get_option("dpi").toDouble(), false);
    this->setFontColor(cc->getStyle("color", SBOX).toUtf8().constData());

    // DRAW SHADOW
    setShadow(QColor(0, 0, 0, ctrl->get_option("shadow-alpha").toInt()),
              ctrl->get_option("shadow-scale").toInt(),
              ctrl->get_option("shadow-blur-radius").toInt(), false);

    // FULLSCREEN STATE
    if(ctrl->get_option("fullscreen").toInt())
        goFullScreenMode();

    // UNIX SIGNALS HANDLER AND CLEANER
    setupCleaner();
    signals_handler();

    // LINE EDIT EVENT FILTERS
    ui->sbox->installEventFilter(this);
    connect(icon, SIGNAL(clicked()), this, SLOT(clear_trigged()));
    connect(ui->sbox, SIGNAL(textChanged(QString )), this, SLOT(text_changed(QString )));
    connect(ui->sbox, SIGNAL(selectionChanged()), this, SLOT(selection_changed()));
}
