#include "callBackFactory.h"

#include "callBackHttp.h"
#include "callBackTCP.h"
#include "callBackUdp.h"



Logger CallbackFactory::logger = Logger::getInstance("callBackFactory");



CallBackBase* CallbackFactory::createCallback(EPOLL_CALLBACK_TYPES types)
{
    CallBackBase *cb = NULL;
    switch (types) {
    case EPOLL_CALLBACK_TCP:
        cb = new CallbackTcp();
        break;
    case EPOLL_CALLBACK_HTTP:
        //cb = new CallbackHttp();
        break;
    case EPOLL_CALLBACK_UDP:
        //cb = new CallbackUdp();
        break;
    default:
        break;
    }
    if(cb != NULL)
    {
        return cb;
    }
    else
    {
        LOG4CPLUS_ERROR(logger, "createCallback error: case not exsist.");
        return NULL;
    }
}
