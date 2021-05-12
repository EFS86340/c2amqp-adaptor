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
#include "spdlog/spdlog.h"

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

#include <sstream>
#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include <unordered_map>


static const int PROTON_STATUS_NO_SUCH_CONNECTION = 1;
static const int PROTON_STATUS_TIMEOUT = 2;

// class for AMQP connection
class Connection : public proton::messaging_handler {
	public:
		Connection(proton::container& cont, const std::string& url, const
				std::string& addr) : conn_url_(url), addr_(addr), work_queue_(0) {
			cont.open_sender(addr, proton::connection_options().handler(*this));
			std::cout << "[debug] Connection ctor open sender done." << std::endl;
			spdlog::debug("Connection ctor open sender done.");
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
			spdlog::info("connection ready to send");
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
			spdlog::info("sender open now");
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

		void on_tracker_accept(proton::tracker& t) override {
			spdlog::info("Proton get tracker info: accepted.");
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
			spdlog::info("Wait for new notification...");
			msg_queue_.wait_and_pop(msg);
			// parse and publish, here we publish directly TODO
			spdlog::info("AdptProtonManager get message {}", msg);
			auto target = connection_dispatcher(msg);
			spdlog::info("Trying to publish to target {}.", target);
			publish(target, msg);
		}
	}

	void new_connection(const std::string& url, const std::string& addr) {
		spdlog::info("New Connection: {} {}", url, addr);
		const std::string conn_key(addr);
		connections_[conn_key] = std::make_shared<Connection>(this->container_, url, addr);
	}

	// returns status code, expose this interface for pistache http server
	int publish(const std::string& destination, const std::string& msg) {
		if(connections_.find(destination) == connections_.end()) {
			spdlog::error("Destination {} Not Found.", destination);
			return 404;
		}
		auto conn = connections_[destination];
		std::thread t([&]() { send_thread(*conn, msg); });
		// t.join();
		spdlog::info("Publish done, wait for send_thread to finish");
		return 200;
	}

	private:

	// we now use the msg body to determine which sender to send the msg
	// TODO: json parser to full parse the notification
	std::string connection_dispatcher(const std::string& msg) {
		int count = 0;
		std::stringstream ss;
		for(auto c : msg) {
			if(c == '"') {
				count++;
			} else if(count == 3) {
				ss << c;
			}
		}
		return ss.str();
	}

	void send_thread(Connection& conn, const std::string& msg) {
		std::stringstream id;
		id << std::this_thread::get_id();
		proton::message s(msg);
		conn.send(s);
		spdlog::info("Thread of id {} send message {}.", id.str(), msg);
	}

	private:
	proton::container container_;
	Msg_queue& msg_queue_;
	std::unordered_map<std::string, std::shared_ptr<Connection> > connections_;

};

#endif
