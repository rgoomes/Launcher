#ifndef ATOMICBOOL_H
#define ATOMICBOOL_H

#include <QMutex>
#include <QSemaphore>

class AtomicBool
{
public:
    AtomicBool(bool v=false);
    bool get();
    void set(bool);

private:
    QMutex mutex;
    bool value;

};

#endif // ATOMICBOOL_H
