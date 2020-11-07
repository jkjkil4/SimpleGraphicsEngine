#pragma once

#include <mutex>
#include <condition_variable>

class Wait_Notify
{
public:
	std::condition_variable cv;
	std::mutex mtx;
	bool flag = false;

	void wait() {
		std::unique_lock<std::mutex> lck(mtx);
		if (!flag)
			cv.wait(lck);
		flag = false;
	}
	void notify() {
		std::unique_lock<std::mutex> lck(mtx);
		flag = true;
		cv.notify_all();
	}
};

