#include "job.h"
#include <QDebug>

Job::Job(){
    mutex = new QMutex();
    sem = new QSemaphore(0);
    count = 0;
}

void Job::remove() {
    sem->acquire();
}

void Job::add() {
    if(sem->available() == 0)
        sem->release();

}
