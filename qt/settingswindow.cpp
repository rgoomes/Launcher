#include "settingswindow.h"
#include "ui_settingswindow.h"

#include <sstream>
#include <QColorDialog>
#include <QRgb>

#define LIMIT 128
#define MAKE_EDITABLE "QComboBox { combobox-popup: 0; }"

std::vector<double> dpis = {0.5, 0.625, 0.75, 0.875, 1.0, 1.12, 1.25, 1.38, 1.5};
std::vector<int> lpos = {151, 162, 183, 199, 231, 240, 254, 275, 297};
std::vector<int> font_sizes = {6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 18, 20, 22, 24,
                               26, 28, 32, 36, 40, 44, 48, 54, 60, 66, 72, 80, 88, 96};

SettingsWindow::~SettingsWindow(){ delete ui; }
SettingsWindow::SettingsWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::SettingsWindow){
    ui->setupUi(this);
}

std::string btn_style(QString c, bool border){
    return "background-color:" + std::string(c.toUtf8().constData())
           + std::string(";\nborder-style: solid;\nborder-width:3px;\nmax-width:104px;\noutline:none;\n")
           + std::string("border-radius:5px;\nmax-height:20px;\nmin-width:104px;\nmin-height:20px;\n")
           + (border ? std::string("border-color: #CCCBCA;") : std::string(""));
}

void SettingsWindow::setMainWindow(MainWindow *w){
    this->w = w;
    inits();
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
    std::stringstream ss(w->getBackgroundColor().toStdString());

    ss.ignore(LIMIT, '(');
    ss >> r; ss.ignore(LIMIT, ',');
    ss >> g; ss.ignore(LIMIT, ',');
    ss >> b;

    QColor c = QColorDialog::getColor(QColor(r,g,b,w->getBackgroundAlpha()), 0, QString(), QColorDialog::ShowAlphaChannel);
    if(!c.isValid())
        return;

    w->setBackgroundColor(c, false);
    ui->backColorBtn->setStyleSheet(btn_style(w->getBackgroundColor(), w->shadowAlpha()).c_str());
}

void SettingsWindow::changeBorderColor(){
    QColor c = QColorDialog::getColor(QColor(w->getSboxBorderColor()), 0, QString());
    if(!c.isValid())
        return;

    w->setSboxBorderColor(c.name().toUtf8().constData());
    ui->borderColorBtn->setStyleSheet(btn_style(w->getSboxBorderColor(), false).c_str());
}

void SettingsWindow::changeTextColor(){
    QColor c = QColorDialog::getColor(QColor(w->getFont()[2]), 0, QString());
    if(!c.isValid())
        return;

    w->setFontColor(c.name().toUtf8().constData());
    ui->fontColorBtn->setStyleSheet(btn_style(w->getFont()[2], false).c_str());
}

void SettingsWindow::changeBorderRadius(int new_value){
    w->setBorderRadius(new_value, false);
}

void SettingsWindow::changeBorderWidth(int new_value){
    w->setSboxBorderWidth(new_value);
}

void SettingsWindow::requestDpiChange(int pos){
    w->change_dpi(dpis[pos], w->in_fullscreen());
    ui->dpiNumberLabel->setText(QString::number(dpis[pos]));
    ui->dpiNumberLabel->move(QPoint(lpos[pos],ui->dpiNumberLabel->y()));
}

void SettingsWindow::onRadioBtnToggled(bool ){
    w->changeIconPos(false);
}

void SettingsWindow::changeIconTheme(bool ){
    w->updateIcon(w->getSboxText(), w->getIconTheme().compare("light") ? "light" : "dark");
}

void SettingsWindow::setFontSize(const QString &s){
    w->setFont(w->getFont()[0], s);
}

void SettingsWindow::setFontFamily(const QString &s){
    w->setFont(s, w->getFont()[1]);
}

void SettingsWindow::changeResizeMargin(const QString &s){
    w->setResizeMargin(s.toInt());
}

void SettingsWindow::updateBtnState(){
    ui->fullWindowBtn->setText(w->in_fullscreen() ? "Go Fullscreen Mode" : "Go Window Mode");
}

void SettingsWindow::centerWindow(){
    w->center_window();
}

void SettingsWindow::setRandomColor(){
    w->setBackgroundColor(QColor(), true);

    ui->backColorBtn->setStyleSheet(btn_style(w->getBackgroundColor(), w->shadowAlpha()).c_str());
}

