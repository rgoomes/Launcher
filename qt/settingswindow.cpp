#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "types.h"

#define LIMIT 128
#define WIDTH1 "43"
#define WIDTH2 "104"
#define MAKE_EDITABLE "QComboBox { combobox-popup: 0; }"

std::vector<double> dpis = {0.5, 0.625, 0.75, 0.875, 1.0, 1.12, 1.25, 1.38, 1.5};
std::vector<int> lpos = {151, 156, 176, 185, 214, 218, 231, 244, 263};
std::vector<int> font_sizes = {12, 13, 14, 15, 16, 18, 20, 22, 24, 26, 28, 32, 36, 40, 44, 48, 54, 60, 66, 72, 80, 88, 96};

SettingsWindow::~SettingsWindow(){ delete ui; }
SettingsWindow::SettingsWindow(MainWindow *w, QWidget *parent) : QMainWindow(parent), ui(new Ui::SettingsWindow){
    this->w = w;
    ui->setupUi(this);
    inits();
}

std::string btn_style(QString c, bool border, std::string px){
    return "background-color:" + std::string(c.toUtf8().constData())
           + std::string(";\nborder-style: solid;\nborder-width:3px;\nmax-width:") + px + std::string("px;\noutline:none;\n")
           + std::string("border-radius:6px;\nmax-height:17px;\nmin-width:") + px + std::string("px;\nmin-height:17px;\n")
           + (border ? std::string("border-color: #CCCBCA;") : std::string(""));
}

void SettingsWindow::closeEvent(QCloseEvent *) {
    this->w->settingsOpened = false;
}

void SettingsWindow::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Escape){
        this->close();
        delete this;
    }
}

void SettingsWindow::changeBackgroundColor(){
    int r, g, b;
    std::stringstream ss(mc->getBackgroundColor().toStdString());

    ss.ignore(LIMIT, '(');
    ss >> r; ss.ignore(LIMIT, ',');
    ss >> g; ss.ignore(LIMIT, ',');
    ss >> b;

    QColor c = QColorDialog::getColor(QColor(r,g,b,mc->getBackgroundAlpha()), 0, QString(), QColorDialog::ShowAlphaChannel);
    if(!c.isValid())
        return;

    mc->setBackgroundColor(c, false);
    ui->backColorBtn->setStyleSheet(btn_style(mc->getBackgroundColor(), mc->shadowAlpha(), WIDTH2).c_str());
}

void SettingsWindow::changeBorderColor(){
    QColor c = QColorDialog::getColor(QColor(mc->getSboxBorderColor()), 0, QString());
    if(!c.isValid())
        return;

    mc->setSboxBorderColor(c.name().toUtf8().constData());
    ui->borderColorBtn->setStyleSheet(btn_style(mc->getSboxBorderColor(), false, WIDTH1).c_str());
}

void SettingsWindow::changeSelectionColor(){
    int r, g, b;
    std::stringstream ss(mc->getSelectionColor().toStdString());

    ss.ignore(LIMIT, '(');
    ss >> r; ss.ignore(LIMIT, ',');
    ss >> g; ss.ignore(LIMIT, ',');
    ss >> b;

    QColor c = QColorDialog::getColor(QColor(r,g,b,mc->getSelectionAlpha()), 0, QString(), QColorDialog::ShowAlphaChannel);
    if(!c.isValid())
        return;

    mc->setSelectionColor(c);
    rc->updateSelectionColor();
    ui->selectColorBtn->setStyleSheet(btn_style(mc->getSelectionColor(), false, WIDTH1).c_str());
}

void SettingsWindow::changeTextColor(){
    QColor c = QColorDialog::getColor(QColor(mc->getFont()[2]), 0, QString());
    if(!c.isValid())
        return;

    mc->setFontColor(c.name().toUtf8().constData());
    ui->fontColorBtn->setStyleSheet(btn_style(mc->getFont()[2], false, WIDTH1).c_str());
}

void SettingsWindow::changeBorderRadius(int new_value){
    mc->setBorderRadius(new_value, false);
}

void SettingsWindow::changeBorderWidth(int new_value){
    mc->setSboxBorderWidth(new_value);
}

void SettingsWindow::requestDpiChange(int pos){
    w->change_dpi(dpis[pos], w->in_fullscreen());
    ui->dpiNumberLabel->setText(QString::number(dpis[pos]));
    ui->dpiNumberLabel->move(QPoint(lpos[pos],ui->dpiNumberLabel->y()));
}

void SettingsWindow::onRadioBtnToggled(bool ){
    mc->changeIconPos(false);
}

void SettingsWindow::onSearchTypeChanged(bool isDatabase){
    mc->setSearchType(isDatabase ? "database" : "standard");
}

