#include  "myLibeventBase.h"
#include  "event2/buffer.h"
#include  "event2/bufferevent.h"

using namespace comm;
using namespace comm::commu;

Logger MyLibeventTcpBase::logger = Logger::getInstance("myLibeventBase");



MyLibeventTcpBase::MyLibeventTcpBase():rootBase_(NULL),
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
    SocketInfoBase* base = NULL;
    if(rootBase_ != NULL)
    {
        event_base_loopexit(rootBase_, NULL);
        event_base_free(rootBase_);
    }

    //循环删除，iter需要指向下一个位置，否则找不到
    hash_map<int, SocketInfoBase*>::iterator iter;
    for(iter = fd_info_.begin(); iter!= fd_info_.end(); iter++)
    {
        base = iter->second;
        base = NULL;
        fd_info_.erase(iter++);
    }

    fd_info_.clear();
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


void MyLibeventTcpBase::conn_cb(struct evconnlistener *listener,
                                       evutil_socket_t fd,
                                       struct sockaddr *addr,
                                       int len,
                                       void *ptr)
{

    LOG4CPLUS_DEBUG(logger, "conn_cb start. fd:" << fd);
    MyLibeventTcpBase *me = static_cast<MyLibeventTcpBase*>(ptr);
    SocketInfo * info = NULL;
    info->_fd = fd;

}



void MyLibeventTcpBase::conn_error_cb(struct evconnlistener *listener, void *ptr)
{

    MyLibeventTcpBase *me = static_cast<MyLibeventTcpBase*>(ptr);

    int err = EVUTIL_SOCKET_ERROR();
    LOG4CPLUS_ERROR(logger, "libevent create listen error:" << err);

}


int MyLibeventTcpBase::add_event(SocketInfoBase *sockinfo, short types)
{
    LOG4CPLUS_DEBUG(logger, "mylibeventTcpBase add_event() start.fd= " << sockinfo->getSocketInfo()._fd);
    CallBackBase *cbBase = NULL;
    SocketInfo info;
    int fd = sockinfo->getSocketInfo()._fd;
    hash_map<int, SocketInfoBase*>::iterator iter;
    iter = fd_info_.find(fd);
    if (iter != fd_info_.end())
    {
        LOG4CPLUS_ERROR(logger, "mylibeventTcpBase add_event() error: socket fd existed");
        return -1;
    }

    if (sockinfo->getSocketTypes() == SOCKET_TYPES_TCP)
    {
        LOG4CPLUS_DEBUG(logger, "add_event() tcp start ");
        SocketInfoTCP *sockTcpInfo = static_cast<SocketInfoTCP*>(sockinfo);
        evutil_make_socket_nonblocking(fd);
        if (sockTcpInfo->getIsListener())
        {
            LOG4CPLUS_DEBUG(logger, "evconnlistener_new() start.");
            struct evconnlistener *rootListen_ = evconnlistener_new(rootBase_,
                                                                    conn_cb,
                                                                    (void*)this,
                                                                    LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,
                                                                    -1,
                                                                    fd);

            if(!rootListen_)
            {
                LOG4CPLUS_ERROR(logger, "evconnlistener_new() error.");
                evconnlistener_set_error_cb(rootListen_, conn_error_cb);
                return -1;
            }

        }
        else
        {
            struct bufferevent *bev = bufferevent_socket_new(rootBase_, fd, BEV_OPT_CLOSE_ON_FREE);
            cbBase = CallbackFactory::createCallback(EPOLL_CALLBACK_TCP);

            //bufferevent_setcb(bev, cbBase->cb_read_, cbBase->cb_write_, cbBase->cb_error_, NULL);
            bufferevent_enable(bev, types);
            info._socket_evbuffer = bev;
        }
        LOG4CPLUS_DEBUG(logger, "mylibeventTcpBase add_event() insert hash map start");
        struct event* ev = event_new(rootBase_, fd, types, NULL, NULL);
        event_add(ev, NULL);
        info._socket_event = ev;
        sockTcpInfo->setSocketInfo(info);
        SocketInfoBase* infoBase = static_cast<SocketInfoBase*>(sockTcpInfo);
        fd_info_.insert(pair<int, SocketInfoBase*>(fd, infoBase));
        LOG4CPLUS_DEBUG(logger, "mylibevntTcpBase add_event() insert hash map end");
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
    int fd = sockinfo->getSocketInfo()._fd;
    hash_map<int, SocketInfoBase*>::iterator iter;
    iter = fd_info_.find(fd);
    if (iter == fd_info_.end())
    {
        LOG4CPLUS_ERROR(logger, "mylibevntTcpBase delete_event() error: socket fd not existed");
        return -1;
    }
    struct event* ev = iter->second->getSocketInfo()._socket_event;
    int ret = event_del(ev);
    if (ret == 0)
    {
        fd_info_.erase(iter);
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

