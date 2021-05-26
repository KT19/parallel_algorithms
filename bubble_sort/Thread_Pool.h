#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include<iostream>
#include<vector>
#include<thread>
#include<mutex>
#include<future>
#include<atomic>
#include<queue>
#include<condition_variable>

using namespace std;

class ThreadPool 
{
    private:
        vector<thread> workers; //track threads
        queue< function<void()> > tasks; //task

        //synchronization
        mutex queue_mutex;
        condition_variable cv_task;
        condition_variable cv_finished;
        int busy;
        bool stop;

        void thread_proc();
    public:
        ThreadPool(size_t);
        template<class F, typename... Args> void enqueue(F&& f, Args&&... args);
        ~ThreadPool();
        void waitAll(void);
};

//constructor (launch threds)
ThreadPool::ThreadPool(size_t threads_num) : busy(0), stop(false) {
    for(size_t i = 0;i < threads_num;i++) {
        workers.emplace_back(bind(&ThreadPool::thread_proc, this));
    }
}

void ThreadPool::thread_proc() {
    for(;;) {
        unique_lock<mutex> lock(queue_mutex);
        cv_task.wait(lock, [this]() {return stop || !tasks.empty();});
        if(!tasks.empty()) {
            busy++;

            auto task = tasks.front();tasks.pop();
            lock.unlock();

            task();
            lock.lock();
            busy--;
            cv_finished.notify_one();
        } else if(stop) {
            break;
        }
    }
}

//destructor
ThreadPool::~ThreadPool() {
    unique_lock<mutex> lock(queue_mutex);
    stop = true;
    cv_task.notify_all();
    lock.unlock();

    for(size_t i = 0;i < workers.size();i++) {
        workers[i].join();
    }

    workers.clear();
}

template<class F, typename... Args> void ThreadPool::enqueue(F&& f, Args&&... args) {
    auto task = bind(forward<F>(f), forward<Args>(args)...);
    unique_lock<mutex> lock(queue_mutex);
    tasks.push( [task](){ (task)(); });
   
    cv_task.notify_one();
    return;
}

void ThreadPool::waitAll(void) {
    unique_lock<mutex> lock(queue_mutex);
    cv_finished.wait(lock, [this](){ return tasks.empty() && (busy == 0);});
}

#endif
