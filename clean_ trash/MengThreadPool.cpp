#include "MengThreadPool.h"

MThreadPool::MThreadPool(int threadNum):stop(false)
{
	for (int i = 0; i <= threadNum; i++) {
		threads.emplace_back([this] {
			while (1) {
				unique_lock<mutex>lock(mtx);
				m_cv.wait(lock, [this] {
					return !tasks.empty();
					});

				if (stop && tasks.empty()) {
					return;
				}
				auto func = tasks.front();
				tasks.pop();
				func();
				taskNum--;
			}
			});
	}
}

bool MThreadPool::waitTaskComplete()
{
	unique_lock<mutex> lock(mtx);
	m_cv.wait(lock, [this] {
		return taskNum == 0;
		});
	return true;
}

MThreadPool::~MThreadPool()
{
	{
		unique_lock<mutex> lock(mtx);
		stop = true;
		m_cv.notify_all();
	}

	for (auto& thread : threads) {
		thread.join();
	}
}
