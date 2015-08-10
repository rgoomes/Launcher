#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::~SettingsWindow(){ delete ui; }
SettingsWindow::SettingsWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::SettingsWindow){
    ui->setupUi(this);
}

std::string btn_style(QString c, bool border){
    return "background-color:" + std::string(c.toUtf8().constData())
           + std::string(";\nborder-style: solid;\nborder-width:3px;\nmax-width:70px;\n")
           + std::string("border-radius:5px;\nmax-height:20px;\nmin-width:70px;\nmin-height:20px;\n")
           + (border ? std::string("border-color: #CCCBCA;") : std::string(""));
}

void SettingsWindow::setMainWindow(MainWindow *w){
    this->w = w;
    inits();
}

void SettingsWindow::new_bordersize(int new_value){
    w->setBorderRadius(new_value, false);
}

void SettingsWindow::checkbox_toggled(bool on){
    w->setBorderVisibility();
}

void SettingsWindow::radiobtn_toggled(bool on_left){
    w->changeIconPos(false);
}

void SettingsWindow::font_sizechange(const QString &s){
    w->setFont(w->getFont()[1], s);
}

void SettingsWindow::updateBtnWindowState(){
    ui->fullWindowBtn->setText(w->in_fullscreen() ? "Go Fullscreen Mode" : "Go Window Mode");
}

void SettingsWindow::change_shadowstate(){
    ui->shadowBtn->setText(w->isShadowVisible() ? "Enable Shadow" : "Disable Shadow");
    w->setShadow(QColor(0, 0, 0, w->isShadowVisible() ? 0 : 255), 3, 15, !w->in_fullscreen());

    std::string style1 = btn_style(w->getFont()[2], w->isShadowVisible());
    std::string style2 = btn_style(w->getBackgroundColor(), w->isShadowVisible());
    ui->fontColorBtn->setStyleSheet(style1.c_str());
    ui->backColorBtn->setStyleSheet(style2.c_str());
}

void SettingsWindow::random_color(){
    QColor c;
    w->setBackgroundColor(c, true);

    std::string style1 = btn_style(w->getFont()[2], w->isShadowVisible());
    std::string style2 = btn_style(w->getBackgroundColor(), w->isShadowVisible());
    ui->fontColorBtn->setStyleSheet(style1.c_str());
    ui->backColorBtn->setStyleSheet(style2.c_str());
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
    std::string style1 = btn_style(font[2], w->isShadowVisible());

    QString back_color = w->getBackgroundColor();
    std::string style2 = btn_style(back_color, w->isShadowVisible());

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
    ui->fontSizeCombo->setStyleSheet("QComboBox { combobox-popup: 0; }");
    ui->fontFamilyCombo->setStyleSheet("QComboBox { combobox-popup: 0; }");
    ui->fontColorBtn->setStyleSheet(style1.c_str());
    ui->backColorBtn->setStyleSheet(style2.c_str());

    connect(ui->iconLeftRadioButton,  SIGNAL(toggled(bool)), this, SLOT(radiobtn_toggled(bool)));
    connect(ui->borderVisibleCheckBox, SIGNAL(clicked(bool)), this, SLOT(checkbox_toggled(bool)));
    connect(ui->borderRadiusSlider, SIGNAL(valueChanged(int)), this, SLOT(new_bordersize(int )));
    connect(ui->fontSizeCombo, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(font_sizechange(const QString&)));
    connect(ui->fullWindowBtn, SIGNAL(clicked()), this, SLOT(change_windowstate()));
    connect(ui->shadowBtn, SIGNAL(clicked()), this, SLOT(change_shadowstate()));
    connect(ui->randomBackColor, SIGNAL(clicked()), this, SLOT(random_color()));
}
