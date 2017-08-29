#include "myEpollBase.h"
#include <pthread.h>

using namespace comm::commu;
using namespace comm;

Logger MyEpollBase::logger = Logger::getInstance("MyepollBase");

MyEpollBase::MyEpollBase(uint32_t size, void* cb):size_(size)

{
    cb_ = static_cast<EVENT_CB*>(cb);
    init();
}

int MyEpollBase::init()
{
    int rootFd = epoll_create(size_);
    if (rootFd == -1)
    {
        LOG4CPLUS_ERROR(logger, "epoll create failed");
        return -1;
    }

    rootFd_ = rootFd;

    return 0;
}

MyEpollBase::~MyEpollBase()
{
    if (rootFd_)
    {
        ::close(rootFd_);
    }
}

int MyEpollBase::add_event_tcp_listen(int fd, int types, int accept_types)
{
    int ret = ctrl_event(EPOLL_CTL_ADD, fd, types);
    if (ret == 0)
    {
        listen_fds_->insert(std::pair<int,int>(fd, accept_types));
        all_fds_->insert(fd);
        return 0;
    }
    return -1;
}

int MyEpollBase::add_event(int fd, int types)
{
    return ctrl_event(EPOLL_CTL_ADD, fd, types);
}

int MyEpollBase::delete_event(int fd, int types)
{
    int ret = ctrl_event(EPOLL_CTL_DEL, fd, types);
    if (ret == 0)
    {
        std::map<int, int>::iterator it;
        it = listen_fds_->find(fd);
        if (it != listen_fds_->end())
        {
            listen_fds_->erase(it);
        }
        std::set<int>::iterator iter;
        iter = all_fds_->find(fd);
        if (iter != all_fds_->end())
        {
            all_fds_->erase(iter);
        }
    }

    return -1;
}

int MyEpollBase::mod_event(int fd, int types)
{
    return ctrl_event(EPOLL_CTL_MOD, fd, types);
}

int MyEpollBase::ctrl_event(int op, int fd, int types)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = types;
    int ret = epoll_ctl(rootFd_, op, fd, &ev);
    if (ret)
    {
        LOG4CPLUS_ERROR(logger, "epoll ctrl event failed");
        return -1;
    }
    return 0;
}

int MyEpollBase::wait_time(int timeout, bool newpthread, int stacksize = 204800 )
{
    timeout_ = timeout;
    if (newpthread)
    {
        pthread_t thread_id;
        pthread_attr_t attr;
        int ret = pthread_attr_init(&attr);
        if (ret != 0)
        {
            LOG4CPLUS_ERROR(logger, "pthread attr init error");
            return -1;
        }

        ret = pthread_attr_setstacksize(&attr, stacksize);
        if (ret != 0)
        {
            LOG4CPLUS_ERROR(logger, "pthread set stacksize error");
            return -1;
        }

        ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        if (ret != 0)
        {
            LOG4CPLUS_ERROR(logger, "pthread set detachstate error");
            return -1;
        }

        ret = pthread_create(&thread_id, &attr, while_get, (void*)this);
        if (ret != 0)
        {
            LOG4CPLUS_ERROR(logger, "pthread create error");
            return -1;
        }

        pthread_attr_destroy(&attr);

        return 0;

    }
    else
    {
        while_get((void*)&timeout);
    }

    return 0;
}

void* MyEpollBase::while_get(void* th)
{
    MyEpollBase* t = static_cast<MyEpollBase*>(th);
    std::map<int, int>::iterator it;
    struct sockaddr_in clientaddr;
    int clientlen = sizeof(clientaddr);
    int fd = 0;
    int nfds = epoll_wait(t->rootFd_, t->ev_, t->size_, t->timeout_);
    for (int i = 0; i < nfds; i++)
    {
        fd = t->ev_[i].data.fd;
        it = t->listen_fds_->find(fd);
        if (it != t->listen_fds_->end())
        {
            /* new connect,must TCP */
            /* set socket  noblocking*/
            int connfd = accept4(fd, (sockaddr*)&clientaddr, (socklen_t*)&clientlen, O_NONBLOCK);
            if (connfd < 0)
            {
                LOG4CPLUS_ERROR(logger, "epoll get new conn failed");
                continue;
            }

            t->add_event(fd, it->second);
        }

        else if(t->ev_[i].events && EPOLLIN)
        {
            t->cb_->cb_read(fd);
        }

        else if(t->ev_[i].events && EPOLLOUT)
        {
            t->cb_->cb_write(fd);
        }
        else
        {
            LOG4CPLUS_ERROR(logger, "epoll wait get wrong msg");
        }
    }

    return NULL;

}

