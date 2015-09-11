#ifndef CLEANER_H
#define CLEANER_H

#include <QApplication>
#include <QObject>

#include "mainwindow.h"

class Cleaner : public QObject {
    Q_OBJECT

    class MainWindow *w;
    QMutex *mtx;

    public:
        Cleaner(MainWindow* , QMutex* );
        ~Cleaner();

    public slots:
        void cleanExit();

    signals:
        void finished();
};

#endif // CLEANER_H
