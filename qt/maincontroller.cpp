#include "maincontroller.h"

#define DARK_ICONS_PATH  QString("../qt/icons/dark/")
#define LIGHT_ICONS_PATH QString("../qt/icons/light/")

#define MAX_SCALE 5.0
#define MIN_SCALE 1.0
#define MARGIN_SIZE 30
#define PADDING 5
#define LIMIT 128

MainController::MainController(MainWindow *w, WindowController *wc, Container *ct){
    this->wc = wc;
    this->ct = ct;
    this->w = w;

    previewScale = 1.0f;
    curPreviewPath = "";
    canRender = false;
}

double MainController::curDpi() {
    return wc->get_option("dpi").toDouble();
}

int MainController::toDpi(QString px){
    return int(px.toInt() * curDpi());
}

int MainController::toPx(int px){
    return int(px / curDpi());
}

int MainController::getResizeMargin(){
    return wc->get_option("resize-margin").toInt();
}

void MainController::setResizeMargin(int margin){
    wc->set_option("resize-margin", QString::number(margin));
}

void MainController::storeWindowPosition(){
    int win_gap = getResizeMargin();

    wc->set_option("x", QString::number(std::max(w->x(), 0) + win_gap));
    wc->set_option("y", QString::number(std::max(w->y(), 0) + win_gap));
}

QString MainController::getBackgroundColor(){
    return ct->getStyle("background-color", FRAME);
}

int MainController::getBackgroundAlpha(){
    int alpha;
    std::stringstream ss(ct->getStyle("background-color", FRAME).toStdString());

    ss.ignore(LIMIT, ',');
    ss.ignore(LIMIT, ',');
    ss.ignore(LIMIT, ',');
    ss >> alpha;

    return alpha;
}

QString MainController::getSboxBorderColor(){
    return ct->getStyle("border-color", SBOX);
}

void MainController::setSboxBorderColor(std::string color){
    ct->setStyle("border-color", QString::fromStdString(color), SBOX);
    w->sboxUi()->setStyleSheet(ct->getStylesheet("Sbox", SBOX));
}

void MainController::setBackgroundColor(QColor color, bool random){
    QString c = genColor(color, random, getBackgroundAlpha());

    ct->setStyle("background-color", c, FRAME);
    w->frameUi()->setStyleSheet(ct->getStylesheet("Frame", FRAME));
}

int MainController::sboxBorderWidth(){
    return ct->getStyle("border-width", SBOX).toInt();
}

void MainController::setSboxBorderWidth(int width){
    ct->setStyle("border-style", width ? "solid" : "transparent", SBOX);
    ct->setStyle("border-width", QString::number(width), SBOX);
    w->sboxUi()->setStyleSheet(ct->getStylesheet("Sbox", SBOX));
    changeIconPos(true);
}

int MainController::shadowAlpha(){
    return wc->get_option("shadow-alpha").toInt();
}

int MainController::shadowScale(){
    return wc->get_option("shadow-scale").toInt();
}

int MainController::shadowBlurRadius(){
    return wc->get_option("shadow-blur-radius").toInt();
}

void MainController::setShadow(QColor c, int scale, int blur_radius, bool going_fullscreen){
    if(!w->in_fullscreen()){
        ShadowEffect* shadow = new ShadowEffect();

        shadow->setColor(c);
        shadow->setDistance(scale);
        shadow->setBlurRadius(blur_radius);
        w->frameUi()->setGraphicsEffect(shadow);
    }

    if(!going_fullscreen){
        wc->set_option("shadow-scale", QString::number(scale));
        wc->set_option("shadow-alpha", QString::number(c.alpha()));
        wc->set_option("shadow-blur-radius", QString::number(blur_radius));
    }
}

int MainController::getSboxBorderRadius(){
    return ct->getStyle("border-radius", SBOX).toInt();
}

void MainController::setSboxBorderRadius(int r){
    ct->setStyle("border-radius", QString::number(r), SBOX);
    w->sboxUi()->setStyleSheet(ct->getStylesheet("Sbox", SBOX));
}

int MainController::getBorderRadius(){
    return ct->getStyle("border-radius", FRAME).toInt();
}

void MainController::setBorderRadius(int r, bool going_fullscreen){
    int old_radius = getBorderRadius();
    ct->setStyle("border-radius", QString::number(r), FRAME);

    if(!w->in_fullscreen())
        w->frameUi()->setStyleSheet(ct->getStylesheet("Frame", FRAME));
    if(going_fullscreen)
        ct->setStyle("border-radius", QString::number(old_radius), FRAME);
}

int MainController::iconOnLeft(){
    return wc->get_option("search-icon-pos").toInt();
}

void MainController::changeIconPos(bool keep){
    int on_left = iconOnLeft();

    if(!keep){
        ct->setStyle("padding-left", on_left ? "0px" : "30px", SBOX);
        ct->setStyle("padding-right", on_left ? "30px" : "0px", SBOX);
        wc->set_option("search-icon-pos", QString::number(!on_left));
    }

    int icon_width = w->iconUi()->iconSize().width();
    int width = (on_left ^ !keep) ? PADDING
              : (w->in_fullscreen()  ? QApplication::desktop()->screenGeometry().width() - MARGIN_SIZE - icon_width
              :  toDpi(wc->get_option("width")) - MARGIN_SIZE-PADDING*2 - icon_width);

    w->iconUi()->move(width + sboxBorderWidth() * ((on_left ^ !keep) ? 1 : -1),
               toDpi(wc->get_option("search-height"))/2 - icon_width/2);
    w->sboxUi()->setStyleSheet(ct->getStylesheet("Sbox", SBOX));
}

