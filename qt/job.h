#ifndef JOB_H
#define JOB_H

#include <QMutex>
#include <QSemaphore>

class Job
{
public:
    Job();
    void remove();
    void add();

private:
    int count;
    QMutex *mutex;
    QSemaphore *sem;
};

#endif // JOB_H
