/*
 * 暂时先封装好，避免忘记delete 内存泄露，
   后续改进
*/

#ifndef ALLOCATEMEM_H
#define ALLOCATEMEM_H

namespace comm {

template<T>
class CAllocateMem
{
public:
    CAllocateMem();
    ~CAllocateMem();

private:


};

}

#endif // ALLOCATEMEM_H
