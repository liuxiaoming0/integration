#include "myEpollBase.h"
#include "callBackFactory.h"

using namespace comm;
using namespace comm::commu;

//uint32_t        captity_;
//int             root_epoll_;
//static Logger   logger;
//epoll_event     *ev_;
//int             timeout_;
//CallBackBase    *cb_;

Logger MyEpollBase::logger = Logger::getInstance("MyEpollBase");


MyEpollBase::MyEpollBase():capacity_(0),
    root_epoll_(0),
    ev_(NULL),
    timeout_(0),
    isStop_(0),
    fd_info_(NULL)

{

}

MyEpollBase::~MyEpollBase()
{
    if (root_epoll_)
    {
        ::close(root_epoll_);
    }
}

int MyEpollBase::init(int capacity, int timeout)
{
    capacity_ = capacity;
    timeout_ = timeout;

    int root_epoll = epoll_create(capacity_);
    if (root_epoll == -1)
    {
        LOG4CPLUS_ERROR(logger, "epoll create failed");
        return -1;
    }

    root_epoll_ = root_epoll;

    return 0;
}

int MyEpollBase::setStop(bool stop)
{
    isStop_ = stop;
    return 0;
}

int MyEpollBase::add_event(SocketInfoBase *socketinfo, short types)
{
    int fd  = socketinfo->getSocketInfo()->_fd;
    hash_map<int, SocketInfoBase*>::iterator iter;
    iter = fd_info_->find(fd);
    if (iter != fd_info_->end())
    {
        fd_info_->insert(pair<int, SocketInfoBase*>(fd, socketinfo));
    }
    else
    {
        LOG4CPLUS_ERROR(logger, "myEpollBase add_event failed. fd alreay in epoll");
        return -1;
    }

    return  ctrl_event(EPOLL_CTL_ADD, socketinfo, types);
}

int MyEpollBase::delete_event(SocketInfoBase *socketinfo)
{
    int fd  = socketinfo->getSocketInfo()->_fd;
    hash_map<int, SocketInfoBase*>::iterator iter;
    iter = fd_info_->find(fd);
    if (iter != fd_info_->end())
    {
        LOG4CPLUS_ERROR(logger, "myEpollBase delete_event failed. fd not in epoll");
        return -1;
    }
    else
    {
        fd_info_->erase(iter);
    }
    return ctrl_event(EPOLL_CTL_DEL, socketinfo, 0);
}

int MyEpollBase::mod_event(SocketInfoBase *socketinfo, short types)
{
    return ctrl_event(EPOLL_CTL_MOD, socketinfo, types);
}

int MyEpollBase::ctrl_event(int op, SocketInfoBase *socketinfo, short types)
{
    struct epoll_event ev;
    int fd = socketinfo->getSocketInfo()->_fd;
    ev.data.fd = fd;
    ev.events = types;
    int ret = epoll_ctl(root_epoll_, op, fd, &ev);
    if (ret)
    {
        LOG4CPLUS_ERROR(logger, "epoll ctrl event failed");
        return -1;
    }
    return 0;
}

void MyEpollBase::wait_time()
{
    int fd = 0;
    SocketInfoBase* socket_info;
    while(!isStop_)
    {
        int nfds = epoll_wait(root_epoll_, ev_, capacity_, -1);
        if (nfds  == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                LOG4CPLUS_ERROR(logger, "myepollbase wait_time() error");
                break;
            }
        }
        else
        {
            for(int i = 0; i <nfds; i++)
            {
                fd = ev_[i].data.fd;
                hash_map<int, SocketInfoBase*>::iterator iter;
                iter = fd_info_->find(fd);
                if (iter == fd_info_->end())
                {
                    LOG4CPLUS_ERROR(logger, "time_wait() error:fd not find.");
                    continue;
                }
                else
                {
                    socket_info = iter->second;
                }

                if (socket_info->getSocketTypes() == SOCKET_TYPES_TCP)
                {
                    if(ev_[i].events == EPOLLIN)
                    {
                        CallbackFactory::createCallback(EPOLL_CALLBACK_TCP)->on_readable(iter->second);
                    }
                    else if (ev_[i].events == EPOLLOUT)
                    {
                        CallbackFactory::createCallback(EPOLL_CALLBACK_TCP)->on_writable(iter->second);
                    }

                    else if(ev_[i].events == EPOLLOUT||EPOLLHUP)
                    {
                        CallbackFactory::createCallback(EPOLL_CALLBACK_TCP)->on_error(iter->second);
                    }
                    else
                    {
                        LOG4CPLUS_ERROR(logger, "tcp:timewait() other event");
                        continue;
                    }
                }
                else if (socket_info->getSocketTypes() == SOCKET_TYPES_UDP)
                {
                    if(ev_[i].events == EPOLLIN)
                    {
                        CallbackFactory::createCallback(EPOLL_CALLBACK_UDP)->on_readable(iter->second);
                    }
                    else if (ev_[i].events == EPOLLOUT)
                    {
                        CallbackFactory::createCallback(EPOLL_CALLBACK_UDP)->on_writable(iter->second);
                    }

                    else if(ev_[i].events == EPOLLOUT||EPOLLHUP)
                    {
                        CallbackFactory::createCallback(EPOLL_CALLBACK_UDP)->on_error(iter->second);
                    }
                    else
                    {
                        LOG4CPLUS_ERROR(logger, "udp:timewait() other event");
                        continue;
                    }
                }
                else if (socket_info->getSocketTypes() == SOCKET_TYPES_HTTP)
                {
                    if(ev_[i].events == EPOLLIN)
                    {
                        CallbackFactory::createCallback(EPOLL_CALLBACK_HTTP)->on_readable(iter->second);
                    }
                    else if (ev_[i].events == EPOLLOUT)
                    {
                        CallbackFactory::createCallback(EPOLL_CALLBACK_HTTP)->on_writable(iter->second);
                    }

                    else if(ev_[i].events == EPOLLOUT||EPOLLHUP)
                    {
                        CallbackFactory::createCallback(EPOLL_CALLBACK_HTTP)->on_error(iter->second);
                    }
                    else
                    {
                        LOG4CPLUS_ERROR(logger, "http:timewait() other event");
                        continue;
                    }
                }
                else
                {
                    LOG4CPLUS_ERROR(logger, "time_wait() error: socket type error");
                    continue;
                }

            }
        }


    }

}



