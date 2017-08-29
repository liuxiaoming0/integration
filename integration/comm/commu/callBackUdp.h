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
#ifndef __EPOLL_CALLBACK_UDP_H__
#define __EPOLL_CALLBACK_UDP_H__

#include "callBackBase.h"

namespace comm
{
namespace commu
{
class CallbackUdp: public CallBackBase
{
public:
    CallbackUdp();
    ~CallbackUdp();

    void on_readable(void*);
    void on_writable(void*);
    void on_error(void*);

protected:
private:

};
}

}



#endif
