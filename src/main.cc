/*
 * =====================================================================================
 *
 *       Filename:  main.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/21/21 15:01:09
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "adaptor.h"
#include <pistache/endpoint.h>
#include <pistache/net.h>

int main() {

	Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(7071));
	auto opts = Pistache::Http::Endpoint::options().threads(1);
	Pistache::Http::Endpoint server(addr);
	Pistache::Rest::Router router = makeAdaptorHandler();
	server.init(opts);
	server.setHandler(router.handler());

	server.serve();

}
