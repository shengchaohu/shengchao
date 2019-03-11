#ifndef DISTRIBUTED_COUNTER_H
#  define DISTRIBUTED_COUNTER_H
// Create multiple "sub-counters" so that different threads are unlikely
// to use the same sub-counter, reducing contention compared to DistributedCounter1
#include<atomic>
#include<mutex>
#include<shared_mutex>
#include<map>
#include<thread>
#include<numeric>
#include<utility>
#include<vector>
namespace mpcs {
  typedef size_t value_type;
  thread_local value_type count;
  std::shared_mutex sm;

class DistributedCounter {
  struct bucket {//合成构造函数
    
  };
  //static class member
  static size_t const buckets{ 128 };//initialze a variable buckets of type size_t;
  std::vector<bucket> counts{ buckets };
public:
  void operator++() {
    //先声明并默认初始化一个hash变量，然后对它用()operator进行hash，最后把hash的返回值取同余
  	size_t index = std::hash<std::thread::id>()(std::this_thread::get_id()) % buckets;
  	std::unique_lock<std::shared_mutex> ul(counts[index].sm);//初始化
    counts[index].count++;
  }
  void operator++(int) {//post-increment
	  ++*this;
  }

  value_type get() {
    return std::accumulate(counts.begin(), counts.end(), (value_type)0, 
		  [](auto acc, auto &x) { std::shared_lock<std::shared_mutex> sl(x.sm); 
      return acc + x.count; });
  }
};
}
#endif