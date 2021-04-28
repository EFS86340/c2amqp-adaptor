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

#include <condition_variable>
#include <iostream>
#include <memory>
#include <proton/connection.hpp>
#include <proton/connection_options.hpp>
#include <proton/container.hpp>
#include <proton/delivery.hpp>
#include <proton/message.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/work_queue.hpp>
#include <proton/source_options.hpp>
#include <proton/tracker.hpp>

#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include <unordered_map>

static std::mutex PRINT_LOCK;
#define PRINT(x) do { std::lock_guard<std::mutex> l(PRINT_LOCK); x; } while(false)

static const int PROTON_STATUS_NO_SUCH_CONNECTION = 1;
static const int PROTON_STATUS_TIMEOUT = 2;

// class for AMQP connection
class Connection : public proton::messaging_handler {
	public:
		Connection(proton::container& cont, const std::string& url, const
				std::string& addr) : conn_url_(url), addr_(addr), work_queue_(0) {
			cont.open_sender(addr, proton::connection_options().handler(*this));
			std::cout << "[debug] Connection ctor open sender done." << std::endl;
		};

	private:
		std::string conn_url_;
		std::string addr_;
		proton::sender sender_;

		proton::work_queue* work_queue_;
		std::mutex lock_;
		std::condition_variable sender_ready_;

		int queued_;
		bool marked_for_deletion_;

		// TODO
		// callbacks

	public:
		void send(const proton::message& m) {
			{
				std::unique_lock<std::mutex> lk(lock_);
				while(!work_queue_) sender_ready_.wait(lk);
				++queued_;
			}
			std::cout << "[info] connection ready to send" << std::endl;
			work_queue_->add([=](){ this->do_send(m); });
		}

		void close() {
			work_queue_->add([=]() { sender_.connection().close(); });
		}

	private:
		proton::work_queue* work_queue() {
			std::unique_lock<std::mutex> lk(lock_);
			while(!work_queue_) sender_ready_.wait(lk);
			return work_queue_;
		}

		// BUG this function is never called 
		// solved by adding connection_options with this pointer
		void on_sender_open(proton::sender& s) override {
			std::lock_guard<std::mutex> lk(lock_);
			sender_ = s;
			work_queue_ = &s.work_queue();
			std::cout << "[info] sender open now" << std::endl;
		}

		void on_sendable(proton::sender& s) override {
			std::lock_guard<std::mutex> lk(lock_);
			sender_ready_.notify_all();
		}

		void do_send(const proton::message& m) {
			sender_.send(m);
			std::lock_guard<std::mutex> lk(lock_);
			--queued_;
			sender_ready_.notify_all();
		}

		void on_error(const proton::error_condition& e) override {
			std::cerr << "error: " << e << std::endl;
			exit(1);
		}

};

class AdptProtonManager {
	public:
	using Msg_queue = threadsafe_queue<std::string>;

	AdptProtonManager() = delete;
	AdptProtonManager(const AdptProtonManager&) = delete;

	AdptProtonManager(Msg_queue& mq) : msg_queue_(mq) { };

	void init() {
		// run the Proton container in a thread
		auto container_thread = std::thread([&]() { container_.run(); });
		std::string msg;
		for(;;) {
			msg_queue_.wait_and_pop(msg);
			// parse and publish, here we publish directly TODO
			std::cout << "[info] mgr get msg  " << msg << std::endl;
			publish("127.0.0.1:5672127.0.0.1:5672/examples", msg);
		}
	}

	void new_connection(const std::string& url, const std::string& addr) {
		std::cout << "[new conn] " << "url: " << url << ", addr: " << addr << std::endl;
		const std::string conn_key(url + addr);
		connections_[conn_key] = std::make_shared<Connection>(this->container_, url, addr);
	}

	// returns status code, expose this interface for pistache http server
	int publish(const std::string& destination, const std::string& msg) {
		if(connections_.find(destination) == connections_.end()) {
			std::cout << "[error] dest " << destination << " not found." << std::endl;
			return 404;
		}
		auto conn = connections_[destination];
		std::thread t([&]() { send_thread(*conn, msg); });
		t.join();
		std::cout << "[info] waiting for publish thead" << std::endl;
		return 200;
	}

	private:
	void send_thread(Connection& conn, const std::string& msg) {
		auto id = std::this_thread::get_id();
		proton::message s(msg);
		conn.send(s);
		PRINT(std::cout << id << " send " << msg << std::endl);
	}

	private:
	proton::container container_;
	Msg_queue& msg_queue_;
	std::unordered_map<std::string, std::shared_ptr<Connection> > connections_;

};

#endif
