#ifndef MY_PACKAGED_TASK_H
#  define MY_PACKAGED_TASK_H
#include<utility>
#include<map>
#include<future>
#include<stdexcept>
using std::forward;
namespace mpcs {
template<typename T>
struct my_packaged_task;

template<typename R, typename ...Args>
struct my_packaged_task<R(Args...)> {
  template<typename T>
  my_packaged_task(T t) : callable(t) {}
  void operator()(Args... args) {
    try {
      result.set_value(callable(args...));
    }
    catch (...) {
      result.set_exception(std::current_exception());
    }
  }
  std::future<R> get_future() { return result.get_future(); }
  std::function<R(Args...)> callable;
  std::promise<R> result;
};

}
#endif