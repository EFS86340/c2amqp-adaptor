/*
 * =====================================================================================
 *
 *       Filename:  qpid_proton.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/22/21 16:19:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "qpid_proton.h"
#include <proton/delivery.hpp>
#include <proton/message.hpp>

#include <iostream>
#include <proton/tracker.hpp>

void AdptProton::on_container_start(proton::container& c) {
	sender_ = c.open_sender(addr_);
}

void AdptProton::on_connection_open(proton::connection& c) {
	// c.open_receiver(addr_);
}

void AdptProton::on_sendable(proton::sender& s) {
	std::string msg;
	std::cout << "[proton try to send]" << std::endl;
	// for(;;) {
		msg_queue_.wait_and_pop(msg);
		std::cout << "[proton ready to send]" << std::endl;
		proton::message req(msg);
		s.send(req);
	 // }
	// s.close();
}

void AdptProton::on_tracker_accept(proton::tracker& t) {
	std::cout << "[proton tracker accepted.]\n";
}

// void AdptProton::on_message(proton::delivery& d, proton::message& m) {
// 	std::cout << "[proton receiver]: " << m.body() << std::endl;
// 	d.connection().close();
// }