QString MainController::getIconTheme(){
    return wc->get_option("icon-theme");
}

void MainController::updateIcon(QString text, QString theme){
    wc->set_option("icon-theme", theme);
    QPixmap pixmap((theme.compare("dark") ? LIGHT_ICONS_PATH : DARK_ICONS_PATH) +
                   (text.compare("") ? "clear.svg" : "search.svg"));
    w->iconUi()->setIcon(QIcon(pixmap));
}

std::vector<QString> MainController::getFont(){
    return {wc->get_option("font"), wc->get_option("font-size"), ct->getStyle("color", SBOX)};
}

void MainController::setFont(QString font, QString size){
    w->sboxUi()->setFont(QFont(font, toDpi(size)));

    wc->set_option("font", font);
    wc->set_option("font-size", size);

    // SETTING FONT WITH HIGH SIZE MIGHT EXCEED SBOX HEIGHT SO WE NEED TO UPDATE IT.
    // ALSO WE NEED TO MAKE SURE SBOX ICON STAYS ALWAYS IN THE MIDDLE OF THE SBOX
    updateSboxHeight();
}

void MainController::setFontColor(std::string color){
    ct->setStyle("color", QString::fromStdString(color), SBOX);
    w->sboxUi()->setStyleSheet(ct->getStylesheet("Sbox", SBOX));
}

void MainController::updateSboxHeight(int diff){
    QFontMetrics fm(QFont(wc->get_option("font"), toDpi(wc->get_option("font-size"))));
    int margin_fix = 2*sboxBorderWidth()+1;

    int height = std::max(wc->get_option("search-height").toInt() + toPx(diff), toPx(fm.height()) + margin_fix);
    height = std::min(height, -toPx(MARGIN_SIZE) + (!w->in_fullscreen() ? wc->get_option("height").toInt()
           : toPx(QApplication::desktop()->screenGeometry().height())));
    height = std::max(height, 0);

    w->sboxUi()->setMinimumHeight(toDpi(QString::number(height)));
    wc->set_option("search-height", QString::number(height));

    changeIconPos(true);
}

int MainController::getMaxResults(){
    return wc->get_option("max-results").toInt();
}

void MainController::setMaxResults(int results){
    wc->set_option("max-results", QString::number(results));
}

int MainController::getSearchTime(){
    return wc->get_option("search-time").toInt();
}

void MainController::setSearchTime(int time){
    wc->set_option("search-time", QString::number(time));
}

QString MainController::getSearchType(){
    return wc->get_option("search-type");
}

void MainController::setSearchType(QString type){
    wc->set_option("search-type", type);
}

void MainController::showLauncher(){
    if(!w->isVisible()){
        w->activateWindow();
        w->sboxUi()->setFocus();
        w->sboxUi()->grabKeyboard();
        w->show();
    }
}

void MainController::hideLauncher(){
    if(w->isVisible()){
        w->sboxUi()->setText("");
        w->hide();
    }
}

bool MainController::hideOnApp(){
    return wc->get_option("hide-on-app").toInt();
}

void MainController::setHideOnApp(int state){
    wc->set_option("hide-on-app", QString::number(state));
}

bool MainController::getHideIcon(){
    return wc->get_option("icon-hidden").toInt();
}

void MainController::setHideIcon(int state){
    w->iconUi()->setVisible(!state);
    wc->set_option("icon-hidden", QString::number(state));
}

QString MainController::getGlobalShortcut(){
    return wc->get_option("global-shortcut");
}

void MainController::setGlobalShortcut(QString shortcut){
    wc->set_option("global-shortcut", shortcut);
}

QString MainController::getSboxText(){
    return w->sboxUi()->text();
}

QString MainController::getSelectionColor(){
    return ct->getStyle("selection-background-color", SBOX);
}

void MainController::setSelectionColor(QColor color){
    QString c = genColor(color, false, getSelectionAlpha());

    ct->setStyle("selection-background-color", c, SBOX);
    w->sboxUi()->setStyleSheet(ct->getStylesheet("Sbox", SBOX));
}

int MainController::getSelectionAlpha(){
    int alpha;
    std::stringstream ss(ct->getStyle("selection-background-color", SBOX).toStdString());

    ss.ignore(LIMIT, ',');
    ss.ignore(LIMIT, ',');
    ss.ignore(LIMIT, ',');
    ss >> alpha;

    return alpha;
}

void MainController::setPreviewScale(double diff){
    previewScale += diff;

    previewScale = std::min(previewScale, MAX_SCALE);
    previewScale = std::max(previewScale, MIN_SCALE);
}

QString MainController::getCurPreviewPath(){
    return this->curPreviewPath;
}

void MainController::setPreviewPath(QString curPreviewPath){
   this->curPreviewPath = curPreviewPath;
}

double MainController::getPreviewScale(){
    return this->previewScale;
}
