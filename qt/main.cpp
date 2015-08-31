#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDir>

#include "singleinstance.h"

#define USER_FOLDER "../User"

void showErrorMessage(){
    QMessageBox::Icon icon = QMessageBox::Information;
    QString title = "Launcher";
    QString text  = "There's already an instance of Launcher running.";
    QMessageBox::StandardButton btn = QMessageBox::Ok;
    Qt::WindowFlags flags = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowStaysOnTopHint;

    QMessageBox running(icon, title, text, btn, 0, flags);
    running.exec();
}

int main(int argc, char *argv[]){

    // CREATE USER FOLDER
    if(!QDir(USER_FOLDER).exists())
        QDir().mkdir(USER_FOLDER);

    QApplication a(argc, argv);

    SingleInstance single("single-instance");
    if(!single.tryToRun()){
        showErrorMessage();
        return -1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
