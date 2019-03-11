#ifndef THREAD_POOL_H
#  define THREAD_POOL_H
#include<queue>
#include<future>
#include<mutex>
#include<condition_variable>
#include<thread>
#include<algorithm>
#include<functional>
#include<vector>
#include<utility>
#include<memory>

using std::queue;
using std::packaged_task;
using std::mutex;
using std::condition_variable;
using std::thread;
using std::max;
using std::vector;
using std::unique_lock;
using std::lock_guard;
using std::move;
using std::ref;
using std::make_shared;

namespace mpcs51044 {

// Producer consumer queue for tasks with no argumented or returns adapted from Lec 6 Anthony Williams code
class TaskQueue {
public:
	void produce(packaged_task<void()> &&task)
	{
		lock_guard lk(m);
		tasks.push(move(task));
		cond.notify_one();
	}

	auto consume() {
		unique_lock lk(m);
		cond.wait(lk, [&]() {return !tasks.empty(); });
		auto task = move(tasks.front());
		tasks.pop();
		return task;
	}

private:
	mutex m;
	queue<packaged_task<void()>> tasks;
	condition_variable cond;
};


class ThreadPool {
public:
	ThreadPool(size_t numThreads = max(thread::hardware_concurrency(), 4u) - 2) {
		for (size_t i = 0; i < numThreads; i++) {
			// Three different versions of the following. Uncomment any one
			workers.push_back(thread(threadWorker, ref(taskQueue)));
//			workers.push_back(thread(&ThreadPool::work, this));
//			workers.push_back(thread([&]() { work(); }));
			workers.back().detach();
		}
	}

	template<typename Func, typename ...Args>
	auto submit(Func f, Args... args) {
		using RetType = typename result_of<Func(Args...)>::type;
		auto pt = make_shared<packaged_task<RetType(Args...)>>(f);
		auto result = pt->get_future();
		taskQueue.produce(packaged_task<void()>([=]() mutable { (*pt)(args...); }));
		return result;
	}

private:
	void work() {
		for (;;) {
			taskQueue.consume()();
		}
	}
	static void threadWorker(TaskQueue &tq) {
		for (;;) {
			tq.consume()();
		}
	}
	vector<thread> workers;
	TaskQueue taskQueue;
};
}

#endif