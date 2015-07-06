#include "mainwindow.h"
#include <QApplication>
#include <QDir>

#include "NcFramelessHelper.h"

#define USER_FOLDER "../User"

int main(int argc, char *argv[]){

    // CREATE USER FOLDER
    if(!QDir(USER_FOLDER).exists())
        QDir().mkdir(USER_FOLDER);

    QApplication a(argc, argv);
    MainWindow w;
    /*NcFramelessHelper fh;
    fh.activateOn(&w);*/
    w.show();

    return a.exec();
}
