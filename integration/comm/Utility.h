/*
 * =====================================================================================
 *
 *       Filename:
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  25/8/2017 16:08:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  danae
 *        Company:  play for fun, China
 *
 * =====================================================================================
 */


#ifndef __UTILITY_H_20170828__
#define __UTILITY_H_20170828__

#include <time.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <sys/time.h>
#include <string.h>
#include <string>
#include "socketInfoBase.h"

using namespace log4cplus;
using namespace comm;

//static Logger logger = Logger::getInstance("Utility");

namespace comm {


class Utility{

public:
    static int set_socket_noblock(int fd, bool block);

    static SocketInfo* createSocket(SOCKET_TYPES type);

    static SocketInfo* createConnSocket(int port);

private:
    Utility(){}
    ~Utility(){}
    static  Logger logger;

};

}




#endif // UTILITY_H
