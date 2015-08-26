#include "worker.h"

Worker::Worker(){
    // TODO
    results = new QList<QString>();
    hasWork = new Job();
    reset = new AtomicBool(false);
}

Worker::~Worker() {
    // TODO
}

// Start.
void Worker::process() {
    qDebug("Hello Thread!");
    while(true){
        hasWork->remove();
        reset->set(false);
        resultsLock.lock();
        results->clear();
        resultsLock.unlock();
        qDebug() << "Working";
        search();

    }
}

void Worker::search(){
    QDir dir = QDir::home();
    qDebug() << "Start Work";
    for(int depth=0; depth<10; depth++){
        dfs(depth, &dir);
        if(reset->get())
            break;
    }
    qDebug() << "End Work\n";
    if(!reset->get()){
        qDebug() << "Results:";
        resultsLock.lock();
        for(QString s : *results)
            qDebug() << s;
        resultsLock.unlock();
    }
}

void Worker::dfs(int depth, QDir *cur){
    if(reset->get()){
        qDebug() << "Stop Work";
        return;
    }
    if(depth <= 0)
        return;
    if (cur->dirName().startsWith("."))
        return;
    QStringList files = cur->entryList(QDir::Files);
    if(depth == 1){
        resultsLock.lock();
        for(QString f : files){
            if (f.toLower().contains(key)){
                results->append(f);
            }
        }
        resultsLock.unlock();
    }
    QStringList dirs = cur->entryList(QDir::Dirs);
    for(QString d : dirs){
        QDir *ndir = new QDir(cur->absolutePath() + "/" + d);
        dfs(depth-1, ndir);
    }
}

void Worker::removeUnmatched(){
    qDebug() << "key:" << this->key;
    resultsLock.lock();
    for(int i = 0; i < results->size(); ){
        if( !results->at(i).toLower().contains(this->key) ){
            results->removeAt(i);
        }else{
            i++;
        }
    }
    resultsLock.unlock();
    qDebug() << "New size" << results->size();
}

void Worker::updateWork(QString k){
    qDebug() << k << k.isEmpty();
    k = k.toLower();

    if(k.isEmpty()){
        qDebug() << "Stop Work";
        this->key = "";

        reset->set(true);
    }else if(k.contains(this->key)){
        QString curkey = this->key;
        this->key = k;
        removeUnmatched();

        qDebug() << "continuing";
        if(curkey.isEmpty())
            hasWork->add();
    }else{
        qDebug() << "restarting";
        this->key = k;

        reset->set(true);
        hasWork->add();
    }

}
