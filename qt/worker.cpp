#include "worker.h"

Worker::Worker() : QObject(){
    // TODO
    results = new QList<QString>();
    hasWork = new QSemaphore(0);
}

Worker::~Worker() {
    // TODO
}

// Start.
void Worker::process() {
    qDebug("Hello Thread!");
    while(true){
        hasWork->acquire();
        working = true;
        search();
        working = false;

    }
}

void Worker::search(){
    QDir dir = QDir::home();
    qDebug() << "Start Work";
    for(int depth=0; depth<10; depth++){
        dfs(depth, &dir);
    }
    qDebug() << "End Work";
    for(QString s : *results)
        qDebug() << s;
}

void Worker::dfs(int depth, QDir *cur){
    if(depth <= 0)
        return;
    if (cur->dirName().startsWith("."))
        return;
    QStringList files = cur->entryList(QDir::Files);
    for(QString f : files){
        if (f.contains(key))
            results->append(f);
    }
    QStringList dirs = cur->entryList(QDir::Dirs);
    for(QString d : dirs){
        QDir *ndir = new QDir(cur->absolutePath() + "/" + d);
        dfs(depth-1, ndir);
    }
}

void Worker::removeUnmatched(){
    for(int i = 0; i < results->size(); ){
        if( !results[i].contains(this->key) )
            results->removeAt(i);
        else
            i++;
    }
}

void Worker::updateWork(QString k){
    if(k.startsWith(this->key)){
        this->key = k;
        removeUnmatched();
    }else{
        results->clear();
        this->key = k;
        hasWork->release();
    }

}
