#ifndef WORKER_H
#define WORKER_H

#include <QDir>
#include <QObject>
#include <QList>
#include <QString>
#include <QSemaphore>
#include <QDebug>

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
    bool working = false;
    QSemaphore *hasWork;
    void search();
    void dfs(int, QDir*);
    void removeUnmatched();

public slots:
    void process();

signals:
    void finished();
    void error(QString err);

};

#endif // WORKER_H

