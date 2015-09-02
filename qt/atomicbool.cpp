#include "atomicbool.h"

AtomicBool::AtomicBool(bool v){
    value = v;
}

bool AtomicBool::get(){
    mutex.lock();
    bool v = value;
    mutex.unlock();
    return v;
}

void AtomicBool::set(bool v){
    mutex.lock();
    value = v;
    mutex.unlock();
}
