#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::~SettingsWindow(){ delete ui; }
SettingsWindow::SettingsWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::SettingsWindow){
    ui->setupUi(this);
    inits();
}

void SettingsWindow::inits(){

}
