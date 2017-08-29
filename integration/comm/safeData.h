/*
 * 线程安全的Ｑueue
1) 数据使用模板
2) 存储的数据结构使用两种vector/set
3) 使用智能指针pthread_mutex_t封装
4) test测试单独的一个cpp的功能
5) 使用execption抓异常

*/

#ifndef __SAFEDATA_H_20170828__
#define __SAFEDATA_H_20170828__

#include <exception>
#include <pthread.h>
#include <set>
#include <vector>
#include <queue>
#include <string>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <time.h>

using std::vector;
using std::set;
using std::queue;
using namespace log4cplus;

#define DATA_CAPACITY 8000

namespace comm {

template <class T>
class CSafeData{
public:

    CSafeData(int capacity);
    ~CSafeData();

    int getDataWait(T &retT, int waitMs);
    int getDataVecWait(vector<T> &retVec, int waitMs);
    int getDataSetWait(set<T> &retSet, int waitMs);
    int getDataQueWait(queue<T> &retQueue, int waitMs);

    int getData(T &retT);
    int getDataVec(vector<T> &retVec);
    int getDataSet(set<T> &retSet);
    int getDataQue(queue<T> &retQueue);

    int putDataWait(T fromT, int waitMs);
    int putDataVecWait(vector<T> fromVec, int waitMs);
    int putDataSetWait(set<T> fromSet, int waitMs);
    int putDataQueWait(queue<T> fromQue, int waitMs);

    int putData(T fromT);
    int putDataVec(vector<T> fromVec);
    int putDataSet(set<T> fromSet);
    int putDataQue(queue<T> fromQue);

private:
    bool checkData();
    bool allocateData();
    bool freeData();
    static timespec msToTimespec(int timeMs);

private:
    pthread_mutex_t _mutex;
    pthread_cond_t _notEmptyCond;
    pthread_cond_t _notFullCond;

    T* _data;
    int _dataStart;
    int _dataLen;
    int _dataCapcity;

    static Logger _logger;

};

}

#endif // SAFEDATA_H
