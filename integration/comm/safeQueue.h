/*
 * 线程安全的Ｑueue，队列中的元素是cmd的基类
 *
*/

#ifndef CSAFEQUEUE
#define CSAFEQUEUE

#include <iostream>
#include <queue>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#include "../src/cmd/command.h"


using namespace std;
using namespace cmd;

#define QueueSize 8000

namespace comm {

class CSafeQueue
{
public:
    CSafeQueue(int queueSize);
    ~CSafeQueue();

    void init();
    Cmd* popCmd();
    int pushCmd(Cmd* cmd);

private:


    int _queueMaxSize;
    queue<Cmd*> *_queue;

    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
    int _queueSize;
};





}





#endif
