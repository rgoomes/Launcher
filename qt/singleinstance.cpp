#include "singleinstance.h"

#include <QCryptographicHash>

namespace{
    QString generateKeyHash(const QString& key, const QString& salt){
        QByteArray data;

        data.append(key.toUtf8());
        data.append(salt.toUtf8());
        data = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();

        return data;
    }
}

SingleInstance::~SingleInstance(){ release(); }
SingleInstance::SingleInstance(const QString& key) : key(key)
    , memLockKey(generateKeyHash(key, "_memLockKey"))
    , sharedmemKey(generateKeyHash(key, "_sharedmemKey"))
    , sharedMem(sharedmemKey)
    , memLock(memLockKey, 1){

        QSharedMemory fix(sharedmemKey);
        fix.attach();
}

bool SingleInstance::isAnotherRunning(){
    if(sharedMem.isAttached())
        return false;

    memLock.acquire();
    const bool isRunning = sharedMem.attach();
    if(isRunning)
        sharedMem.detach();
    memLock.release();

    return isRunning;
}

bool SingleInstance::tryToRun(){
    if(isAnotherRunning())
        return false;

    memLock.acquire();
    const bool result = sharedMem.create(sizeof(quint64));
    memLock.release();
    if (!result){
        release();
        return false;
    }

    return true;
}

void SingleInstance::release(){
    memLock.acquire();
    if(sharedMem.isAttached())
        sharedMem.detach();
    memLock.release();
}
