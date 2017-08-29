/*

测试log4cplus的使用方法

*/

#include <iostream>
#include <log4cplus/configurator.h>
#include <log4cplus/logger.h>
#include <log4cplus/layout.h>
#include <log4cplus/loggingmacros.h>

using namespace log4cplus;
using namespace log4cplus::helpers;

#define PATH_CONFIG "/home/liuming/server/conf/log4cplus.properties"


using namespace std;

static log4cplus::Logger _logger = log4cplus::Logger::getInstance("main");


int read_conf()
{
    PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(PATH_CONFIG));
    return 0;
}

int main()
{
    read_conf();

    return 0;
}