void SettingsWindow::changeWindowMode(){
    bool in_full = w->in_fullscreen();
    updateBtnState();

    if(in_full)
        w->goWindowMode();
    else
        w->goFullScreenMode();
}

void SettingsWindow::changeBlurRadius(int new_value){
    w->setShadow(QColor(0,0,0, w->shadowAlpha()), w->shadowScale(), new_value, w->in_fullscreen());
}

void SettingsWindow::changeShadowAlpha(int new_value){
    w->setShadow(QColor(0,0,0, new_value), w->shadowScale(), w->shadowBlurRadius(), w->in_fullscreen());

    ui->backColorBtn->setStyleSheet(btn_style(w->getBackgroundColor(), w->shadowAlpha()).c_str());
}

void SettingsWindow::changeShadowScale(int new_value){
    w->setShadow(QColor(0,0,0, w->shadowAlpha()), new_value, w->shadowBlurRadius(), w->in_fullscreen());
}

void SettingsWindow::changeHideOnAppState(bool state){
    w->setHideOnApp(state);
}

void SettingsWindow::showLauncher(){
    if(!w->isVisible())
        w->show();
}

void SettingsWindow::inits(){
    this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    int dist = std::distance(dpis.begin(), std::find(dpis.begin(), dpis.end(), w->curDpi()));

    QStringList values, resize_margins;
    for(int size : font_sizes)
        values << QString::number(size);
    for(int i = 0; i <= 30; i++)
        resize_margins << QString::number(i);

    if(w->iconOnLeft())
        ui->iconLeftRadioButton->setChecked(true);
    else
        ui->iconRightRadioButton->setChecked(true);

    if(w->getIconTheme().compare("light"))
        ui->darkRadio->setChecked(true);
    else
        ui->lightRadio->setChecked(true);

    ui->dpiSlider->setValue(dist);
    ui->dpiNumberLabel->move(QPoint(lpos[dist],ui->dpiNumberLabel->y()));
    ui->dpiNumberLabel->setText(QString::number(w->curDpi()));
    ui->shadowBlurSlider->setValue(w->shadowBlurRadius());
    ui->shadowAlphaSlider->setValue(w->shadowAlpha());
    ui->shadowScaleSlider->setValue(w->shadowScale());
    ui->borderRadiusSlider->setValue(w->getBorderRadius());
    ui->fullWindowBtn->setText(w->in_fullscreen() ? "Go Window Mode" : "Go Fullscreen Mode");
    ui->fontSizeCombo->addItems(values);
    ui->fontSizeCombo->setCurrentText(w->getFont()[1]);
    ui->fontSizeCombo->setStyleSheet(MAKE_EDITABLE);
    ui->fontFamilyCombo->setStyleSheet(MAKE_EDITABLE);
    ui->resizePxCombo->setStyleSheet(MAKE_EDITABLE);
    ui->resizePxCombo->addItems(resize_margins);
    ui->resizePxCombo->setCurrentText(QString::number(w->getResizeMargin()));
    ui->fontFamilyCombo->setCurrentText(w->getFont()[0]);
    ui->fontColorBtn->setStyleSheet(btn_style(w->getFont()[2], false).c_str());
    ui->backColorBtn->setStyleSheet(btn_style(w->getBackgroundColor(), w->shadowAlpha()).c_str());
    ui->borderColorBtn->setStyleSheet(btn_style(w->getSboxBorderColor(), false).c_str());
    ui->borderWidthSlider->setValue(w->sboxBorderWidth());
    ui->hideOnAppCheck->setChecked(w->hideOnApp());

    connect(ui->lightRadio,  SIGNAL(toggled(bool)), this, SLOT(changeIconTheme(bool)));
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
    connect(ui->centerBtn, SIGNAL(clicked()), this, SLOT(centerWindow()));
    connect(ui->borderColorBtn, SIGNAL(clicked()), this, SLOT(changeBorderColor()));
    connect(ui->shadowAlphaSlider, SIGNAL(valueChanged(int)), this, SLOT(changeShadowAlpha(int )));
    connect(ui->shadowBlurSlider, SIGNAL(valueChanged(int)), this, SLOT(changeBlurRadius(int )));
    connect(ui->shadowScaleSlider, SIGNAL(valueChanged(int)), this, SLOT(changeShadowScale(int )));
    connect(ui->hideOnAppCheck, SIGNAL(clicked(bool)), this, SLOT(changeHideOnAppState(bool )));
    connect(ui->showLauncherBtn, SIGNAL(clicked(bool)), this, SLOT(showLauncher()));

}
