/*
 * =====================================================================================
 *
 *       Filename:  adaptor.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/21/21 14:53:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef C2AMQP_ADAPTOR_H_
#define C2AMQP_ADAPTOR_H_

#include <pistache/endpoint.h>
#include <pistache/router.h>

Pistache::Rest::Router makeAdaptorHandler(void);

#endif
