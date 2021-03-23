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
#include <memory>
#include <thread>
#include <pistache/endpoint.h>
#include <pistache/http.h>

#include <iostream>
#include <pistache/http_defs.h>
#include <pistache/net.h>
#include <pistache/router.h>
#include <proton/container.hpp>

class BucketNotificationHandler : public Pistache::Http::Handler {
	public:
	HTTP_PROTOTYPE(BucketNotificationHandler)

	using Msg_queue = threadsafe_queue<std::string>;

	BucketNotificationHandler(Msg_queue& q) : msg_queue_(q) { }

	void onRequest(const Pistache::Http::Request &requst,
			Pistache::Http::ResponseWriter response) override {
				std::cout << "[http server got: ]" << requst.body() << std::endl;

				msg_queue_.push(requst.body());
				response.send(Pistache::Http::Code::Ok, "hi, ceph!\n");
	}

	private:
	Msg_queue& msg_queue_;

};

// void receive_bucket(const Pistache::Rest::Request& request,
// 		Pistache::Http::ResponseWriter response) {
// 	auto resp_body = request.body();
// 	std::cout <<"received: \n" + resp_body << std::endl;
// 	response.send(Pistache::Http::Code::Ok, "hi notification:\n" + resp_body);
// 
// }
// 
// Pistache::Rest::Router makeAdaptorHandler(void) {
// 	Pistache::Rest::Router router;
// 
// 	Pistache::Rest::Routes::Post(router, "/",
// 	Pistache::Rest::Routes::bind(&receive_bucket));
// 
// 	return router;
// }


void Adaptor::init_and_serve() {
	Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(7074));
	auto opts = Pistache::Http::Endpoint::options().threads(1);
	server_ = std::make_shared<Pistache::Http::Endpoint>(addr);
	// Pistache::Http::Endpoint server(addr);
	// Pistache::Rest::Router router = makeAdaptorHandler();

	server_->init(opts);
	server_->setHandler(Pistache::Http::make_handler<BucketNotificationHandler>(this->queue_));
	// server_->serve();

	std::thread t(&Pistache::Http::Endpoint::serve, this->server_);
	t.detach();

	std::string proton_url("127.0.0.1:5672");
	std::string proton_addr("examples");
	proton_ = std::make_shared<AdptProton>(proton_url, proton_addr, this->queue_);
	proton::container(*proton_).run();

}

