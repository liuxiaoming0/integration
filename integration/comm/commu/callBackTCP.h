/*
 * =====================================================================================
 *
 *       Filename:
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  24/8/2017 15:29:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  danae
 *        Company:  play for fun, China
 *
 * =====================================================================================
 */


#ifndef __EPOLL_CALLBACK_TCP_H__
#define __EPOLL_CALLBACK_TCP_H__

#include "callBackBase.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

using namespace log4cplus;

namespace comm
{
namespace commu
{
class CallbackTcp: public CallBackBase
{
public:
    CallbackTcp(){}
    ~CallbackTcp(){}

    void on_readable(void*);
    void on_writable(void*);
    void on_error(void*);

protected:
private:
    static Logger logger;

};
}

}



#endif
