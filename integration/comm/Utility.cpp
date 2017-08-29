#include "Utility.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

using namespace comm;

Logger Utility::logger = Logger::getInstance("Utility");

int Utility::set_socket_noblock(int fd, bool block)
{
    int flags = fcntl(fd, F_GETFL, 0);
    int ret  = 0;
    if (flags == -1)
    {
        LOG4CPLUS_ERROR(logger, "utility setnoblock failed.");
        return -1;
    }
    if (block)
    {
        ret = fcntl(fd, F_SETFL, flags&~O_NONBLOCK);
    }
    else
    {
        ret = fcntl(fd, F_SETFL, flags|O_NONBLOCK);

    }
    if (ret == -1)
    {
        LOG4CPLUS_ERROR(logger, "utility setnoblock failed.");
        return -1;
    }

    return 0;
}

SocketInfo* Utility::createSocket(SOCKET_TYPES type)
{
    int fd = 0;
    switch(type)
    {
    case SOCKET_TYPES_TCP:
        fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        break;
    case SOCKET_TYPES_UDP:
        fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        break;
    default:
        break;
    }
    if (fd != 0)
    {

        SocketInfo *info = NULL;
        info->_fd = fd;
        return info;
    }
    else
    {
        LOG4CPLUS_ERROR(logger, "utility createSocket create failed.");
        return NULL;
    }
}

SocketInfo* Utility::createConnSocket(int port)
{
    struct sockaddr_in addr;
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    bind(fd, (sockaddr*)&addr, sizeof(addr));

    if (fd != 0)
    {
        SocketInfo *info = NULL;
        info->_fd = fd;
        info->_port = port;
        return info;
    }
    else
    {
        LOG4CPLUS_ERROR(logger, "utility createConnSocket create failed.");
        return NULL;
    }

}
