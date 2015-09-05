#include "utils.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cleaner.h"
#include "globalshortcut.h"

#define DARK_ICONS_PATH  QString("../qt/icons/dark/")
#define LIGHT_ICONS_PATH QString("../qt/icons/light/")

#if defined(__linux) || defined(__unix)
    #include <signal.h>
    #define SIGNALS
#endif

#define MARGIN_SIZE 30
#define WAIT_TIME 10
#define GRIP_SIZE 5
#define PADDING 5
#define LIMIT 128

QMutex sigLock;

MainWindow::~MainWindow(){ delete ui; }
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    sigLock.lock();
    inits();
    setupWorker();

    ui->results->layout()->addWidget(new ResultWidget(ui->results, "main.cpp"));
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
    Cleaner *cleaner = new Cleaner(this, &sigLock);
    cleaner->moveToThread(thread);
    connect(thread, &QThread::started, cleaner, &Cleaner::cleanExit);
    thread->start();
}

void MainWindow::setupGlobalShortcut(){
    QThread *thread = new QThread;
    globalshortcut = new GlobalShortcut(this, getGlobalShortcut());
    globalshortcut->moveToThread(thread);
    connect(thread, &QThread::started, globalshortcut, &GlobalShortcut::listenShortcuts);
    thread->start();
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    // CLEANER QUIT FOR TESTING, LATER CHANGE TO this->hide()
    if(event->key() == Qt::Key_Escape)
        sigLock.unlock(); // FREE LOCK IN CLEANER

    // SHOW SETTINGS
    if(!settingsOpened && event->key() == Qt::Key_F1){
        settingsWindow = new SettingsWindow(this);
        settingsWindow->setMainWindow(this);
        settingsWindow->show();
        settingsWindow->activateWindow();
        settingsOpened = true;
    }

    // AUTOCOMPLETE
    if(event->key() == Qt::Key_Tab){;}
}

double MainWindow::curDpi() { return ctrl->get_option("dpi").toDouble(); }
int MainWindow::toDpi(QString px){ return int(px.toInt() * curDpi()); }
int MainWindow::toPx(int px){  return int(px / curDpi()); }

int MainWindow::getResizeMargin(){
    return ctrl->get_option("resize-margin").toInt();
}

void MainWindow::setResizeMargin(int margin){
    ctrl->set_option("resize-margin", QString::number(margin));
}

void MainWindow::storeWindowPosition(){
    int win_gap = this->getResizeMargin();

    ctrl->set_option("x", QString::number(std::max(this->x(), 0) + win_gap));
    ctrl->set_option("y", QString::number(std::max(this->y(), 0) + win_gap));
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton)
        goMode();
}

void MainWindow::mousePressEvent(QMouseEvent* event){
    mpos = event->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event){
    if(!this->in_fullscreen() && !scaling && !resizing)
        move(event->globalPos() - mpos);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* ){
    if(!this->in_fullscreen())
        storeWindowPosition();
}

void MainWindow::request_resize(){
    ctrl->set_option("width",  QString::number(toPx(this->width())));
    ctrl->set_option("height", QString::number(toPx(this->height())));

    this->updateSboxHeight(0);
    if(resizing)
        this->storeWindowPosition();

    resizing = false;
}

void MainWindow::resizeEvent(QResizeEvent* event){
    if(!this->in_fullscreen() && event->spontaneous()){
        resizing = true;
        request_resize();
    }
}

QString MainWindow::getBackgroundColor(){
    return cc->getStyle("background-color", FRAME);
}

int MainWindow::getBackgroundAlpha(){
    int alpha;
    std::stringstream ss(cc->getStyle("background-color", FRAME).toStdString());

    ss.ignore(LIMIT, ',');
    ss.ignore(LIMIT, ',');
    ss.ignore(LIMIT, ',');
    ss >> alpha;

    return alpha;
}

QString MainWindow::getSboxBorderColor(){
    return cc->getStyle("border-color", SBOX);
}

void MainWindow::setSboxBorderColor(std::string color){
    cc->setStyle("border-color", QString::fromStdString(color), SBOX);
    ui->sbox->setStyleSheet(cc->getStylesheet("Sbox", SBOX));
}

