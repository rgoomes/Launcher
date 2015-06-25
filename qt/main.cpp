#include "mainwindow.h"
#include <QApplication>
#include <QDir>

#define USER_FOLDER "../User"

int main(int argc, char *argv[]){

    // CREATE USER FOLDER
    if(!QDir(USER_FOLDER).exists())
        QDir().mkdir(USER_FOLDER);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
