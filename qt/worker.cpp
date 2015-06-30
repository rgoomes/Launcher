#include "worker.h"

Worker::Worker() : QObject(){
    // TODO
}

Worker::~Worker() {
    // TODO
}

// Start.
void Worker::process() {
    qDebug("Hello Thread!");
}
