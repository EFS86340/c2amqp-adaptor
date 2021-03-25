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

#include "qpid_proton.h"

#include <memory>
#include <pistache/endpoint.h>
#include <pistache/router.h>

Pistache::Rest::Router makeAdaptorHandler(void);

class Adaptor {
	private:
	threadsafe_queue<std::string> queue_;

	std::shared_ptr<Pistache::Http::Endpoint> server_ = nullptr;
	std::shared_ptr<AdptProton> proton_ = nullptr;

	public:
	Adaptor() = default;

	void init_and_serve();

};

#endif
