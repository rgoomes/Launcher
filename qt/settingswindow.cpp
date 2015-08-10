#include "settingswindow.h"
#include "ui_settingswindow.h"

#include <QColorDialog>
#include <QRgb>

#define MAKE_EDITABLE "QComboBox { combobox-popup: 0; }"

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

void SettingsWindow::change_backcolor(){
    const char *color = w->getBackgroundColor().toStdString().c_str();
    int r,g,b,alpha; sscanf(color, "rgba(%d,%d,%d,%d)", &r,&g,&b,&alpha);

    QColor c = QColorDialog::getColor(QColor(r,g,b,alpha), 0, QString(), QColorDialog::ShowAlphaChannel);
    if(!c.isValid())
        return;

    w->setBackgroundColor(c, false);
    ui->backColorBtn->setStyleSheet(btn_style(w->getBackgroundColor(), w->isShadowVisible()).c_str());
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

void SettingsWindow::checkbox_toggled(bool ){
    w->setBorderVisibility();
}

void SettingsWindow::radiobtn_toggled(bool ){
    w->changeIconPos(false);
}

void SettingsWindow::font_sizechange(const QString &s){
    w->setFont(w->getFont()[1], s);
}

void SettingsWindow::font_familychange(const QString &s){
    w->setFont(s, w->getFont()[1]);
}

void SettingsWindow::updateBtnWindowState(){
    ui->fullWindowBtn->setText(w->in_fullscreen() ? "Go Fullscreen Mode" : "Go Window Mode");
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
    if(w->iconOnLeft())
        ui->iconLeftRadioButton->setChecked(true);
    else
        ui->iconRightRadioButton->setChecked(true);

    if(w->borderIsVisible())
        ui->borderVisibleCheckBox->setChecked(true);

    ui->borderRadiusSlider->setValue(w->getBorderRadius());
    ui->fullWindowBtn->setText(w->in_fullscreen() ? "Go Window Mode" : "Go Fullscreen Mode");
    ui->shadowBtn->setText(w->isShadowVisible() ? "Disable Shadow" : "Enable Shadow");

    vector<QString> font = w->getFont();
    QString back_color = w->getBackgroundColor();

    QStringList comboValues;
    for(int i = 6; i < 100;){
        comboValues.append(QStringList() << QString::number(i));

        if(i < 16) i++;
        else if(i < 28) i+=2;
        else if(i < 48) i+=4;
        else if(i < 72) i+=6;
        else i+=8;
    }

    ui->fontSizeCombo->addItems(comboValues);
    ui->fontSizeCombo->setCurrentText(font[1]);
    ui->fontSizeCombo->setStyleSheet(MAKE_EDITABLE);
    ui->fontFamilyCombo->setStyleSheet(MAKE_EDITABLE);
    ui->fontFamilyCombo->setCurrentText(font[0]);
    ui->fontColorBtn->setStyleSheet(btn_style(font[2], false).c_str());
    ui->backColorBtn->setStyleSheet(btn_style(back_color, w->isShadowVisible()).c_str());

    connect(ui->iconLeftRadioButton,  SIGNAL(toggled(bool)), this, SLOT(radiobtn_toggled(bool)));
    connect(ui->borderVisibleCheckBox, SIGNAL(clicked(bool)), this, SLOT(checkbox_toggled(bool)));
    connect(ui->borderRadiusSlider, SIGNAL(valueChanged(int)), this, SLOT(new_bordersize(int )));
    connect(ui->fontSizeCombo, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(font_sizechange(const QString&)));
    connect(ui->fontFamilyCombo, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(font_familychange(const QString&)));
    connect(ui->fullWindowBtn, SIGNAL(clicked()), this, SLOT(change_windowstate()));
    connect(ui->shadowBtn, SIGNAL(clicked()), this, SLOT(change_shadowstate()));
    connect(ui->randomBackColor, SIGNAL(clicked()), this, SLOT(random_color()));
    connect(ui->backColorBtn, SIGNAL(clicked()), this, SLOT(change_backcolor()));
    connect(ui->fontColorBtn, SIGNAL(clicked()), this, SLOT(change_textcolor()));
}
