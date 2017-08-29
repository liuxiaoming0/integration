#include "socketInfoBase.h"

using namespace comm;

SocketInfoTCP::SocketInfoTCP(int fd, bool is_listener):SocketInfoBase(fd)
{
    is_listener_ = is_listener;
}


SocketInfoHttp::SocketInfoHttp(int fd, bool is_listener):SocketInfoBase(fd)
{
    is_listener_ = is_listener;
}
