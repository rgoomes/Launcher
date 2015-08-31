#include "cleaner.h"

Cleaner::~Cleaner(){}
Cleaner::Cleaner(MainWindow *w, QMutex *mtx){
    this->mtx = mtx;
    this->w = w;
}

void Cleaner::cleanExit(){
    mtx->lock();

    // UPDATE ALL CHANGES
    w->update();

    // TODO: LATER CLEAN ALL THE STUFF

    mtx->unlock();
    QApplication::quit();
}
