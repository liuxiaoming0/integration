/*
 * =====================================================================================
 *
 *       Filename:  callbackBase.h
 *
 *    Description:  for CallBackBase.h and MyLibeventBase.h
 *
 *        Version:  1.0
 *        Created:  22/8/2017 16:34:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  danae
 *        Company:  play for fun, China
 *
 * =====================================================================================
 */

#ifndef __COMMU_CALLBACKBASE__
#define __COMMU_CALLBACKBASE__



namespace comm
{

namespace commu {

class CallBackBase;

typedef void (CallBackBase::*cb_readable)(void*);
typedef void (CallBackBase::*cb_writable)(void*);
typedef void (CallBackBase::*cb_error)(void*);


class CallBackBase
{

public:
    CallBackBase()
    {
        cb_read_ = &CallBackBase::on_readable;
        cb_write_ = &CallBackBase::on_writable;
        cb_error_ = &CallBackBase::on_error;
    }

    virtual ~CallBackBase()
    {

    }

    virtual void on_readable(void*) = 0;
    virtual void on_writable(void*) = 0;
    virtual void on_error(void*) = 0;

public:
    cb_readable         cb_read_;
    cb_readable         cb_write_;
    cb_writable         cb_error_;

protected:

private:

};
}
}



#endif
