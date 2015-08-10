#ifndef CLEANER_H
#define CLEANER_H

#include <QApplication>
#include <QObject>
#include <mutex>

#include "stylecontainer.hpp"
#include "windowcontroller.h"
#include "mainwindow.h"

class Cleaner : public QObject {
    Q_OBJECT

    std::mutex *mtx;
    MainWindow *w;

    public:
        Cleaner(MainWindow* , std::mutex* );
        ~Cleaner();

    public slots:
        void cleanExit();
};

#endif // CLEANER_H
