#ifndef WORKER_H
#define WORKER_H

#include <QDir>
#include <QObject>
#include <QList>
#include <QString>
#include <QSemaphore>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>
#include <QMainWindow>
#include "job.h"
#include "atomicbool.h"

class Interrupt{
public:
    Interrupt(){
    }
};

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker();
    ~Worker();
    void updateWork(QString);

private:
    QString key;
    QList<QString>* results;
    AtomicBool *reset;
    Job *hasWork;
    QMutex resultsLock;

    void search();
    void dfs(int, QDir*) throw(Interrupt);
    void removeUnmatched();

public slots:
    void process();

signals:
    void finished();
    void error(QString err);

};

#endif // WORKER_H

