#ifndef CLEANER_H
#define CLEANER_H

#include <QApplication>
#include <QObject>
#include <mutex>

#include "stylecontainer.hpp"
#include "windowcontroller.h"

class Cleaner : public QObject {
    Q_OBJECT

    std::mutex *mtx;
    WindowController *ctrl;
    Container *cc;

    public:
        Cleaner(std::mutex* , WindowController* , Container* );
        ~Cleaner();

    public slots:
        void cleanExit();
};

#endif // CLEANER_H
