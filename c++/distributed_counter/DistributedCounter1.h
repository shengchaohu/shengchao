#ifndef DISTRIBUTED_COUNTER_H
#  define DISTRIBUTED_COUNTER_H
// Implement a distributed counter with a thread local count in
// conformance with cache-conscious programming best practices in
// the lecture.
//
// Most of the complexity here is in managing the thread local counts
// as threads are created or destroyed. Study how we use various
// multithreading techniques to accurately and safely track these.

#include<mutex>
#include<shared_mutex>
#include<map>
#include<numeric>

namespace mpcs {
  typedef long long value_type;
  thread_local value_type count=0;
class DistributedCounter { 
private:
  std::shared_mutex mtx;

public:
  void operator++() {
	  std::unique_lock<std::shared_mutex> lock(mtx);
	  ++count;
  }
  void operator++(int) {
	  std::unique_lock<std::shared_mutex> lock(mtx);
	  count++;
  }

  value_type get() {
	  std::shared_lock<std::shared_mutex> mtx;
	  return count;
  }
};
}
#endif