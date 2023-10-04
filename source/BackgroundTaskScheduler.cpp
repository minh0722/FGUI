#include "BackgroundTaskScheduler.h"

std::queue<Task> BackgroundTaskScheduler::m_TaskQueue;
std::mutex BackgroundTaskScheduler::m_mutex;
std::condition_variable BackgroundTaskScheduler::m_cv;

void BackgroundTaskScheduler::Init()
{
    for (int i = 0; i < WORKER_THREAD_COUNT; ++i)
    {
        std::thread t = std::thread(&BackgroundTaskScheduler::DoWork);
        t.detach();
    }
}

void BackgroundTaskScheduler::DoWork()
{
    while (true)
    {
        std::unique_lock<std::mutex> lockGuard(m_mutex);
        m_cv.wait(lockGuard, []() { return !m_TaskQueue.empty(); });

        Task t = m_TaskQueue.front();
        m_TaskQueue.pop();
        lockGuard.unlock();

        t();
    }
}