void SettingsWindow::changeIconTheme(bool ){
    mc->updateIcon(mc->getSboxText(), mc->getIconTheme().compare("light") ? "light" : "dark");
}

void SettingsWindow::setFontSize(const QString &s){
    mc->setFont(mc->getFont()[0], s);
}

void SettingsWindow::setFontFamily(const QString &s){
    mc->setFont(s, mc->getFont()[1]);
}

void SettingsWindow::changeResizeMargin(const QString &s){
    mc->setResizeMargin(s.toInt());
}

void SettingsWindow::updateBtnState(){
    ui->fullWindowBtn->setText(w->in_fullscreen() ? "Go Fullscreen Mode" : "Go Window Mode");
}

void SettingsWindow::centerWindow(){
    w->center_window();
}

void SettingsWindow::setRandomColor(){
    mc->setBackgroundColor(QColor(), true);

    ui->backColorBtn->setStyleSheet(btn_style(mc->getBackgroundColor(), mc->shadowAlpha(), WIDTH2).c_str());
}

void SettingsWindow::changeWindowMode(){
    updateBtnState();
    w->goMode();
}

void SettingsWindow::changeBlurRadius(int new_value){
    mc->setShadow(QColor(0,0,0, mc->shadowAlpha()), mc->shadowScale(), new_value, false);
}

void SettingsWindow::changeShadowAlpha(int new_value){
    mc->setShadow(QColor(0,0,0, new_value), mc->shadowScale(), mc->shadowBlurRadius(), false);

    ui->backColorBtn->setStyleSheet(btn_style(mc->getBackgroundColor(), mc->shadowAlpha(), WIDTH2).c_str());
}

void SettingsWindow::changeShadowScale(int new_value){
    mc->setShadow(QColor(0,0,0, mc->shadowAlpha()), new_value, mc->shadowBlurRadius(), false);
}

void SettingsWindow::changeHideOnAppState(bool state){
    mc->setHideOnApp(state);
}

void SettingsWindow::changeHideIcon(bool state){
    ui->iconPosWidget->setDisabled(state);
    ui->iconThemeWidget->setDisabled(state);

    mc->setHideIcon(state);
}

void SettingsWindow::showLauncher(){
    mc->showLauncher();
}

void SettingsWindow::changeSearchTime(int new_value){
    mc->setSearchTime(new_value);
    ui->realTime->setText(new_value ? QString::number(new_value) + "s" : "Infinite");
}

void SettingsWindow::changeMaxResults(int new_value){
    mc->setMaxResults(new_value);
    ui->resultsLabel->setText(QString::number(new_value));
}

void SettingsWindow::changeSboxBorderRadius(int new_value){
    mc->setSboxBorderRadius(new_value);
}

void SettingsWindow::onTextChanged(QString s){
    mc->setGlobalShortcut(s);
}

