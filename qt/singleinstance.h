#ifndef SINGLEINSTANCE_H
#define SINGLEINSTANCE_H

#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>

class SingleInstance{

    public:
        SingleInstance(const QString& key);
        ~SingleInstance();

    public:
        bool isAnotherRunning();
        bool tryToRun();
        void release();

    private:
        const QString key;
        const QString memLockKey;
        const QString sharedmemKey;

        QSharedMemory sharedMem;
        QSystemSemaphore memLock;

        Q_DISABLE_COPY(SingleInstance)
};

#endif // SINGLEINSTANCE_H
