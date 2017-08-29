#include "safeQueue.h"

using namespace comm;


CSafeQueue::CSafeQueue(int queueSize = QueueSize)
{
    _queueSize = queueSize;
}

CSafeQueue::~CSafeQueue()
{
    pthread_mutex_destroy(&_mutex);
    pthread_cond_destroy(&_cond);
}

Cmd* CSafeQueue::popCmd()
{
    pthread_mutex_lock(&_mutex);
    if (_queueSize > 0)
    {
        pthread_cond_wait(&_cond, &_mutex);
    }
    Cmd* cmd = _queue->front();
    _queue->pop();
    pthread_mutex_unlock(&_mutex);
    return cmd;
}

int CSafeQueue::pushCmd(Cmd* cmd)
{
    pthread_mutex_lock(&_mutex);
    pthread_cond_wait(&_cond, &_mutex);

    _queue->push(cmd);
    pthread_cond_signal(&_cond);
    pthread_mutex_unlock(&_mutex);

    return 0;
}


void CSafeQueue::init()
{
    //_queue = new queue<Cmd*>(_queueMaxSize); //
    pthread_cond_init(&_cond, NULL);
    pthread_mutex_init(&_mutex, NULL);
}


