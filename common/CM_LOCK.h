#pragma once
#ifndef CM_LOCK_H
#define CM_LOCK_H

#include <atomic>
#include <thread>

typedef std::atomic<std::uint8_t> CM_LOCK;

inline void LOCK_Initialize(CM_LOCK* pLock)
{
	pLock->store(0, std::memory_order_seq_cst);
}

inline void LOCK_Aquire(CM_LOCK* pLock)
{
    while (pLock->exchange(1, std::memory_order_seq_cst) != 0)
    {
        std::this_thread::yield();
    }
}

inline void LOCK_Release(CM_LOCK* pLock)
{
    pLock->store(0, std::memory_order_seq_cst);
}


#endif // CM_LOCK_H