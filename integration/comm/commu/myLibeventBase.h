/*
 * =====================================================================================
 *
 *       Filename:  MyLibeventBase.h
 *
 *    Description:  package of libevent
 *
 *        Version:  1.0
 *        Created:  11/8/2017 14:05:24 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  danae
 *        Company:  play for fun, China
 *
 * =====================================================================================
 */

#ifndef LIBEVENTBASE_H
#define LIBEVENTBASE_H

#include    <log4cplus/logger.h>
#include    <log4cplus/loggingmacros.h>
#include    <map>
#include    <set>
#include    <hash_map>
#include    <event2/event.h>
#include    "event2/listener.h"
#include    "event2/http.h"
#include    "event2/util.h"
#include    "callBackBase.h"
#include    "../socketInfoBase.h"
#include    "callBackFactory.h"


using namespace log4cplus;
using namespace __gnu_cxx;

namespace comm
{
namespace commu
{

class MyLibeventTcpBase
{
public:
    MyLibeventTcpBase();
    ~MyLibeventTcpBase();

    int init(int captity, int timeout);
    int add_event(SocketInfoBase *sockinfo, short types);
    int delete_event(SocketInfoBase *sockinfo);
    int mod_event(SocketInfoBase *sockinfo, short types);
    int wait_time();
    inline int setStop(bool stop);

private:
    int free();

    /*static void accept_conn_cb(struct evconnlistener *listener,
                               evutil_socket_t fd,
                               struct sockaddr *addr,
                               int len,
                               void *ptr);
    static void accept_conn_error_cb(struct evconnlistener *listener, void *ptr);*/

private:
    /*
    关于libevent-tcp-callback的设计选型
    tcp比udp复杂，
    方法一：需要为每一个连接fd【监听端口】都创建一套自己的callback；
    第二种方法：在callback里面识别连接fd，进行分类
    第三种方法：都是相同的callback，然后msg是不同的；
    一般都是第三种方法处理，但是我觉得这个地方也可以做一下分流，前提是查找fd的这个要很快，不能出现瓶颈。
    */
    event_base                      *rootBase_;
    static  Logger                  logger;
    hash_map<int, SocketInfoBase*>  *fd_info_;
    int                             captity_;
    int                             timeout_;
    int                             stop_;
};

class MyLibeventUdpBase
{
public:
    MyLibeventUdpBase(){}
    ~MyLibeventUdpBase(){}

//    int init(int captity, int timeout){return 0;}
//    int add_event(SocketInfoBase *sockinfo, short types){return 0;}
//    int delete_event(SocketInfoBase *sockinfo){return 0;}
//    int mod_event(SocketInfoBase *sockinfo, short types){return 0;}
//    int wait_time(){}

private:
    //int free();


private:
    event_base                      *rootBase_;
    static  Logger                  logger_tcp_;
    hash_map<int, SocketInfoBase*>  *fd_info_;


    std::map<int, struct event*>    *add_fds_;
    std::map<int, struct bufferevent*> *fds_buffer_;
};

class MyLibeventHttpBase
{
public:
    MyLibeventHttpBase(){}
    ~MyLibeventHttpBase(){}

    int init(){return 0;}
    int addPortListen(int port){return 0;}
    int delPortListen(int port){return 0;}
    int start_dispath(){return 0;}

private:

    static Logger       logger_http;
    struct event_base   *rootBase_;
    struct evhttp       *rootHttp_;

    //std::map<int, HTTP_PORT_INFO*>   *port_info_;

};

class MyLibeventDNSBase
{
public:
    MyLibeventDNSBase(){}
    ~MyLibeventDNSBase(){}

private:
};

class MyLibeventRPCBase
{
public:
    MyLibeventRPCBase(){}
    ~MyLibeventRPCBase(){}
private:
};

}

}


#endif // LIBEVENTBASE_H
