#ifndef CAUTOLOCK_H
#define CAUTOLOCK_H

#include <pthread.h>

namespace comm {

class AutoMutex{

public:
    AutoMutex()
    {
        pthread_mutex_init(&_lock);
    }

    ~AutoMutex()
    {
        pthread_mutex_destroy(&_lock);
    }

    int lock()
    {
        return pthread_mutex_lock(&_lock);
    }

    int unlock()
    {
        return pthread_mutex_unlock(&_lock);
    }

private:
    pthread_mutex_t _lock;
};

}


#endif // CAUTOLOCK_H
