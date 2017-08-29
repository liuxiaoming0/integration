/*
 * =====================================================================================
 *
 *       Filename:
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  22/8/2017 17:15:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  danae
 *        Company:  play for fun, China
 *
 * =====================================================================================
 */

#ifndef __SOCKET_INFO_BASE__
#define __SOCKET_INFO_BASE__


#include    <event2/event.h>
#include    "event2/listener.h"
#include    "event2/http.h"
#include    "event2/util.h"
#include    <string>

namespace comm
{

typedef enum SOCKET_TYPES
{
    SOCKET_TYPES_TCP = 0,
    SOCKET_TYPES_UDP,
    SOCKET_TYPES_HTTP,
}SOCKET_TYPES;

typedef struct SocketInfo
{
    int                 _fd;
    short               _event_types;   //EV_PERSIST....
    struct event        *_socket_event;
    struct bufferevent  *_socket_evbuffer;
    std::string         _ip;
    int                 _port;
}SocketInfo;

class SocketInfoBase
{
public:
    explicit SocketInfoBase(int fd)
    {
        socketinfo_->_fd = fd;
    }

    virtual ~SocketInfoBase(){}

    virtual SOCKET_TYPES getSocketTypes() = 0;

    inline void setSocketInfo(SocketInfo * socketinfo)
    {
        socketinfo_ = socketinfo;
    }

    inline SocketInfo* getSocketInfo()
    {
        return  socketinfo_;
    }

protected:
    SocketInfo *socketinfo_;
private:

};


class SocketInfoTCP : public SocketInfoBase
{
public:
    SocketInfoTCP(int fd, bool is_listener);
    ~SocketInfoTCP(){}

    SOCKET_TYPES getSocketTypes()
    {
        return SOCKET_TYPES_TCP;
    }

    inline bool getIsListener()
    {
        return is_listener_;
    }

private:
    bool    is_listener_;
};


class SocketInfoUDP : public SocketInfoBase
{
public:
    SocketInfoUDP(int fd);
    ~SocketInfoUDP(){}

    inline SOCKET_TYPES getSocketTypes()
    {
        return SOCKET_TYPES_UDP;
    }

private:

};


class SocketInfoHttp : public SocketInfoBase
{
public:
    SocketInfoHttp(int fd, bool is_listener);
    ~SocketInfoHttp(){}

    inline SOCKET_TYPES getSocketTypes()
    {
        return SOCKET_TYPES_HTTP;
    }

    inline bool getIsListener()
    {
        return is_listener_;
    }
private:
    bool    is_listener_;
};

}

#endif
