#include "cpthread.h"

#include <exception>

using namespace comm;
using std::exception;

Logger CPthread::logger = Logger::getInstance("CPthread");

CPthread::CPthread(int num, int stackSize = 20480)
{
    if (num > 0 && num < 100 )
    {
        _num = num;
        _stackSize = stackSize;
    }
    else
    {
        LOG4CPLUS_ERROR(logger, "CPthread num is error");
    }
}


CPthread::~CPthread()
{
    _num = 0;
    _stackSize = 0;
}

int CPthread::start()
{
    int ret = 0;

    pthread_t thread_id;
    pthread_attr_t attr;
    ret = pthread_attr_init(&attr);
    if (ret != 0)
    {
        LOG4CPLUS_ERROR(logger, "pthread attr init error");
        return -1;
    }

    ret = pthread_attr_setstacksize(&attr, _stackSize);
    if (ret != 0)
    {
        LOG4CPLUS_ERROR(logger, "pthread set stacksize error");
        return -1;
    }

    ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (ret != 0)
    {
        LOG4CPLUS_ERROR(logger, "pthread set detachstate error");
        return -1;
    }

    for (int i = 0; i < _num ;i++)
    {
        ret = pthread_create(&thread_id, &attr, threadFunc, (void*)this);
        if (ret != 0)
        {
            LOG4CPLUS_ERROR(logger, "pthread create error");
            return -1;
        }
        //pthread_detach(pthread_self());
    }

    pthread_attr_destroy(&attr);
    return 0;
}


void* CPthread::threadFunc(void *para)
{
    CPthread* me = static_cast<CPthread*>(para);
    if (me != NULL)
    {
        pthread_t tid = pthread_self();
        //LOG4CPLUS_DEBUG(logger, "child of CPthread run, childThreadID = " << tid);
        //LOG4CPLUS_INFO(logger, "CPthread starting...");
        try
        {
            me->run();
        }
        catch (std::exception &e)
        {
            LOG4CPLUS_ERROR(logger, "CPthread run excepthion:" << e.what());
        }
        catch (...)
        {
            LOG4CPLUS_ERROR(logger, "CPthread run excepthion Unknown");
        }

    }
    else
    {
        LOG4CPLUS_ERROR(logger, "CPthread threadFunc error");
    }

    return NULL;
}


