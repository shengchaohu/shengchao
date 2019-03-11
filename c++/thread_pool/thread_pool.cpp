#include "thread_pool.h"
#include<iostream>
#include<functional>
#include<string>
using namespace mpcs51044;
using namespace std;
using namespace std::string_literals;
ThreadPool pool;

int main()
{
	auto voidResult = pool.submit([]() { cout << "This is result1\n"; });
	auto product = pool.submit(multiplies(), 1.1, 2.3);
	cout << "product is " + to_string(product.get());
	return 0;
}