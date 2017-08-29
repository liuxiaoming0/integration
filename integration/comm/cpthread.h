#ifndef __CPTHREAD_H__
#define __CPTHREAD_H__

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <pthread.h>
#include <iostream>

using namespace log4cplus;

namespace comm {

class CPthread
{
public:
    CPthread(int num, int stackSize);
    virtual ~CPthread();

    int start();
    virtual void run() = 0;
    static Logger logger;

private:
    static void *threadFunc(void *para);

protected:

    int _num; //pthread num;
    int _stackSize; //the size of stack pthread

};

}


#endif // PTHREAD_H
