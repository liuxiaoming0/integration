/*
 * =====================================================================================
 *
 *       Filename:  epollproxy.h
 *
 *    Description:  for CallBackBase.h and MyLibeventBase.h
 *
 *        Version:  1.0
 *        Created:  22/8/2017 16:36:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  danae
 *        Company:  play for fun, China
 *
 * =====================================================================================
 */


#ifndef __EPOLL_PROXY__
#define __EPOLL_PROXY__

#include "./commu/myEpollBase.h"
#include "./commu/myLibeventBase.h"
#include "socketInfoBase.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <pthread.h>

/*
events可以是以下几个宏的集合：
EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
EPOLLOUT：表示对应的文件描述符可以写；
EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
EPOLLERR：表示对应的文件描述符发生错误；
EPOLLHUP：表示对应的文件描述符被挂断；
EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，
          这是相对于水平触发(Level Triggered)来说的。
EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，
          如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里
*/

namespace comm
{
typedef enum EPOLL_TYPES
{
    INTER_EPOLL = 0,
    LIBEVENT_TCP_PROXY,
    LIBEVENT_UDP_PROXY,
    LIBEVENT_HTTP_PROXY,
    LIBEVENT_DNS_PROXY,
}EPOLL_TYPES;

typedef union EPOLL_ROOT
{
    MyEpollBase             *_root_epoll;
    MyLibeventTcpBase       *_root_libevent_tcp;
    MyLibeventUdpBase       *_root_libevent_udp;
    MyLibeventHttpBase      *_root_libevent_http;
    MyLibeventDNSBase       *_root_libevent_dns;
}EPOLL_ROOT;

class EpollProxy
{
public:

    EpollProxy(EPOLL_TYPES type);
    ~EpollProxy();

    int init(int captity, int timeout);
    int add_event(SocketInfoBase *socketinfo, short types);
    int delete_event(SocketInfoBase *socketinfo);
    int mod_event(SocketInfoBase *socketinfo, short types);

    int wait_time();

protected:

private:

    EPOLL_TYPES         type_;
    static Logger       logger;
    EPOLL_ROOT          *root_;
    int                 active_num_;
    pthread_mutex_t     *lock_;

};

}





#endif
