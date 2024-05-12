# Thread pool

This is a simple thread pool implementation in C++. It uses a fixed-size array of threads to execute tasks. The thread pool can be used to execute tasks concurrently, which can improve the performance of applications that need to perform many small tasks.

Here is a simple example of how to use the thread pool:

```c++
    Threadpool pool(4);
    std::vector< std::future<int> > results;
    
    for (int i = 0; i < 8; ++i) {
        results.emplace_back(pool.enqueue(fun, i));
    }
```