void SettingsWindow::inits(){
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    int dist = std::distance(dpis.begin(), std::find(dpis.begin(), dpis.end(), mc->curDpi()));

    QStringList values, resize_margins;
    for(int size : font_sizes)
        values << QString::number(size);
    for(int i = 0; i <= 30; i++)
        resize_margins << QString::number(i);

    if(mc->iconOnLeft())
        ui->iconLeftRadioButton->setChecked(true);
    else
        ui->iconRightRadioButton->setChecked(true);

    if(mc->getIconTheme().compare("light"))
        ui->darkRadio->setChecked(true);
    else
        ui->lightRadio->setChecked(true);

    if(mc->getSearchType().compare("standard"))
        ui->databaseTypeRadio->setChecked(true);
    else
        ui->stdTypeRadio->setChecked(true);

#if defined(_WIN32) || defined(_WIN64)
    ui->databaseTypeRadio->setDisabled(true);
#endif

    ui->dpiSlider->setValue(dist);
    ui->timeSlider->setValue(mc->getSearchTime());
    ui->realTime->setText(mc->getSearchTime() ? QString::number(mc->getSearchTime()) + "s" : "Infinite");
    ui->resultsSlider->setValue(mc->getMaxResults());
    ui->resultsLabel->setText(QString::number(mc->getMaxResults()));
    ui->dpiNumberLabel->move(QPoint(lpos[dist],ui->dpiNumberLabel->y()));
    ui->dpiNumberLabel->setText(QString::number(mc->curDpi()));
    ui->shadowBlurSlider->setValue(mc->shadowBlurRadius());
    ui->shadowAlphaSlider->setValue(mc->shadowAlpha());
    ui->shadowScaleSlider->setValue(mc->shadowScale());
    ui->borderRadiusSlider->setValue(mc->getBorderRadius());
    ui->fullWindowBtn->setText(w->in_fullscreen() ? "Go Window Mode" : "Go Fullscreen Mode");
    ui->fontSizeCombo->addItems(values);
    ui->fontSizeCombo->setEditText(mc->getFont()[1]);
    ui->fontSizeCombo->setStyleSheet(MAKE_EDITABLE);
    ui->fontFamilyCombo->setStyleSheet(MAKE_EDITABLE);
    ui->resizePxCombo->setStyleSheet(MAKE_EDITABLE);
    ui->resizePxCombo->addItems(resize_margins);
    ui->resizePxCombo->setEditText(QString::number(mc->getResizeMargin()));
    ui->fontFamilyCombo->setEditText(mc->getFont()[0]);
    ui->fontColorBtn->setStyleSheet(btn_style(mc->getFont()[2], false, WIDTH1).c_str());
    ui->backColorBtn->setStyleSheet(btn_style(mc->getBackgroundColor(), mc->shadowAlpha(), WIDTH2).c_str());
    ui->borderColorBtn->setStyleSheet(btn_style(mc->getSboxBorderColor(), false, WIDTH1).c_str());
    ui->selectColorBtn->setStyleSheet(btn_style(mc->getSelectionColor(), false, WIDTH1).c_str());
    ui->borderWidthSlider->setValue(mc->sboxBorderWidth());
    ui->hideOnAppCheck->setChecked(mc->hideOnApp());
    ui->hideIconCheck->setChecked(mc->getHideIcon());
    ui->iconPosWidget->setDisabled(mc->getHideIcon());
    ui->iconThemeWidget->setDisabled(mc->getHideIcon());
    ui->sboxBorderRadiusSlider->setValue(mc->getSboxBorderRadius());
    ui->sedit->setText(mc->getGlobalShortcut());

    connect(ui->lightRadio,  SIGNAL(toggled(bool)), this, SLOT(changeIconTheme(bool)));
    connect(ui->databaseTypeRadio,  SIGNAL(toggled(bool)), this, SLOT(onSearchTypeChanged(bool)));
    connect(ui->iconLeftRadioButton,  SIGNAL(toggled(bool)), this, SLOT(onRadioBtnToggled(bool)));
    connect(ui->borderRadiusSlider, SIGNAL(valueChanged(int)), this, SLOT(changeBorderRadius(int )));
    connect(ui->borderWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(changeBorderWidth(int )));
    connect(ui->fontSizeCombo, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(setFontSize(const QString&)));
    connect(ui->fontFamilyCombo, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(setFontFamily(const QString&)));
    connect(ui->resizePxCombo, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(changeResizeMargin(const QString&)));
    connect(ui->fullWindowBtn, SIGNAL(clicked()), this, SLOT(changeWindowMode()));
    connect(ui->randomBackColor, SIGNAL(clicked()), this, SLOT(setRandomColor()));
    connect(ui->backColorBtn, SIGNAL(clicked()), this, SLOT(changeBackgroundColor()));
    connect(ui->fontColorBtn, SIGNAL(clicked()), this, SLOT(changeTextColor()));
    connect(ui->dpiSlider, SIGNAL(valueChanged(int)), this, SLOT(requestDpiChange(int )));
    connect(ui->timeSlider, SIGNAL(valueChanged(int)), this, SLOT(changeSearchTime(int )));
    connect(ui->resultsSlider, SIGNAL(valueChanged(int)), this, SLOT(changeMaxResults(int )));
    connect(ui->centerBtn, SIGNAL(clicked()), this, SLOT(centerWindow()));
    connect(ui->borderColorBtn, SIGNAL(clicked()), this, SLOT(changeBorderColor()));
    connect(ui->shadowAlphaSlider, SIGNAL(valueChanged(int)), this, SLOT(changeShadowAlpha(int )));
    connect(ui->shadowBlurSlider, SIGNAL(valueChanged(int)), this, SLOT(changeBlurRadius(int )));
    connect(ui->shadowScaleSlider, SIGNAL(valueChanged(int)), this, SLOT(changeShadowScale(int )));
    connect(ui->sboxBorderRadiusSlider, SIGNAL(valueChanged(int)), this, SLOT(changeSboxBorderRadius(int )));
    connect(ui->hideOnAppCheck, SIGNAL(clicked(bool)), this, SLOT(changeHideOnAppState(bool )));
    connect(ui->showLauncherBtn, SIGNAL(clicked(bool)), this, SLOT(showLauncher()));
    connect(ui->hideIconCheck, SIGNAL(clicked(bool)), this, SLOT(changeHideIcon(bool )));
    connect(ui->sedit, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString )));
    connect(ui->selectColorBtn, SIGNAL(clicked(bool)), this, SLOT(changeSelectionColor()));
}
