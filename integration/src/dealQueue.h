/*开启n个线程处理cmd，每个cmd放在vector中的Ｑueue中；
  Ｑueue需要是线程安全的
  cmd的类型是基础类CCommand
*/

#ifndef __CDEALQUEUE_H__
#define __CDEALQUEUE_H__

#include "comm/cpthread.h"
#include "cmd/command.h"
#include "comm/safeData.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

using namespace comm;
using namespace log4cplus;

template <class T>
class CDealQueue: public CPthread
{
public:
    CDealQueue(int num, int stackSize, CSafeData<T> *data);
    ~CDealQueue(){}

    void run();

private:
    CSafeData<int> *_data;
    static Logger logger;

};

#endif
