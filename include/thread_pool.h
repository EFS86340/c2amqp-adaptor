/*
 * =====================================================================================
 *
 *       Filename:  thread_pool.h
 *
 *    Description:  A simple thread pool for AdptProtonManager
 *
 *        Version:  1.0
 *        Created:  05/11/2021 15:21:09
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Xu Wang (), wilyxerus@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef C2AMQP_THREAD_POOL_H_
#define C2AMQP_THREAD_POOL_H_

#include "ts_queue.h"

#include <atomic>
#include <functional>
#include <memory>
#include <thread>
#include <type_traits>
#include <future>
#include <vector>

class join_threads {
	std::vector<std::thread>& threads_;
	public:
	explicit join_threads(std::vector<std::thread>& threads) : threads_(threads)
	{  }

	~join_threads() {
		for(unsigned long i = 0; i < threads_.size(); ++i) {
			if(threads_[i].joinable())
				threads_[i].join();
		}
	}
};

// for those functions with return value
class function_wrapper {
	struct impl_base {
		virtual void call() = 0;
		virtual ~impl_base() { }
	};
	std::unique_ptr<impl_base> impl;
	template<typename F>
		struct impl_type : impl_base {
			F f_;
			impl_type(F&& f) : f_(std::move(f)) {  }
			void call() { f_(); }
		};

	public:
	template<typename F>
	function_wrapper(F&& f) : impl(new impl_type<F>(std::move(f))) { }

	void operator()() { impl->call(); }

	function_wrapper() = default;

	function_wrapper(function_wrapper&& other) : impl(std::move(other.impl)) {  }

	function_wrapper& operator=(function_wrapper&& other) {
		impl = std::move(other.impl);
		return *this;
	}

	function_wrapper(const function_wrapper&) = delete;
	function_wrapper(function_wrapper&) = delete;
	function_wrapper& operator=(const function_wrapper&) = delete;
};

class thread_pool {
	std::atomic_bool done_;
	threadsafe_queue<function_wrapper> pool_work_queue_;
	std::vector<std::thread> threads_;
	join_threads joiner_;

	void worker_thread() {
		while(!done_) {
			function_wrapper task;
			if(pool_work_queue_.try_pop(task)) {
				task();
			} else {
				std::this_thread::yield();
			}
		}
	}

	public:
	thread_pool() : done_(false), joiner_(threads_) {
		// too many to run in laptop
		// unsigned const thread_count = std::thread::hardware_concurrency();
		unsigned const thread_count = 5;

		try {
			for(unsigned i = 0; i < thread_count; ++i) {
				threads_.push_back(std::thread(&thread_pool::worker_thread, this));
			}
		}
		catch(...) {
			done_ = true;
			throw;
		}
	}

	~thread_pool() {
		done_ = true;
	}

	template<typename FunctionType>
		std::future<typename std::result_of<FunctionType()>::type> submit(FunctionType f) {
			using result_type = typename std::result_of<FunctionType()>::type;
			std::packaged_task<result_type()> task(std::move(f));
			std::future<result_type> res(task.get_future());
			pool_work_queue_.push(std::move(task));
			return res;
		}

	template<typename FunctionType>
		void submit_async(FunctionType f) {
		}

};


#endif
