#pragma once

#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

#define WORKER_THREAD_COUNT 2

struct Task
{
    template <class Func>
    Task(Func func)
    {
        f = func;
    }

    void operator()()
    {
        f();
    }

    std::function<void()> f;
};

class BackgroundTaskScheduler
{
public:
    static void Init();

    template <class Func>
    static void AddTask(Func func);

private:
    static void DoWork();

private:
    static std::queue<Task> m_TaskQueue;
    static std::mutex m_mutex;
    static std::condition_variable m_cv;
};

template <class Func>
void BackgroundTaskScheduler::AddTask(Func func)
{
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    m_TaskQueue.push(Task(func));
    m_cv.notify_one();
}

