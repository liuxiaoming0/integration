#include "safeData.h"
#include "Utility.h"

using namespace comm;

template <class T>
Logger CSafeData<T>::_logger = Logger::getInstance("safeData");

template <class T>
CSafeData <T>::CSafeData(int capacity = DATA_CAPACITY)
{
    _dataCapcity = capacity;
    _dataStart = 0;
    _dataLen = 0;
    pthread_cond_init(&_notEmptyCond, NULL);
    pthread_cond_init(&_notFullCond, NULL);
    pthread_mutex_init(&_mutex, NULL);

    allocateData();
}

template <class T>
CSafeData <T>::~CSafeData()
{
    pthread_cond_destroy(&_notEmptyCond);
    pthread_cond_destroy(&_notFullCond);
    pthread_mutex_destroy(&_mutex);

    freeData();
}

template <class T>
int CSafeData <T>::getDataWait(T &retT, int waitMs)
{
    timespec t;
    t = msToTimespec(waitMs);
    pthread_mutex_lock(&_mutex);
    if (_dataLen < 0)
    {
        LOG4CPLUS_WARN(_logger, "getDataWait error: _dataLen < 0");
        return -1;
    }
    else if( 0 == _dataLen )
    {
        if (waitMs == 0)
        {
            pthread_cond_wait(&_notEmptyCond, &_mutex);
        }
        else
        {
            pthread_cond_timedwait(&_notEmptyCond, &_mutex, &t);
        }
    }

    retT = *(_data + _dataStart);

    if ((_dataStart + 1) <= _dataCapcity)
    {
        _dataStart++;
    }
    else
    {
        _dataStart = 0;
    }

    _dataLen--;

    LOG4CPLUS_WARN(_logger, "CSafeData::getDataWait _dataLen:"
                   << _dataLen << " _dataStart:" << _dataStart);

    pthread_cond_signal(&_notFullCond);
    pthread_mutex_unlock(&_mutex);

    return 0;
}

template <class T>
int CSafeData <T>::getDataVecWait(vector<T> &retVec, int waitMs)
{
    timespec t;
    t = msToTimespec(waitMs);
    pthread_mutex_lock(&_mutex);
    if (_dataLen < 0)
    {
        LOG4CPLUS_WARN(_logger, "getDataWait error: _dataLen < 0");
        return -1;
    }
    else if(_dataLen = 0)
    {
        pthread_cond_timedwait(&_notEmptyCond, &_mutex, &t);
    }

    T temp = *(_data + _dataStart);
    retVec.push_back(temp);

    if ((_dataStart + 1) <= _dataCapcity)
    {
        _dataStart++;
    }
    else
    {
        _dataStart = 0;
    }

    _dataLen--;

    pthread_mutex_unlock(&_mutex);

    return 0;
}

template <class T>
int CSafeData <T>::getDataSetWait(set<T> &retSet, int waitMs)
{
    timespec t;
    t = msToTimespec(waitMs);
    pthread_mutex_lock(&_mutex);
    if (_dataLen < 0)
    {
        LOG4CPLUS_WARN(_logger, "getDataWait error: _dataLen < 0");
        return -1;
    }
    else if(_dataLen = 0)
    {
        pthread_cond_timedwait(&_notEmptyCond, &_mutex, &t);
    }

    T temp = *(_data + _dataStart);
    retSet.insert(temp);

    if ((_dataStart + 1) <= _dataCapcity)
    {
        _dataStart++;
    }
    else
    {
        _dataStart = 0;
    }

    _dataLen--;

    pthread_mutex_unlock(&_mutex);

    return 0;
}

template <class T>
int CSafeData <T>::getDataQueWait(queue<T> &retQueue, int waitMs)
{
    timespec t;
    t = msToTimespec(waitMs);
    pthread_mutex_lock(&_mutex);
    if (_dataLen < 0)
    {
        LOG4CPLUS_WARN(_logger, "getDataWait error: _dataLen < 0");
        return -1;
    }
    else if(_dataLen = 0)
    {
        pthread_cond_timedwait(&_notEmptyCond, &_mutex, &t);
    }

    T temp = *(_data + _dataStart);
    retQueue.push_back(temp);

    if ((_dataStart + 1) <= _dataCapcity)
    {
        _dataStart++;
    }
    else
    {
        _dataStart = 0;
    }

    _dataLen--;

    pthread_mutex_unlock(&_mutex);

    return 0;
}

template <class T>
int CSafeData <T>::getData(T &retT)
{
    getDataWait(retT, 0);
    return 0;
}

template <class T>
int CSafeData <T>::getDataVec(vector<T> &retVec)
{
    return 0;
}

template <class T>
int CSafeData <T>::getDataSet(set<T> &retSet)
{
    return 0;
}

template <class T>
int CSafeData <T>::getDataQue(queue<T> &retQueue)
{
    return 0;
}

template <class T>
int CSafeData <T>::putDataWait(T fromT, int waitMs)
{
    checkData();
    pthread_mutex_lock(&_mutex);

    timespec t = msToTimespec(waitMs);

    if (_dataLen > _dataCapcity)
    {
        LOG4CPLUS_WARN(_logger, "putDataWait error > dataCapcity");
    }
    else if (_dataLen == _dataCapcity)
    {
        if (waitMs == 0)
        {
            pthread_cond_wait(&_notFullCond, &_mutex);
        }
        else
        {
            pthread_cond_timedwait(&_notFullCond, &_mutex, &t);
        }
    }

    int end = (_dataStart + _dataLen) % _dataCapcity;
    *(_data + end) = fromT;
    _dataLen ++;
    pthread_cond_signal(&_notEmptyCond);
    pthread_mutex_unlock(&_mutex);

    return 0;
}

template <class T>
int CSafeData <T>::putDataVecWait(vector<T> fromVec, int waitMs)
{
    return 0;
}

template <class T>
int CSafeData <T>::putDataSetWait(set<T> fromSet, int waitMs)
{
    return 0;
}

template <class T>
int CSafeData <T>::putDataQueWait(queue<T> fromQue, int waitMs)
{
    return 0;
}

template <class T>
int CSafeData <T>::putData(T fromT)
{
    putDataWait(fromT, 0);
    return 0;
}

template <class T>
int CSafeData <T>::putDataVec(vector<T> fromVec)
{
    return 0;
}

template <class T>
int CSafeData <T>::putDataSet(set<T> fromSet)
{
    return 0;
}

template <class T>
int CSafeData <T>::putDataQue(queue<T> fromQue)
{
    return 0;
}

template <class T>
bool CSafeData <T>::checkData()
{
    if (_data == NULL)
    {
        allocateData();
    }
    return true;
}

template <class T>
bool CSafeData <T>::allocateData()
{
    //_data = new <T>(_dataCapcity);
    _data = new T[_dataCapcity];
    return true;
}

template <class T>
bool CSafeData <T>::freeData()
{
    delete []_data;
    return true;
}

template <class T>
timespec CSafeData <T>::msToTimespec(int timeMs)
{
    if (timeMs < 0)
    {
        //LOG4CPLUS_WARN(logger, "msToTimespec error: para < 0?");
        //return reinterpret_cast<timespec>(0);
    }
    timeval now;
    gettimeofday(&now, NULL);
    timespec t;
    t.tv_sec = timeMs / 1000 + now.tv_sec;
    t.tv_nsec = (timeMs % 1000) * 1000 * 1000 + now.tv_usec * 1000;
    return t;
}

