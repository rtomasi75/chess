#pragma once
#ifndef EXECUTIONTOKEN_H
#define EXECUTIONTOKEN_H

#include <condition_variable>
#include <mutex>

class ExecutionToken
{
private:
	std::condition_variable cv;
	std::mutex mtx;
	bool _bIsDone;
	void* _pExecutionContext;
public:
	ExecutionToken(void* pExecutionContext) :
		_bIsDone(false),
		_pExecutionContext(pExecutionContext)
	{
	}
	~ExecutionToken() = default;
	void Reset()
	{
		std::lock_guard<std::mutex> lock(mtx);
		_bIsDone = false;
	}
	void Signal()
	{
		{
			std::lock_guard<std::mutex> lock(mtx);
			_bIsDone = true;
		}
		cv.notify_one();
	}
	void Wait()
	{
		std::unique_lock<std::mutex> lock(mtx);
		cv.wait(lock, [&]() { return _bIsDone; });
	}
	const void* Context() const
	{
		return _pExecutionContext;
	}
};


#endif // EXECUTIONTOKEN_H