void MainWindow::setBackgroundColor(QColor color, bool random){
    QString c = genColor(color, random, this->getBackgroundAlpha());

    cc->setStyle("background-color", c, FRAME);
    ui->frame->setStyleSheet(cc->getStylesheet("Frame", FRAME));
}

int MainWindow::sboxBorderWidth(){
    return cc->getStyle("border-width", SBOX).toInt();
}

void MainWindow::setSboxBorderWidth(int width){
    cc->setStyle("border-style", width ? "solid" : "transparent", SBOX);
    cc->setStyle("border-width", QString::number(width), SBOX);
    ui->sbox->setStyleSheet(cc->getStylesheet("Sbox", SBOX));
    changeIconPos(true);
}

int MainWindow::shadowAlpha(){
    return ctrl->get_option("shadow-alpha").toInt();
}

int MainWindow::shadowScale(){
    return ctrl->get_option("shadow-scale").toInt();
}

int MainWindow::shadowBlurRadius(){
    return ctrl->get_option("shadow-blur-radius").toInt();
}

void MainWindow::setShadow(QColor c, int scale, int blur_radius, bool going_fullscreen){
    if(!in_fullscreen()){
        ShadowEffect* shadow = new ShadowEffect();

        shadow->setColor(c);
        shadow->setDistance(scale);
        shadow->setBlurRadius(blur_radius);
        ui->frame->setGraphicsEffect(shadow);
    }

    if(!going_fullscreen){
        ctrl->set_option("shadow-scale", QString::number(scale));
        ctrl->set_option("shadow-alpha", QString::number(c.alpha()));
        ctrl->set_option("shadow-blur-radius", QString::number(blur_radius));
    }
}

int MainWindow::getSboxBorderRadius(){
    return cc->getStyle("border-radius", SBOX).toInt();
}

void MainWindow::setSboxBorderRadius(int r){
    cc->setStyle("border-radius", QString::number(r), SBOX);
    ui->sbox->setStyleSheet(cc->getStylesheet("Sbox", SBOX));
}

int MainWindow::getBorderRadius(){
    return cc->getStyle("border-radius", FRAME).toInt();
}

void MainWindow::setBorderRadius(int r, bool going_fullscreen){
    int old_radius = getBorderRadius();
    cc->setStyle("border-radius", QString::number(r), FRAME);

    if(!in_fullscreen())
        ui->frame->setStyleSheet(cc->getStylesheet("Frame", FRAME));
    if(going_fullscreen)
        cc->setStyle("border-radius", QString::number(old_radius), FRAME);
}

bool MainWindow::in_fullscreen(){
    return windowState() == Qt::WindowFullScreen;
}

void MainWindow::goFullScreenMode(){
    setBorderRadius(0, true);
    setShadow(QColor(0,0,0,0), 0, 0, true);
    if(settingsOpened) settingsWindow->updateBtnState();

    ui->centralWidget->layout()->setContentsMargins(0, 0, 0, 0);
    ui->frameLayout->layout()->setContentsMargins(0, 15, 0, 0);
    ctrl->set_option("fullscreen", "1");
    this->showFullScreen();

    changeIconPos(true);
}

void MainWindow::goWindowMode(){
    if(settingsOpened) settingsWindow->updateBtnState();

    ui->centralWidget->layout()->setContentsMargins(5, 5, 5, 5);
    ui->frameLayout->layout()->setContentsMargins(0, 0, 0, 0);
    ctrl->set_option("fullscreen", "0");
    this->showNormal();

    setBorderRadius(getBorderRadius(), false);
    setShadow(QColor(0,0,0, shadowAlpha()), shadowScale(), shadowBlurRadius(), false);

    // CHANGING DPI'S IN FULLSCREEN WILL NOT AFFECT MANY THINGS LIKE WINDOW SIZE
    // BECAUSE WE'RE IN FULLSCREEN, SO WHEN WE CHANGE THE WINDOW STATE TO WINDOW
    // MODE WE NEED TO RESTORE THOSE THINGS TO THE CORRECT VALUES, TO MATCH THE
    // CURRENT DPI'S. SOLUTION: CALL AGAIN CHANGE_DPI WHEN WE GO WINDOW MODE
    change_dpi(ctrl->get_option("dpi").toDouble(), false);
}

