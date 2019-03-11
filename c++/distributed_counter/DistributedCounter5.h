#ifndef DISTRIBUTED_COUNTER_5_H
#  define DISTRIBUTED_COUNTER_5_H
#include<memory>
#include<atomic>
#include<mutex>
#include<shared_mutex>
#include<numeric>
#include<set>
#include<array>
#include<bitset>
#include<stdexcept>
using std::unique_ptr;
using std::atomic;
using std::mutex;
using std::lock_guard;
using std::shared_lock;
using std::shared_mutex;
using std::set;
using std::accumulate;
using std::array;
using std::bitset;
using std::out_of_range;

namespace mpcs51044 {
int const maxCounters = 100;
class DistributedCounter5 {
public:
	DistributedCounter5() {
		for (int i = 0; i < maxCounters; i++) {
			if (!usedCounters[i]) {
				counter = i;
				usedCounters.set(i);
				return;
			}
		}
		throw out_of_range("Too many counters");
	}
	~DistributedCounter5() {
		callsFromDeadCounters[counter] = 0;
		for (auto &liveCounter : liveCounters)
			(*liveCounter)[counter] = 0;
		usedCounters.reset(counter);
	}
	// This method keeps track of how many times it has been called
	unsigned get() {
		shared_lock lck(mtx);
		return accumulate(liveCounters.begin(), liveCounters.end(), callsFromDeadCounters[counter].load(),
			[&](unsigned acc, array<atomic<unsigned>, maxCounters> * next) { return acc + (*next)[counter]; });
	}
	void operator++(int) { calls.increment(counter); };
	void operator++() { calls.increment(counter); };
private:
	
	struct DistributedCounterThrLocal {
		DistributedCounterThrLocal() {
			lock_guard lck(mtx);
			liveCounters.insert(&counts);
		}
		~DistributedCounterThrLocal() {
			lock_guard lck(mtx);
			for(int i = 0; i < maxCounters; i++)
				callsFromDeadCounters[i] += counts[i];
			liveCounters.erase(&counts);
		}
		void increment(int i) { counts[i]++; };
		array<atomic<unsigned>, maxCounters> counts;
	};
	size_t counter;
	static bitset<maxCounters> usedCounters;
	static atomic<unsigned> callsFromDeadCounters[maxCounters];
	static set<array<atomic<unsigned>, maxCounters> *> liveCounters;
	static shared_mutex mtx;
	static thread_local DistributedCounterThrLocal calls;

};
}
#endif