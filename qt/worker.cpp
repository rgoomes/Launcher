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
    try{
        for(int depth=0; depth<10; depth++){
            dfs(depth, &dir);
            if(reset->get())
                break;
        }
    }catch(Interrupt itr){
    }

    if(!reset->get()){
        qDebug() << "Got Results";
        /*resultsLock.lock();
        for(QString s : *results)
            qDebug() << s;
        resultsLock.unlock();*/
    }

}

void Worker::dfs(int depth, QDir *cur) throw(Interrupt){
    if(reset->get()){
        throw Interrupt();
    }
    if(depth <= 0)
        return;
    if (cur->dirName().startsWith("."))
        return;
    if(depth == 1){
        QStringList files = cur->entryList(QDir::Files);
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
        delete ndir;
    }
}

void Worker::removeUnmatched(){
    resultsLock.lock();
    for(int i = 0; i < results->size(); ){
        if( !results->at(i).toLower().contains(this->key) ){
            results->removeAt(i);
        }else{
            i++;
        }
    }
    resultsLock.unlock();
}

void Worker::updateWork(QString k){
    k = k.toLower();

    if(k.isEmpty()){
        this->key = "";

        reset->set(true);
    }else if(k.contains(this->key)){
        QString curkey = this->key;
        this->key = k;
        removeUnmatched();

        if(curkey.isEmpty())
            hasWork->add();
    }else{
        this->key = k;

        reset->set(true);
        hasWork->add();
    }

}
