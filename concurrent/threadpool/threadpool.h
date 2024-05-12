#pragma once

#include <vector>
#include <thread>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <future>

class Threadpool{
public:
    Threadpool(size_t threads);
    ~Threadpool();
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)->std::future<typename std::result_of<F(Args...)>::type>;
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    //synchronization
    bool stop;
    std::condition_variable cv;
    std::mutex mu;
};

Threadpool::Threadpool(size_t threads):stop(false){
    for(size_t i=0;i<threads;i++){
        workers.emplace_back([this]{
            while(true){
                //创建task
                std::function<void()> task;
                {   
                    //将std::mutex改为std::unique_lock
                    std::unique_lock<std::mutex> lock(this->mu);
                    //等待lock，并且返回stop或者tasks执行完毕
                    //这里通过条件变量，让任务队列为空的时候可以阻塞，避免不断轮询判断缓冲区是否为空消耗CPU
                    this->cv.wait(lock,
                        [this]{return this->stop || !this->tasks.empty();});
                    if(this->stop&&this->tasks.empty())
                        return;
                    task=std::move(this->tasks.front());
                    this->tasks.pop();
                }
                //执行取出的task
                task();
            }
        });
    }
}

template<class F,class... Args>
auto Threadpool::enqueue(F&& f, Args&&... args)->std::future<typename std::result_of<F(Args...)>::type>{
    //using用来给typename std::result_of<F(Args...)>::type取别名，相当于typedef
    using return_type=typename std::result_of<F(Args...)>::type;
    // std::packaged_task是C++11引入的标准库类，用于封装可调用对象，如函数等，
    // 并将封装对象作为异步任务进行管理，通过与std::future结合使用，完成异步任务结果的获取
    auto task=std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f),std::forward<Args>(args)...)
    );
    //这里创建得到一个packaged_task对象，进而通过它的get_future()成员函数得到已
    //经配对完成的future对象。随后，我们通过ThreadPool::ThreadPool中创建的子线
    //程来执行这个packaged_task，也就是执行线程函数
    std::future<return_type> res=task->get_future();
    {
        std::unique_lock<std::mutex> lock(this->mu);
        if(this->stop)
            throw std::runtime_error("enqueue on stopped Threadpool");
        this->tasks.emplace([task](){(*task)();});
    }
    cv.notify_one();
    return res;
    //std::queue< std::function<void()> > tasks是任务队列。
    //std::function等价于函数指针，绑定{ (*task)(); }。这里将任务放到任务队列中
    //，为了避免有多个线程同时对tasks进行操作，所以这里得加锁std::unique_lock<std::mutex> lock(queue_mutex)，
    //保证在任一时刻，只能有一个线程访问它。然后通过condition.notify_one() 
    //通知ThreadPool::ThreadPool中第一个进入阻塞或者等待的线程，取消对线程的阻塞
}

Threadpool::~Threadpool(){
    {
        std::unique_lock<std::mutex> lock(this->mu);
        this->stop=true;
    }
    cv.notify_all();
    for(std::thread& worker:workers){
        worker.join();
    }
}
