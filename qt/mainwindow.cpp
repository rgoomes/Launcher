#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cleaner.h"
#include "globalshortcut.h"
#include "types.h"

#define DARK_ICONS_PATH  QString("../qt/icons/dark/")
#define LIGHT_ICONS_PATH QString("../qt/icons/light/")

#if defined(__linux) || defined(__unix)
    #include <signal.h>
    #define SIGNALS
#endif

#define GRIP_SIZE 5
#define KEYPRESS  6

const int FRAME = 0;
const int SBOX  = 1;
const int ICON  = 2;
QMutex sigLock;

MainWindow::~MainWindow(){ delete ui; }
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    sigLock.lock();
    inits();
    setupWorker();
}

void MainWindow::setupWorker(){
    QThread* thread = new QThread;
    worker = new Worker();
    worker->moveToThread(thread);
    connect(thread, &QThread::started, worker, &Worker::process );
    connect(worker, &Worker::newResult, rc, &ResultsController::addResult);
    connect(worker, &Worker::cleanResults, rc, &ResultsController::clearResults);
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
    connect(cleaner, &Cleaner::finished, cleaner, &Cleaner::deleteLater );
    connect(thread, &QThread::finished, thread, &QThread::deleteLater );
    thread->start();
}

void MainWindow::setupGlobalShortcut(){
    QThread *thread = new QThread;
    globalshortcut = new GlobalShortcut(this, mc->getGlobalShortcut());
    globalshortcut->moveToThread(thread);
    connect(thread, &QThread::started, globalshortcut, &GlobalShortcut::listenShortcuts);
    connect(globalshortcut, &GlobalShortcut::finished, globalshortcut, &GlobalShortcut::deleteLater );
    connect(thread, &QThread::finished, thread, &QThread::deleteLater );
    thread->start();
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    // TODO: CLEANER QUIT FOR TESTING, LATER CHANGE TO this->hide()
    if(event->key() == Qt::Key_Escape)
        sigLock.unlock(); // FREE LOCK IN CLEANER

    if(!settingsOpened && event->key() == Qt::Key_F1){
        settingsWindow = new SettingsWindow(this);
        settingsWindow->show();
        settingsWindow->activateWindow();
        settingsOpened = true;
    }

    // TODO: AUTOCOMPLETE
    if(event->key() == Qt::Key_Tab){;}
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
        mc->storeWindowPosition();
}

void MainWindow::request_resize(){
    wc->set_option("width",  QString::number(mc->toPx(this->width())));
    wc->set_option("height", QString::number(mc->toPx(this->height())));

    mc->updateSboxHeight();
    if(resizing)
        mc->storeWindowPosition();

    resizing = false;
}

void MainWindow::resizeEvent(QResizeEvent* event){
    if(!this->in_fullscreen() && event->spontaneous()){
        resizing = true;
        request_resize();
        mc->canRender = true;
    }
}

bool MainWindow::in_fullscreen(){
    return windowState() == Qt::WindowFullScreen;
}

void MainWindow::goFullScreenMode(){
    mc->setBorderRadius(0, true);
    mc->setShadow(QColor(0,0,0,0), 0, 0, true);
    if(settingsOpened) settingsWindow->updateBtnState();

    ui->centralWidget->layout()->setContentsMargins(0, 0, 0, 0);
    ui->frameLayout->layout()->setContentsMargins(0, 15, 0, 0);
    wc->set_option("fullscreen", "1");
    this->showFullScreen();

    mc->changeIconPos(true);

    // TODO: PREVIEW AREA SIZE STILL NOT UPDATED HERE. WHY? FIX..
    mc->canRender = true;
    ui->previewArea->repaint();
}

void MainWindow::goWindowMode(){
    if(settingsOpened) settingsWindow->updateBtnState();

    ui->centralWidget->layout()->setContentsMargins(5, 5, 5, 5);
    ui->frameLayout->layout()->setContentsMargins(0, 0, 0, 0);
    wc->set_option("fullscreen", "0");
    this->showNormal();

    mc->setBorderRadius(mc->getBorderRadius(), false);
    mc->setShadow(QColor(0,0,0, mc->shadowAlpha()), mc->shadowScale(), mc->shadowBlurRadius(), false);

    // CHANGING DPI'S IN FULLSCREEN WILL NOT AFFECT MANY THINGS LIKE WINDOW SIZE
    // BECAUSE WE'RE IN FULLSCREEN, SO WHEN WE CHANGE THE WINDOW STATE TO WINDOW
    // MODE WE NEED TO RESTORE THOSE THINGS TO THE CORRECT VALUES, TO MATCH THE
    // CURRENT DPI'S. SOLUTION: CALL AGAIN CHANGE_DPI WHEN WE GO WINDOW MODE
    change_dpi(wc->get_option("dpi").toDouble(), false);
}

