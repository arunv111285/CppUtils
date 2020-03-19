//=========================================================================================================================!
// Implementation base on the following article
// https://www.fluentcpp.com/2018/12/28/timer-cpp/
//=========================================================================================================================!

#pragma once

//=========================================================================================================================!

#include <chrono>
#include <thread>
#include <atomic>

//=========================================================================================================================!
namespace av { namespace utils {
//=========================================================================================================================!

class Timer
{
public:

	template <typename Func>
	void setTimeout(Func func, std::chrono::microseconds timeout)
	{
		isActive_ = true;
		std::thread t([=]()
		{
			if (!isActive_)
				return;
			std::this_thread::sleep_for(timeout);
			if (!isActive_)
				return;
				func();
		});
		t.detach();
	}

	template <typename Func>
	void setInterval(Func func, std::chrono::microseconds timeout)
	{
		isActive_ = true;
		std::thread t([=]()
		{
			while (true)
			{
				if (!isActive_)
					return;
				std::this_thread::sleep_for(timeout);
				if (!isActive_)
					return;
				func();
			}
		});
		t.detach();
	}

	void stop()
	{
		isActive_ = false;
	}

private:

	std::atomic<bool> isActive_;
};

//=========================================================================================================================!
}}
//=========================================================================================================================!

