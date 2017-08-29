/*
 * =====================================================================================
 *
 *       Filename:  MyEpollBase.h
 *
 *    Description:  package of epoll
 *
 *        Version:  1.0
 *        Created:  09/8/2017 10:08:24 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  danae
 *        Company:  play for fun, China
 *
 * =====================================================================================
 */

#ifndef __MYEPOLLBASE__
#define __MYEPOLLBASE__

#include <stdio.h>
#include <sys/epoll.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <set>
#include <map>
#include "callBackBase.h"

using namespace log4cplus;
using namespace comm;

namespace comm
{
namespace commu
{


//init(int captity, CallBackBase *cb, int timeout);
//int add_event(SocketInfoBase fd, short types);
//int delete_event(SocketInfoBase fd);
//int mod_event(SocketInfoBase fd, short types);

//int wait_time();


class MyEpollBase
{
public:
    MyEpollBase(uint32_t size, void* cb);
    ~MyEpollBase();

    /* only TCP */
    int add_event_tcp_listen(int fd, int types, int accept_types = EPOLLIN|EPOLLOUT|EPOLLET);

    int add_event(int fd, int types);
    int delete_event(int fd, int types);
    int mod_event(int fd, int types);
    int wait_time(int timeout, bool newpthread, int stacksize);

private:
    int init();
    int ctrl_event(int op, int fd, int types);
    static void* while_get(void* timeout);


private:
    uint32_t        size_;
    int             rootFd_;
    static          Logger logger;
    epoll_event     *ev_;
    std::map<int, int>   *listen_fds_;/*<fd,监听这个fd的新连接的监听方式>*/
    std::set<int>   *all_fds_;
    EVENT_CB        *cb_;
    int             timeout_;
    //CComuObject commuObject_;*/

};


}
}


#endif


/*
typedef union epoll_data {
    void *ptr;
    int fd;
    __uint32_t u32;
    __uint64_t u64;
} epoll_data_t;

struct epoll_event {
    __uint32_t events;
    epoll_data_t data;
};

*/
