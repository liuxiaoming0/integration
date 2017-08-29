#include  "myLibeventBase.h"
#include  "event2/buffer.h"
#include  "event2/bufferevent.h"

using namespace comm;
using namespace comm::commu;

Logger MyLibeventTcpBase::logger = Logger::getInstance("myLibeventBase");



MyLibeventTcpBase::MyLibeventTcpBase():rootBase_(NULL),
    fd_info_(NULL),
    captity_(0),
    timeout_(0),
    stop_(0)
{

    rootBase_ = event_base_new();
    if (NULL == rootBase_)
    {
        LOG4CPLUS_ERROR(logger, "libevent create error");
    }

}

MyLibeventTcpBase::~MyLibeventTcpBase()
{
    if(rootBase_ != NULL)
    {
        event_base_loopexit(rootBase_, NULL);
        event_base_free(rootBase_);
    }
}

int MyLibeventTcpBase::init(int captity, int timeout)
{
    captity_ = captity;
    timeout_ = timeout;
    event_base_dispatch(rootBase_);
    return 0;
}

inline int MyLibeventTcpBase::setStop(bool stop)
{
    stop_ = stop;
    return 0;
}

/*

int MyLibeventTcpBase::add_event(int fd,
                                               short types,
                                               int accept_types = EPOLLIN|EPOLLOUT|EPOLLET)
{
    struct evconnlistener *rootListen_ = evconnlistener_new(rootBase_, conn_cb, (void*)this, LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, fd);

    if(!rootListen_)
    {
        evconnlistener_set_error_cb(rootListen_, conn_error_cb);
        return -1;
    }
    listen_fds_->insert(std::pair<int, struct evconnlistener*>(fd, rootListen_));
    return 0;

            struct evconnlistener *listener;
            listener = evconnlistener_new(rootBase_, accept_conn_cb, (void*)this, LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, -1, fd);
            if (!listener)
            {
                LOG4CPLUS_ERROR(logger, "mylibevntTcpBase add_event() error: socket conn new failed.");
                return -1;
            }
            evconnlistener_set_error_cb(listener, accept_conn_error_cb);


}
*/

 /*
void MyLibeventTcpBase::accept_conn_cb(struct evconnlistener *listener,
                                       evutil_socket_t fd,
                                       struct sockaddr *addr,
                                       int len,
                                       void *ptr)
{

    MyLibeventTcpBase *me = static_cast<MyLibeventTcpBase*>(ptr);
    SocketInfo * info = NULL;
    info->fd = fd;
    info->
    SocketInfoTCP *tmp = new SocketInfoTCP(fd, 0);
    me->add_event(tmp, )

}
*/

/*
void MyLibeventTcpBase::accept_conn_error_cb(struct evconnlistener *listener, void *ptr)
{

    MyLibeventTcpBase *me = static_cast<MyLibeventTcpUdpBase*>(ptr);

    int err = EVUTIL_SOCKET_ERROR();
    LOG4CPLUS_ERROR(logger, "libevent create listen error:" << err);

}
*/

int MyLibeventTcpBase::add_event(SocketInfoBase *sockinfo, short types)
{
    CallBackBase *cbBase = NULL;
    SocketInfo *info = NULL;
    int fd = sockinfo->getSocketInfo()->_fd;
    hash_map<int, SocketInfoBase*>::iterator iter;
    iter = fd_info_->find(fd);
    if (iter != fd_info_->end())
    {
        LOG4CPLUS_ERROR(logger, "mylibevntTcpBase add_event() error: socket fd existed");
        return -1;
    }

    if (sockinfo->getSocketTypes() == SOCKET_TYPES_TCP)
    {
        SocketInfoTCP *sockTcpInfo = static_cast<SocketInfoTCP*>(sockinfo);
        evutil_make_socket_nonblocking(fd);
        if (sockTcpInfo->getIsListener())
        {



        }
        else
        {
            struct bufferevent *bev = bufferevent_socket_new(rootBase_, fd, BEV_OPT_CLOSE_ON_FREE);
            cbBase = CallbackFactory::createCallback(EPOLL_CALLBACK_TCP);

//            bufferevent_setcb(bev, cbBase->cb_read_, cbBase->cb_write_, cbBase->cb_error_, NULL);
            bufferevent_setcb(bev, NULL, NULL, NULL, NULL);
            bufferevent_enable(bev, types);
            info->_socket_evbuffer = bev;
        }
        struct event* ev = event_new(rootBase_, fd, types, NULL, NULL);
        event_add(ev, NULL);
        info->_socket_event = ev;
        sockTcpInfo->setSocketInfo(info);
        SocketInfoBase* infoBase = static_cast<SocketInfoBase*>(sockTcpInfo);
        fd_info_->insert(pair<int, SocketInfoBase*>(fd, infoBase));
    }
    else
    {
        LOG4CPLUS_ERROR(logger, "mylibevntTcpBase add_event() error: socket type wrong");
        return -1;
    }

    return 0;

}

int MyLibeventTcpBase::delete_event(SocketInfoBase *sockinfo)
{
    int fd = sockinfo->getSocketInfo()->_fd;
    hash_map<int, SocketInfoBase*>::iterator iter;
    iter = fd_info_->find(fd);
    if (iter == fd_info_->end())
    {
        LOG4CPLUS_ERROR(logger, "mylibevntTcpBase delete_event() error: socket fd not existed");
        return -1;
    }
    struct event* ev = iter->second->getSocketInfo()->_socket_event;
    int ret = event_del(ev);
    if (ret == 0)
    {
        fd_info_->erase(iter);
    }
    else
    {
        LOG4CPLUS_ERROR(logger, "mylibevntTcpBase delete_event() error: event_del() failed.");
        return -1;
    }

    return 0;

}

int MyLibeventTcpBase::mod_event(SocketInfoBase *sockinfo, short types)
{
    //???
    return 0;
}

int MyLibeventTcpBase::wait_time()
{
    while(true)
    {
        event_base_dispatch(rootBase_);
    }

    return 0;
}