void MainWindow::goMode(){
    if(this->in_fullscreen())
        goWindowMode();
    else
        goFullScreenMode();
}

void MainWindow::center_window(){
    if(this->in_fullscreen())
        return;

    QRect screen = QApplication::desktop()->screenGeometry();
    this->move(screen.width()/2  - this->width()/2,
               screen.height()/2 - this->height()/2);

    storeWindowPosition();
}

QString MainWindow::getIconTheme(){
    return ctrl->get_option("icon-theme");
}

void MainWindow::updateIcon(QString text, QString theme){
    ctrl->set_option("icon-theme", theme);
    QPixmap pixmap((theme.compare("dark") ? LIGHT_ICONS_PATH : DARK_ICONS_PATH) +
                   (text.compare("") ? "clear.svg" : "search.svg"));
    icon->setIcon(QIcon(pixmap));
}

QString MainWindow::getSboxText(){
    return ui->sbox->text();
}

void MainWindow::text_changed(QString text){
    updateIcon(text, getIconTheme());

    qDebug() << text;

    if(!getSearchType().compare("standard"))
        worker->updateWork(text, this->getSearchTime() * 1000, this->getMaxResults());
}

void MainWindow::change_dpi(double new_dpi, bool fullscreen_on){
    ctrl->set_option("dpi", QString::number(new_dpi));

    this->updateSboxHeight(0);
    this->setFont(ctrl->get_option("font"), ctrl->get_option("font-size"));

    if(fullscreen_on)
        return;

    this->move(ctrl->get_option("x").toInt(), ctrl->get_option("y").toInt());
    this->resize(toDpi(ctrl->get_option("width")), toDpi(ctrl->get_option("height")));
    this->request_resize();
}

std::vector<QString> MainWindow::getFont(){
    return {ctrl->get_option("font"), ctrl->get_option("font-size"), cc->getStyle("color", SBOX)};
}

void MainWindow::setFont(QString font, QString size){
    ui->sbox->setFont(QFont(font, toDpi(size)));

    ctrl->set_option("font", font);
    ctrl->set_option("font-size", size);

    // SETTING FONT WITH HIGH SIZE MIGHT EXCEED SBOX HEIGHT SO WE NEED TO UPDATE IT.
    // ALSO WE NEED TO MAKE SURE SBOX ICON STAYS ALWAYS IN THE MIDDLE OF THE SBOX
    updateSboxHeight(0);
}

void MainWindow::setFontColor(std::string color){
    cc->setStyle("color", QString::fromStdString(color), SBOX);
    ui->sbox->setStyleSheet(cc->getStylesheet("Sbox", SBOX));
}

int MainWindow::iconOnLeft(){
    return ctrl->get_option("search-icon-pos").toInt();
}

void MainWindow::changeIconPos(bool keep){
    int on_left = iconOnLeft();

    if(!keep){
        cc->setStyle("padding-left", on_left ? "0px" : "30px", SBOX);
        cc->setStyle("padding-right", on_left ? "30px" : "0px", SBOX);
        ctrl->set_option("search-icon-pos", QString::number(!on_left));
    }

    int icon_width = icon->iconSize().width();
    int width = (on_left ^ !keep) ? PADDING
              : (in_fullscreen()  ? QApplication::desktop()->screenGeometry().width() - MARGIN_SIZE - icon_width
              :  toDpi(ctrl->get_option("width")) - MARGIN_SIZE-PADDING*2 - icon_width);

    icon->move(width + sboxBorderWidth() * ((on_left ^ !keep) ? 1 : -1),
               toDpi(ctrl->get_option("search-height"))/2 - icon_width/2);
    ui->sbox->setStyleSheet(cc->getStylesheet("Sbox", SBOX));
}

void MainWindow::updateSboxHeight(int diff){
    QFontMetrics fm(QFont(ctrl->get_option("font"), toDpi(ctrl->get_option("font-size"))));
    int margin_fix = 0; // TODO: FIND OUTSIDE TOP MARGIN. FIXES ICON NOT STAYING IN MIDDLE OF SBOX

    int height = std::max(ctrl->get_option("search-height").toInt() + toPx(diff), toPx(fm.height()) + margin_fix);
    height = std::min(height, -toPx(MARGIN_SIZE) + (!in_fullscreen() ? ctrl->get_option("height").toInt()
           : toPx(QApplication::desktop()->screenGeometry().height())));
    height = std::max(height, 0);

    ui->sbox->setMinimumHeight(toDpi(QString::number(height)));
    ctrl->set_option("search-height", QString::number(height));

    changeIconPos(true);
}

