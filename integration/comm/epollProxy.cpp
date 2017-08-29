#include "epollProxy.h"

using namespace comm;
using namespace comm::commu;

Logger EpollProxy::logger = Logger::getInstance("epollProxy");

EpollProxy::EpollProxy(EPOLL_TYPES type):type_(type),root_(NULL),active_num_(0)
{
    switch (type) {

    case INTER_EPOLL:
        root_->_root_epoll = new MyEpollBase();
        break;
    case LIBEVENT_TCP_PROXY:
        root_->_root_libevent_tcp = new MyLibeventTcpBase();
        break;
    case LIBEVENT_UDP_PROXY:
        root_->_root_libevent_http = new MyLibeventHttpBase();
        break;
    case LIBEVENT_DNS_PROXY:
        root_->_root_libevent_dns = new MyLibeventDNSBase();
        break;
    default:
        LOG4CPLUS_ERROR(logger, "Epollproxy() error: types wrong");
        break;
    }

    pthread_mutex_init(lock_, NULL);
}

EpollProxy::~EpollProxy()
{
    delete root_;
    root_ = NULL;
    pthread_mutex_destroy(lock_);
}

int EpollProxy::init(int captity, int timeout)
{
    switch (type_) {

    case INTER_EPOLL:
        root_->_root_epoll->init(captity, timeout);
        break;
    case LIBEVENT_TCP_PROXY:
        root_->_root_libevent_tcp->init(captity, timeout);
        break;
    case LIBEVENT_UDP_PROXY:
        //root_->_root_libevent_http->init(captity, timeout);
        break;
    case LIBEVENT_DNS_PROXY:
        //root_->_root_libevent_dns->init(captity, timeout);
        break;
    default:
        LOG4CPLUS_ERROR(logger, "Epollproxy() error: types wrong");
        return -1;
        break;
    }
    return 0;
}

int EpollProxy::add_event(SocketInfoBase *socketinfo, short types)
{
    int ret;
    switch (type_) {

    case INTER_EPOLL:
        ret = root_->_root_epoll->add_event(socketinfo, types);
        break;
    case LIBEVENT_TCP_PROXY:
        ret = root_->_root_libevent_tcp->add_event(socketinfo, types);
        break;
    case LIBEVENT_UDP_PROXY:
//        ret = root_->_root_libevent_http->add_event(socketinfo, types);
        break;
    case LIBEVENT_DNS_PROXY:
//        ret = root_->_root_libevent_dns->add_event(socketinfo, types);
        break;
    default:
        LOG4CPLUS_ERROR(logger, "Epollproxy() error: types wrong");
        return -1;
        break;
    }

    if(ret == 0)
    {
        pthread_mutex_lock(lock_);
        active_num_ ++;
        pthread_mutex_unlock(lock_);
        return 0;
    }

    return -1;
}

int EpollProxy::delete_event(SocketInfoBase *socketinfo)
{
    int ret;
    switch (type_) {

    case INTER_EPOLL:
        ret = root_->_root_epoll->delete_event(socketinfo);
        break;
    case LIBEVENT_TCP_PROXY:
        ret = root_->_root_libevent_tcp->delete_event(socketinfo);
        break;
    case LIBEVENT_UDP_PROXY:
//        ret = root_->_root_libevent_http->delete_event(socketinfo);
        break;
    case LIBEVENT_DNS_PROXY:
//        ret = root_->_root_libevent_dns->delete_event(socketinfo);
        break;
    default:
        LOG4CPLUS_ERROR(logger, "Epollproxy() error: types wrong");
        return -1;
        break;
    }

    if (ret == 0)
    {
        pthread_mutex_lock(lock_);
        active_num_ --;
        pthread_mutex_unlock(lock_);
    }

    return -1;
}

int EpollProxy::mod_event(SocketInfoBase *socketinfo, short types)
{
    int ret;
    switch (type_) {

    case INTER_EPOLL:
        ret = root_->_root_epoll->mod_event(socketinfo, types);
        break;
    case LIBEVENT_TCP_PROXY:
        ret = root_->_root_libevent_tcp->mod_event(socketinfo, types);
        break;
    case LIBEVENT_UDP_PROXY:
//        ret = root_->_root_libevent_http->mod_event(socketinfo, types);
        break;
    case LIBEVENT_DNS_PROXY:
//        ret = root_->_root_libevent_dns->mod_event(socketinfo, types);
        break;
    default:
        LOG4CPLUS_ERROR(logger, "Epollproxy() error: types wrong");
        return -1;
        break;
    }
    return ret;
}

int EpollProxy::wait_time()
{
    switch (type_) {

    case INTER_EPOLL:
        root_->_root_epoll->wait_time();
        break;
    case LIBEVENT_TCP_PROXY:
        root_->_root_libevent_tcp->wait_time();
        break;
    case LIBEVENT_UDP_PROXY:
//      root_->_root_libevent_http->wait_time();
        break;
    case LIBEVENT_DNS_PROXY:
//      root_->_root_libevent_dns->wait_time();
        break;
    default:
        LOG4CPLUS_ERROR(logger, "Epollproxy() error: types wrong");
        return -1;
        break;
    }

    return 0;
}
