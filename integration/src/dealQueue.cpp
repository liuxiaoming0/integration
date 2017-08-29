#include "dealQueue.h"

template <class T>
Logger CDealQueue <T>::logger = Logger::getInstance("dealQueue");

template <class T>
CDealQueue <T>::CDealQueue(int num, int stackSize, CSafeData<T> *data):CPthread(num, stackSize)
{
    _data = data;
}

template <class T>
void CDealQueue <T>::run()
{
    LOG4CPLUS_INFO(logger, "starting...");
    int a = 0;
    while (1)
    {
        _data->getData(a);
        LOG4CPLUS_INFO(logger, "CDealQueue get a element:" << a);
    }

}








