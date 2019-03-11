#include<thread>
#include<utility>
#include<map>
#include<cmath>
#include<iostream>
#include<future>

#include"my_packaged_task.h"
using namespace mpcs;
// Factors its argument in to primes. Could be slow, so good
// candidate to run in another thread
// Interesting exercise: How does this function work?
std::map<unsigned, unsigned> factor(unsigned n)
{
  std::map<unsigned, unsigned> result;
  for (int i = 2; i <= std::sqrt(n); i++) {
    if (n % i == 0) {
      result[i]++;
      n /= i;
      i = 1;
    }
  }
  result[n]++;
  return result;
}

my_packaged_task<std::map<unsigned, unsigned>(unsigned)> mpt(factor);

int main()
{ 
  auto factorization = mpt.get_future();
  int num{ 7356 };
  std::thread thr([](unsigned n) { mpt(n); }, num);
  std::cout << num << " has " << factorization.get().size() << " distinct prime factors" << std::endl;
  thr.join();
  return 0;
}