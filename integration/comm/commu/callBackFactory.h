/*
 * =====================================================================================
 *
 *       Filename:
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  24/8/2017 15:07:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  danae
 *        Company:  play for fun, China
 *
 * =====================================================================================
 */


#ifndef __CALLBACK_FACTORY_H_20170828__
#define __CALLBACK_FACTORY_H_20170828__

#include "callBackBase.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

using namespace comm;
using namespace comm::commu;
using namespace log4cplus;


namespace comm {

namespace commu {

//#define CallbackFactory::EPOLL_CALLBACK_TCP CallbackFactory::createCallback(EPOLL_CALLBACK_TCP)
//#define CallbackFactory::EPOLL_CALLBACK_UDP CallbackFactory::createCallback(EPOLL_CALLBACK_UDP)
//#define CallbackFactory::EPOLL_CALLBACK_HTTP CallbackFactory::createCallback(EPOLL_CALLBACK_HTTP)


typedef enum EPOLL_CALLBACK_TYPES
{
    EPOLL_CALLBACK_TCP = 0,
    EPOLL_CALLBACK_UDP,
    EPOLL_CALLBACK_HTTP,

}EPOLL_CALLBACK_TYPES;

class CallbackFactory
{
public:
    CallbackFactory(){}
    ~CallbackFactory(){}

    static CallBackBase* createCallback(EPOLL_CALLBACK_TYPES types);

protected:
private:
    static Logger  logger;

};

}

}


#endif
