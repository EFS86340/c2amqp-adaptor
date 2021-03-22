/*
 * =====================================================================================
 *
 *       Filename:  adaptor.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/21/21 14:54:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "adaptor.h"
#include <pistache/http.h>

#include <iostream>
#include <pistache/http_defs.h>
#include <pistache/router.h>

class BucketNotificationHandler : public Pistache::Http::Handler {
	public:
	HTTP_PROTOTYPE(BucketNotificationHandler)

	void onRequest(const Pistache::Http::Request &requst,
			Pistache::Http::ResponseWriter response) override {
				std::cout << "[http server got: ]" << requst.body() << std::endl;

				response.send(Pistache::Http::Code::Ok, "hi, ceph!\n");
	}

};

void naive_hi(const Pistache::Rest::Request& requst,
		Pistache::Http::ResponseWriter response) {
	response.send(Pistache::Http::Code::Ok, "hi there\n");
}

void receive_bucket(const Pistache::Rest::Request& request,
		Pistache::Http::ResponseWriter response) {
	auto resp_body = request.body();
	std::cout <<"received: \n" + resp_body << std::endl;
	response.send(Pistache::Http::Code::Ok, "hi notification:\n" + resp_body);

}

Pistache::Rest::Router makeAdaptorHandler(void) {
	Pistache::Rest::Router router;

	Pistache::Rest::Routes::Post(router, "/hi",
	Pistache::Rest::Routes::bind(&naive_hi));

	Pistache::Rest::Routes::Post(router, "/",
	Pistache::Rest::Routes::bind(&receive_bucket));

	return router;
}


