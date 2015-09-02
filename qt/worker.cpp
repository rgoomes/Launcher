#include "worker.h"

#define MAX_DEPTH 128
#define DEBUG_SEARCH true

std::vector<QString> paths;
QMap<QString, quint8> prunePaths;

void initPrunePaths(){
    #if defined(__linux) || defined(__unix)
        paths = {"/var/spool", "/media", "/home/.ecryptfs", "/tmp", "/usr/tmp", "/var/tmp", "/proc", "/usr/share"};
    #elif defined(_WIN32) || defined(_WIN64)
        paths = {};
    #endif

    for(QString s : paths)
        prunePaths[s];
}

bool ignorePath(QString cur){
    return prunePaths.find(cur) != prunePaths.end() ? true : false;
}

Worker::~Worker(){}
Worker::Worker(){
    initPrunePaths();

    results = new QList<QString>();
    hasWork = new Job();
    reset = new AtomicBool(false);
}

void Worker::process() {
    while(true){
        hasWork->remove();
        reset->set(false);
        resultsLock.lock();
        results->clear();
        resultsLock.unlock();

        #if DEBUG_SEARCH
            qDebug() << "Working";
        #endif

        search();
    }
}

void Worker::search(){
    QDir dir = QDir::root();
    for(int depth=0; depth < MAX_DEPTH; depth++){
        try{
            dfs(depth, &dir);
        } catch(Interrupt &){
            break;
        }

    #if DEBUG_SEARCH
        qDebug() << "depth" << depth+1
                 << "time"  << duration_cast<milliseconds>(high_resolution_clock::now() - t).count()+1
                 << "ms";
    #endif

        if(reset->get())
            break;
    }

    if(!reset->get()){
        #if DEBUG_SEARCH
            qDebug() << "Got Results" << results->size();
        #endif

        /*resultsLock.lock();
        for(QString s : *results)
            qDebug() << s;
        resultsLock.unlock();*/
    }
}

void Worker::dfs(int depth, QDir *cur) throw(Interrupt){
    if(reset->get())
        throw Interrupt();
    if(searchTime > 0 && duration_cast<milliseconds>(high_resolution_clock::now() - t).count() > searchTime)
        throw Interrupt();

    if(ignorePath(cur->absolutePath()))
        return;
    if(depth <= 0)
        return;
    if(depth == 1){
        QDirIterator files(cur->absolutePath(), QDir::Files);
        resultsLock.lock();
        while(files.hasNext()){
            files.next();
            QString name = files.fileName();
            if(name.toLower().contains(key))
                results->append(name);
        }
        resultsLock.unlock();
    }

    QDirIterator dirs(cur->absolutePath(), QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    while(dirs.hasNext()){
        QDir *ndir = new QDir(dirs.next());
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

void Worker::updateWork(QString k, int searchTime, int maxResults){
    k = k.toLower();

    this->maxResults = maxResults;
    this->searchTime = searchTime;
    t = high_resolution_clock::now();

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
