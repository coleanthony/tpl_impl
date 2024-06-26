#include <iostream>
#include <vector>
#include <chrono>
 
#include "threadpool.h"
 
int fun(int i)
{
    std::cout << "hello " << i << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "world " << i << std::endl;
    return i * i;
}
 
int main()
{
 
    Threadpool pool(4);
    std::vector< std::future<int> > results;
    
    for (int i = 0; i < 8; ++i) {
        results.emplace_back(pool.enqueue(fun, i));
    }
 
    for(auto && result: results)
        std::cout << "get:" << result.get() << std::endl;
    //std::cout << std::endl;
 
    return 0;
}