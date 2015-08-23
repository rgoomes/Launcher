#include "settingswindow.h"
#include "ui_settingswindow.h"

#include <sstream>
#include <QColorDialog>
#include <QRgb>

#define LIMIT 128
#define MAKE_EDITABLE "QComboBox { combobox-popup: 0; }"

std::vector<double> dpis = {0.5, 0.625, 0.75, 0.875, 1.0, 1.12, 1.25, 1.38, 1.5};
std::vector<int> lpos = {166, 177, 199, 214, 246, 255, 269, 290, 312};
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
    if(event->key() == Qt::Key_Escape)
        this->close();
}

void SettingsWindow::change_backcolor(){
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
    ui->backColorBtn->setStyleSheet(btn_style(w->getBackgroundColor(), w->isShadowVisible()).c_str());
}

void SettingsWindow::change_bordercolor(){
    QColor c = QColorDialog::getColor(QColor(w->getSboxBorderColor()), 0, QString());
    if(!c.isValid())
        return;

    w->setSboxBorderColor(c.name().toUtf8().constData());
    ui->borderColorBtn->setStyleSheet(btn_style(w->getSboxBorderColor(), false).c_str());
}

void SettingsWindow::change_textcolor(){
    QColor c = QColorDialog::getColor(QColor(w->getFont()[2]), 0, QString());
    if(!c.isValid())
        return;

    w->setFontColor(c.name().toUtf8().constData());
    ui->fontColorBtn->setStyleSheet(btn_style(w->getFont()[2], false).c_str());
}

void SettingsWindow::new_bordersize(int new_value){
    w->setBorderRadius(new_value, false);
}

void SettingsWindow::new_borderwidth(int new_value){
    w->setSboxBorderWidth(new_value);
}


void SettingsWindow::new_dpi(int pos){
    w->change_dpi(dpis[pos], w->in_fullscreen());
    ui->dpiNumberLabel->setText(QString::number(dpis[pos]));
    ui->dpiNumberLabel->move(QPoint(lpos[pos],ui->dpiNumberLabel->y()));
}

void SettingsWindow::radiobtn_toggled(bool ){
    w->changeIconPos(false);
}

void SettingsWindow::change_icontheme(bool ){
    w->updateIcon(w->getSboxText(), w->getIconTheme().compare("light") ? "light" : "dark");
}

void SettingsWindow::font_sizechange(const QString &s){
    w->setFont(w->getFont()[0], s);
}

void SettingsWindow::font_familychange(const QString &s){
    w->setFont(s, w->getFont()[1]);
}

void SettingsWindow::updateBtnWindowState(){
    bool in_full = w->in_fullscreen();
    ui->fullWindowBtn->setText(in_full ? "Go Fullscreen Mode" : "Go Window Mode");

    if(in_full)
        ui->shadowBtn->setDisabled(false);
    else
        ui->shadowBtn->setDisabled(true);
}

void SettingsWindow::center(){
    w->center_window();
}

void SettingsWindow::change_shadowstate(){
    ui->shadowBtn->setText(w->isShadowVisible() ? "Enable Shadow" : "Disable Shadow");
    w->setShadow(QColor(0, 0, 0, w->isShadowVisible() ? 0 : 255), 3, 15, !w->in_fullscreen());

    ui->backColorBtn->setStyleSheet(btn_style(w->getBackgroundColor(), w->isShadowVisible()).c_str());
}

void SettingsWindow::random_color(){
    w->setBackgroundColor(QColor(), true);

    ui->backColorBtn->setStyleSheet(btn_style(w->getBackgroundColor(), w->isShadowVisible()).c_str());
}

void SettingsWindow::change_windowstate(){
    bool in_full = w->in_fullscreen();
    updateBtnWindowState();

    if(in_full)
        w->goWindowMode();
    else
        w->goFullScreenMode();
}

void SettingsWindow::inits(){
    this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    int dist = std::distance(dpis.begin(), std::find(dpis.begin(), dpis.end(), w->curDpi()));

    QStringList values;
    for(int size : font_sizes)
        values << QString::number(size);

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
    ui->borderRadiusSlider->setValue(w->getBorderRadius());
    ui->fullWindowBtn->setText(w->in_fullscreen() ? "Go Window Mode" : "Go Fullscreen Mode");
    ui->shadowBtn->setText(w->isShadowVisible() ? "Disable Shadow" : "Enable Shadow");
    ui->fontSizeCombo->addItems(values);
    ui->fontSizeCombo->setCurrentText(w->getFont()[1]);
    ui->fontSizeCombo->setStyleSheet(MAKE_EDITABLE);
    ui->fontFamilyCombo->setStyleSheet(MAKE_EDITABLE);
    ui->fontFamilyCombo->setCurrentText(w->getFont()[0]);
    ui->fontColorBtn->setStyleSheet(btn_style(w->getFont()[2], false).c_str());
    ui->backColorBtn->setStyleSheet(btn_style(w->getBackgroundColor(), w->isShadowVisible()).c_str());
    ui->borderColorBtn->setStyleSheet(btn_style(w->getSboxBorderColor(), false).c_str());
    ui->borderWidthSlider->setValue(w->sboxBorderWidth());

    connect(ui->lightRadio,  SIGNAL(toggled(bool)), this, SLOT(change_icontheme(bool)));
    connect(ui->iconLeftRadioButton,  SIGNAL(toggled(bool)), this, SLOT(radiobtn_toggled(bool)));
    connect(ui->borderRadiusSlider, SIGNAL(valueChanged(int)), this, SLOT(new_bordersize(int )));
    connect(ui->borderWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(new_borderwidth(int )));
    connect(ui->fontSizeCombo, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(font_sizechange(const QString&)));
    connect(ui->fontFamilyCombo, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(font_familychange(const QString&)));
    connect(ui->fullWindowBtn, SIGNAL(clicked()), this, SLOT(change_windowstate()));
    connect(ui->shadowBtn, SIGNAL(clicked()), this, SLOT(change_shadowstate()));
    connect(ui->randomBackColor, SIGNAL(clicked()), this, SLOT(random_color()));
    connect(ui->backColorBtn, SIGNAL(clicked()), this, SLOT(change_backcolor()));
    connect(ui->fontColorBtn, SIGNAL(clicked()), this, SLOT(change_textcolor()));
    connect(ui->dpiSlider, SIGNAL(valueChanged(int)), this, SLOT(new_dpi(int )));
    connect(ui->centerBtn, SIGNAL(clicked()), this, SLOT(center()));
    connect(ui->borderColorBtn, SIGNAL(clicked()), this, SLOT(change_bordercolor()));
}
