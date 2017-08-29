/*
 * =====================================================================================
 *
 *       Filename:
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  24/8/2017 15:29:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  danae
 *        Company:  play for fun, China
 *
 * =====================================================================================
 */
#include "callBackTCP.h"

using namespace comm::commu;

Logger CallbackTcp::logger = Logger::getInstance("callbackTCP");

void CallbackTcp::on_readable(void*)
{
    LOG4CPLUS_ERROR(logger, "CallbackTcp::on_readable");
}

void CallbackTcp::on_writable(void*)
{
    LOG4CPLUS_ERROR(logger, "CallbackTcp::on_writable");
}

void CallbackTcp::on_error(void*)
{
    LOG4CPLUS_ERROR(logger, "CallbackTcp::on_error");
}
