#include<iostream>
#include<map>
#include"my_async.h"
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

int main()
{
  int num{ 7356 };
  auto result = my_async(factor, num);
  // Do a bunch of stuff
  std::cout << num << " has " << result.get().size() << " distinct prime factors" << std::endl;
  return 0;
}