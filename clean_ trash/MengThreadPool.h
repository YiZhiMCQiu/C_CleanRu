#include<iostream>
#include<Thread>
#include<vector>
#include<queue>
#include<functional>
#include<condition_variable>
#include<mutex>
using namespace std;

class MThreadPool {
public:
	MThreadPool(int threadNum);
	template<class T>
	void enterTask(T&& t);
	bool waitTaskComplete();
	~MThreadPool();

private:
	vector<thread> threads;
	queue<function<void()>> tasks;
	condition_variable m_cv;
	mutex mtx;
	unsigned int taskNum;
	bool stop;
};

template<class T>
void MThreadPool::enterTask(T&& t)
{
	function<void()> func(t);
	{
		unique_lock(mutex) lock(mtx);
		tasks.emplace(func);
		taskNum++;
	}
	m_cv.notify_one();
}
