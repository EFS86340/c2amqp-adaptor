/*
 * =====================================================================================
 *
 *       Filename:  ts_queue.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/23/21 15:39:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef C2AMQP_THREADSAFE_QUEUE_H_
#define C2AMQP_THREADSAFE_QUEUE_H_

#include <memory>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <type_traits>

template<typename T>
class threadsafe_queue {
	private:
	mutable std::mutex mut;
	std::queue<std::shared_ptr<T> > data_queue;
	std::condition_variable data_cond;

	public:
	threadsafe_queue() {}

	void wait_and_pop(T& value) {
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this]{ return !data_queue.empty();});
		value = std::move(*data_queue.front());
		data_queue.pop();
	}

	bool try_pop(T& value) {
		std::lock_guard<std::mutex> lk(mut);
		if(data_queue.empty())
			return false;
		value = std::move(*data_queue.front());
		data_queue.pop();
		return true;
	}

	void push(T new_value) {
		std::shared_ptr<T> data(std::make_shared<T>(std::move(new_value)));
		std::lock_guard<std::mutex> lk(mut);
		data_queue.push(data);
		data_cond.notify_one();
	}
};

#endif
