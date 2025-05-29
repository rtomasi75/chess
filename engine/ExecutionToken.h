#pragma once
#ifndef EXECUTIONTOKEN_H
#define EXECUTIONTOKEN_H

#include <condition_variable>
#include <mutex>
#include "libSearch.h"

class ExecutionToken
{
private:
	std::condition_variable cv;
	std::mutex mtx;
	bool _bIsDone;
	void* _pExecutionContext;
	SE_SEARCHCONTEXTSTORAGE _searchContext;
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
	const void* ExecutionContext() const
	{
		return _pExecutionContext;
	}
	const SE_SEARCHCONTEXTSTORAGE* SearchContext() const
	{
		return &_searchContext;
	}
	void SetSearchContext(SE_SEARCHCONTEXTSTORAGE* pSearchContext)
	{
		memcpy(&_searchContext, pSearchContext, sizeof(SE_SEARCHCONTEXTSTORAGE));
	}
};


#endif // EXECUTIONTOKEN_H