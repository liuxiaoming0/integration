/*

测试comm/safeData.cpp

*/

#include <iostream>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include "comm/safeData.h"
#include "comm/safeData.cpp"   //template
#include "src/dealQueue.h"
#include "src/dealQueue.cpp"  //template

#include "comm/socketInfoBase.h"
#include "comm/Utility.h"
#include "comm/epollProxy.h"

using namespace log4cplus;
using namespace comm;
using namespace comm::commu;

#define PATH_CONFIG "/home/liuming/server/conf/log4cplus.properties"

static Logger logger = Logger::getInstance("main");


void writeData(CSafeData<int> *data)

{
    for(int i = 0; i < 100; i++)
    {
        data->putData(i);
    }
}

void testSafeData()
{
    int dataCapacity = 200;
    CSafeData<int> *data= new CSafeData<int> (dataCapacity);
    //    CSafeData<char> data(dataCapacity);
    writeData(data);
    CDealQueue<int> *dealQueue = new CDealQueue<int>(10, 102400, data);
    dealQueue->start();
}


void testEpoll()
{
    SocketInfo* info = NULL;
    info = Utility::createConnSocket(8900);//createCcreateSocket(SOCKET_TYPES_TCP);
    Utility::set_socket_noblock(info->_fd, true);
    SocketInfoTCP* infoTcp = new SocketInfoTCP(info->_fd, true);
    EpollProxy * root = new EpollProxy(LIBEVENT_TCP_PROXY);
    root->add_event(infoTcp, EPOLLIN||EPOLLOUT||EPOLLERR||EPOLLET);
    root->init(10000, 3600);
    root->wait_time();

}

int main()
{
    PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(PATH_CONFIG));

    //testSafeData();
    testEpoll();


    while(1);
    return 0;
}