int MainWindow::getMaxResults(){
    return ctrl->get_option("max-results").toInt();
}

void MainWindow::setMaxResults(int results){
    ctrl->set_option("max-results", QString::number(results));
}

int MainWindow::getSearchTime(){
    return ctrl->get_option("search-time").toInt();
}

void MainWindow::setSearchTime(int time){
    ctrl->set_option("search-time", QString::number(time));
}

QString MainWindow::getSearchType(){
    return ctrl->get_option("search-type");
}

void MainWindow::setSearchType(QString type){
    ctrl->set_option("search-type", type);
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

void MainWindow::update(){
    ctrl->update_file();
    cc->update_file();
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
        updateSboxHeight(cur.y() - mpos.y());
    if(event->type() == QEvent::MouseMove)
        mpos = cur;
    if(event->type() == QEvent::MouseButtonPress)
        if(abs(toDpi(ctrl->get_option("search-height")) - cur.y()) <= GRIP_SIZE)
            scaling = true;
    if(event->type() == QEvent::MouseButtonRelease)
        scaling = false;

    return false;
}

bool MainWindow::hideOnApp(){
    return ctrl->get_option("hide-on-app").toInt();
}

void MainWindow::setHideOnApp(int state){
    ctrl->set_option("hide-on-app", QString::number(state));
}

bool MainWindow::getHideIcon(){
    return ctrl->get_option("icon-hidden").toInt();
}

void MainWindow::setHideIcon(int state){
    icon->setVisible(!state);
    ctrl->set_option("icon-hidden", QString::number(state));
}

QString MainWindow::getGlobalShortcut(){
    return ctrl->get_option("global-shortcut");
}

void MainWindow::setGlobalShortcut(QString shortcut){
    ctrl->set_option("global-shortcut", shortcut);
}

void MainWindow::signals_handler(){
    #ifdef SIGNALS
        std::vector<int> quit_signals = {SIGINT, SIGQUIT, SIGTERM, SIGHUP, SIGSEGV};
    #else
        std::vector<int> quit_signals = std::vector<int>();
    #endif

    for(int sig : quit_signals)
        signal(sig, [](int ) { sigLock.unlock(); });
}

void MainWindow::onFullscreenShortcut(){
    this->goMode();
}

void MainWindow::inits(){
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    // LINE EDIT DEFAULT ICON
    icon = new QToolButton(ui->sbox);

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

    // INIT STORED PATH
    QPixmap pixmap((getIconTheme().compare("dark") ? LIGHT_ICONS_PATH : DARK_ICONS_PATH) + "search.svg");
    icon->setIcon(QIcon(pixmap));
    icon->setIconSize(pixmap.size());
    icon->setVisible(!getHideIcon());

    this->change_dpi(ctrl->get_option("dpi").toDouble(), false);
    this->setFontColor(cc->getStyle("color", SBOX).toUtf8().constData());

    // DRAW SHADOW
    setShadow(QColor(0,0,0, shadowAlpha()), shadowScale(), shadowBlurRadius(), false);

    // FULLSCREEN STATE
    if(ctrl->get_option("fullscreen").toInt())
        goFullScreenMode();

    // UNIX SIGNALS HANDLER AND CLEANER
    setupCleaner();
    signals_handler();

    // SETUP FULLSCREEN AND GLOBAL SHORTCUTS
    setupGlobalShortcut();
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_F12), this);
    QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(onFullscreenShortcut()));

    // LINE EDIT EVENT FILTERS
    ui->sbox->installEventFilter(this);
    connect(icon, SIGNAL(clicked()), this, SLOT(clear_trigged()));
    connect(ui->sbox, SIGNAL(textChanged(QString )), this, SLOT(text_changed(QString )));
    connect(ui->sbox, SIGNAL(selectionChanged()), this, SLOT(selection_changed()));
}
