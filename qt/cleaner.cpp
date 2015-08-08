#include "cleaner.h"

Cleaner::~Cleaner(){}
Cleaner::Cleaner(std::mutex *mtx, WindowController *ctrl, Container *cc){
    this->mtx = mtx;
    this->ctrl = ctrl;
    this->cc = cc;
}

void Cleaner::cleanExit(){
    mtx->lock();

    // UPDATE ALL CHANGES
    cc->update_file();
    ctrl->update_file();

    // TODO: LATER CLEAN ALL THE STUFF

    mtx->unlock();
    QApplication::quit();
}
