/*
 * =====================================================================================
 *
 *       Filename:  qpid_proton.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/21/21 20:52:17
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef C2AMQP_QPID_PROTON_H_
#define C2AMQP_QPID_PROTON_H_

#include "ts_queue.h"

#include <proton/connection.hpp>
#include <proton/container.hpp>
#include <proton/delivery.hpp>
#include <proton/message.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/source_options.hpp>
#include <proton/tracker.hpp>

#include <string>

class AdptProton : public proton::messaging_handler {
	public:
	using Msg_queue = threadsafe_queue<std::string>;
	private:
	std::string conn_url_;
	std::string addr_;
	proton::sender sender_;
	proton::receiver receiver_;

	Msg_queue& msg_queue_;

	public:
	AdptProton(const std::string& u, const std::string& a, Msg_queue& mq) :
	conn_url_(u), addr_(a), msg_queue_(mq) {  }

	void on_container_start(proton::container& c) override;

	void on_connection_open(proton::connection& c) override;

	void on_sendable(proton::sender& s) override;

	void on_message(proton::delivery &d, proton::message &response) override;

};

#endif
