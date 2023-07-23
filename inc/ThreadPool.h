//
// Created by 12414 on 2023/2/27.
//

#ifndef SOCKETTEST_THREADPOOL_H
#define SOCKETTEST_THREADPOOL_H

#pragma once
#ifndef THREAD_POOL_H
#define THREAD_POOL_H
// 使用#pragma once  和 #ifndef THREAD_POOL_H 是为了避免同一个文件被include多次
#include <vector>
#include <queue>
#include <atomic>
#include <future>
#include <condition_variable>
#include <thread>
#include <functional>
#include <stdexcept>
namespace std
{
//线程池最大容量,应尽量设小一点
#define  THREADPOOL_MAX_NUM 16
//#define  THREADPOOL_AUTO_GROW

//线程池,可以提交变参函数或拉姆达表达式的匿名函数执行,可以获取执行返回值
//不直接支持类成员函数, 支持类静态成员函数或全局函数,Opteron()函数等
    class ThreadPool
    {
        using Task = function<void()>;	//定义类型
        vector<thread> threads;     //线程池
        queue<Task> taskQ;            //任务队列
        mutex muLocker;                   //同步
        condition_variable taskCv;   //条件阻塞
        atomic<bool> ifRun;     //线程池是否执行
        atomic<int>  freeThreadNum;  //空闲线程数量

    public:
        inline ThreadPool(unsigned short size = 4) {
            ifRun=true;
            freeThreadNum=0;
            addThread(size);
        }
        inline ~ThreadPool()
        {
            ifRun = false;
            taskCv.notify_all(); // 唤醒所有线程执行
            for (thread& thread : threads) {
                //thread.detach(); // 让线程“自生自灭”
                if (thread.joinable())
                    thread.join(); // 等待任务结束， 前提：线程一定会执行完
            }
        }

    public:
        // 提交一个任务
        // 调用.get()获取返回值会等待任务执行完,获取返回值
        // 有两种方法可以实现调用类成员，
        // 一种是使用   bind： .commit(std::bind(&Dog::sayHello, &dog));
        // 一种是用   mem_fn： .commit(std::mem_fn(&Dog::sayHello), this)
        template<class F, class... Args>
        auto commit(F&& f, Args&&... args) ->future<decltype(f(args...))>
        {
            if (!ifRun) {   // stoped ??
                throw runtime_error("commit on ThreadPool is stopped.");
            }

            using RetType = decltype(f(args...)); // typename std::result_of<F(Args...)>::type, 函数 f 的返回值类型
            auto task = make_shared<packaged_task<RetType()>>(
                    bind(forward<F>(f), forward<Args>(args)...)
            ); // 把函数入口及参数,打包(绑定)
            future<RetType> future = task->get_future();
            {    // 添加任务到队列
                lock_guard<mutex> lock{ muLocker };//对当前块的语句加锁  lock_guard 是 mutex 的 stack 封装类，构造的时候 lock()，析构的时候 unlock()
                taskQ.emplace([task]() { // push(Task{...}) 放到队列后面
                    (*task)();
                });
            }
#ifdef THREADPOOL_AUTO_GROW
            if (_idlThrNum < 1 && _pool.size() < THREADPOOL_MAX_NUM)
				addThread(1);
#endif // !THREADPOOL_AUTO_GROW
            taskCv.notify_one(); // 唤醒一个线程执行

            return future;
        }

        //空闲线程数量
        int idlCount() { return freeThreadNum; }
        //线程数量
        int thrCount() { return threads.size(); }
#ifndef THREADPOOL_AUTO_GROW
    private:
#endif // !THREADPOOL_AUTO_GROW
        //添加指定数量的线程
        void addThread(unsigned short size)
        {
            for (; threads.size() < THREADPOOL_MAX_NUM && size > 0; --size)
            {   //增加线程数量,但不超过 预定义数量 THREADPOOL_MAX_NUM
                threads.emplace_back([this] { //工作线程函数
                    while (ifRun)
                    {
                        Task task; // 获取一个待执行的 task
                        {
                            // unique_lock 相比 lock_guard 的好处是：可以随时 unlock() 和 lock()
                            unique_lock<mutex> lock{ muLocker };
                            taskCv.wait(lock, [this] {
                                return !ifRun || !taskQ.empty();
                            }); // wait 直到有 task
                            if (!ifRun && taskQ.empty())
                                return;
                            task = move(taskQ.front()); // 按先进先出从队列取一个 task
                            taskQ.pop();
                        }
                        freeThreadNum--;
                        task();//执行任务
                        freeThreadNum++;
                    }
                });
                freeThreadNum++;
            }
        }
    };

}


#endif

#endif //SOCKETTEST_THREADPOOL_H