void MainWindow::goMode(){
    if(this->in_fullscreen())
        goWindowMode();
    else
        goFullScreenMode();
}

void MainWindow::onFullscreenShortcut(){
    this->goMode();
}

void MainWindow::center_window(){
    if(this->in_fullscreen())
        return;

    QRect screen = QApplication::desktop()->screenGeometry();
    move(screen.width()/2 - width()/2, screen.height()/2 - height()/2);
    mc->storeWindowPosition();
}

void MainWindow::text_changed(QString text){
    mc->updateIcon(text, mc->getIconTheme());

    if(!mc->getSearchType().compare("standard"))
        worker->updateWork(text, mc->getSearchTime() * 1000);
}

void MainWindow::change_dpi(double new_dpi, bool fullscreen_on){
    wc->set_option("dpi", QString::number(new_dpi));

    mc->updateSboxHeight();
    mc->setFont(wc->get_option("font"), wc->get_option("font-size"));

    if(fullscreen_on)
        return;

    this->move(wc->get_option("x").toInt(), wc->get_option("y").toInt());
    this->resize(mc->toDpi(wc->get_option("width")), mc->toDpi(wc->get_option("height")));
    this->request_resize();
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

void MainWindow::updateFiles(){
    wc->update_file();
    ct->update_file();
}

void MainWindow::previewAreaEventFilter(QEvent *event){
    if(event->type() == QEvent::Wheel){
        bool isControlPressed = QApplication::keyboardModifiers() & Qt::ControlModifier;
        QWheelEvent *wheel = static_cast<QWheelEvent*>(event);

        if(isControlPressed)
            mc->setPreviewScale(wheel->angleDelta().y() > 0 ? 0.1 : -0.1);
    }
    else if(event->type() == QEvent::Paint){
        if(mc->canRender){
            // TODO: IMPLEMENT ZOOM MUST TAKE INTO ACCOUNT THE POSITION OF THE MOUSE

            if(!mc->getCurPreviewPath().isEmpty()){
                QImageReader reader(mc->getCurPreviewPath());
                int readerWidth = reader.size().width();
                int readerHeight = reader.size().height();

                int width_ = std::min(ui->previewArea->width(), readerWidth);
                int height_ = std::min(int(ui->previewArea->width()/(readerWidth/(readerHeight * 1.0f))), readerHeight);
                int pos_x = (readerWidth < ui->previewArea->width()) ? ui->previewArea->width()/2 - int(readerWidth/2) : 0;
                int pos_y = (readerHeight < ui->previewArea->height()) ? ui->previewArea->height()/2 - int(readerHeight/2)
                        : ui->previewArea->height()/2 - height_/2;

                double scale = mc->getPreviewScale();
                reader.setScaledSize(QSize(scale * width_, scale * height_));
                QImage tmp = reader.read();
                QPainter painter(ui->previewArea);
                painter.drawImage(pos_x, pos_y, tmp);
                imgCache = (ImageCache){tmp, mc->getCurPreviewPath(), pos_x, pos_y};
            }

            ui->previewArea->update();
            mc->canRender = false;
        }
        else if(!imgCache.path.compare(mc->getCurPreviewPath())){
            // USING IMAGE CACHE HERE

            QPainter painter(ui->previewArea);
            painter.drawImage(imgCache.posx, imgCache.posy, imgCache.img);
        }
    }
}

bool MainWindow::resultsEventFilter(QEvent *event){
    if(event->type() == QEvent::Wheel){
        QWheelEvent *wheel = static_cast<QWheelEvent*>(event);
        return wheel->orientation() == Qt::Horizontal ? true : false;
    }

    return false;
}

void MainWindow::sboxEventFilter(QEvent *event){
    if(event->type() == KEYPRESS ){
        QKeyEvent *ev = static_cast<QKeyEvent *>(event);
        if(ev->key() == Qt::Key_Return)
            rc->openSelectedResult();
        else if(ev->key() == Qt::Key_Up)
            rc->changeSelection(-1);
        else if(ev->key() == Qt::Key_Down)
            rc->changeSelection(+1);
    }
    else{
        QPoint cur = static_cast<const QMouseEvent*>(event)->pos();

        if(abs(mc->toDpi(wc->get_option("search-height")) - cur.y()) <= GRIP_SIZE)
            QApplication::setOverrideCursor(Qt::SplitVCursor);
        else if(cur.y() && !scaling)
            QApplication::setOverrideCursor(Qt::IBeamCursor);
        if(event->type() == QEvent::MouseButtonRelease)
            if(abs(mc->toDpi(wc->get_option("search-height")) - cur.y()) > GRIP_SIZE)
                QApplication::setOverrideCursor(Qt::IBeamCursor);
        if(event->type() == QEvent::Leave)
            QApplication::setOverrideCursor(Qt::ArrowCursor);

        if(scaling && event->type() == QEvent::MouseMove)
            mc->updateSboxHeight(cur.y() - mpos.y());
        if(event->type() == QEvent::MouseMove)
            mpos = cur;
        if(event->type() == QEvent::MouseButtonPress)
            if(abs(mc->toDpi(wc->get_option("search-height")) - cur.y()) <= GRIP_SIZE)
                scaling = true;
        if(event->type() == QEvent::MouseButtonRelease)
            scaling = false;
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event){
    if(obj == ui->previewArea)
        previewAreaEventFilter(event);
    else if(obj == ui->sbox)
        sboxEventFilter(event);
    else if(obj == ui->results)
        return resultsEventFilter(event);

    return false;
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

QLineEdit* MainWindow::sboxUi(){ return ui->sbox; }
QFrame* MainWindow::frameUi(){ return ui->frame;  }
QToolButton* MainWindow::iconUi(){ return icon;   }
QWidget* MainWindow::previewUi(){ return ui->previewArea; }
QPlainTextEdit* MainWindow::previewTextUi(){ return ui->plainTextEdit; }

void MainWindow::inits(){
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    ui->scrollArea->setFocusPolicy(Qt::NoFocus);
    ui->plainTextEdit->setFocusPolicy(Qt::NoFocus);

    // INIT FORMAT THEMES
    initFormatThemes();

    // LINE EDIT DEFAULT ICON
    icon = new QToolButton(ui->sbox);

    // CSS CLASS NAMES
    ui->frame->setObjectName("Frame");
    ui->sbox->setObjectName("Sbox");
    icon->setObjectName("Icon");

    // CONTAINER FOR STYLESHEETS
    ct = new Container("../User/stylesheet.user");
    ui->sbox->setStyleSheet(ct->getStylesheet("Sbox", SBOX));
    ui->frame->setStyleSheet(ct->getStylesheet("Frame", FRAME));
    icon->setStyleSheet(ct->getStylesheet("Icon", ICON));

    // CONTROLLERS
    wc = new WindowController("../User/window.user");
    mc = new MainController(this, this->wc, this->ct);
    rc = new ResultsController(ui->results, ui->scrollArea);

    // INIT STORED PATH
    QPixmap pixmap((mc->getIconTheme().compare("dark") ? LIGHT_ICONS_PATH : DARK_ICONS_PATH) + "search.svg");
    icon->setIcon(QIcon(pixmap));
    icon->setIconSize(pixmap.size());
    icon->setVisible(!mc->getHideIcon());

    // USER INTERFACE DEFAULTS
    this->change_dpi(wc->get_option("dpi").toDouble(), false);
    mc->setFontColor(ct->getStyle("color", SBOX).toUtf8().constData());
    mc->setShadow(QColor(0,0,0, mc->shadowAlpha()), mc->shadowScale(), mc->shadowBlurRadius(), false);
    if(wc->get_option("fullscreen").toInt())
        goFullScreenMode();

    // UNIX SIGNALS HANDLER AND CLEANER
    setupCleaner();
    signals_handler();

    // SETUP FULLSCREEN AND GLOBAL SHORTCUTS
    setupGlobalShortcut();
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_F12), this);

    // EVENT FILTERS AND SIGNALS/SLOTS
    ui->sbox->installEventFilter(this);
    ui->results->installEventFilter(this);
    ui->previewArea->installEventFilter(this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(onFullscreenShortcut()));
    connect(icon, SIGNAL(clicked()), this, SLOT(clear_trigged()));
    connect(ui->sbox, SIGNAL(textChanged(QString )), this, SLOT(text_changed(QString )));
    connect(ui->sbox, SIGNAL(selectionChanged()), this, SLOT(selection_changed()));
}
