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

#ifndef __MYEPOLL_BASE__00__
#define __MYEPOLL_BASE__00__

#include <stdio.h>
#include <sys/epoll.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <hash_map>
#include "callBackBase.h"
#include "../socketInfoBase.h"

using namespace log4cplus;
using namespace comm;
using namespace __gnu_cxx;

namespace comm
{
namespace commu
{

//class SocketInfoBase;

class MyEpollBase
{
public:
    MyEpollBase();
    ~MyEpollBase();

    int init(int captity, int timeout);

    int add_event(SocketInfoBase *socketinfo, short types);
    int delete_event(SocketInfoBase *socketinfo);
    int mod_event(SocketInfoBase *socketinfo, short types);

    void wait_time();
    int setStop(bool stop);

private:
    int ctrl_event(int op, SocketInfoBase *socketinfo, short types);


private:
    int             capacity_;
    int             root_epoll_;
    static Logger   logger;
    epoll_event     *ev_;
    int             timeout_;   //manager管理移除很久没有事件的fd
    bool            isStop_;
    hash_map<int, SocketInfoBase*> fd_info_;

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
