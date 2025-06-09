#pragma once
//------------------------------------------------------------------------------
/**
    thread.h
    
    Platform agnostic threading
    
    (C) 2025 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#if __WIN32__
#else
#include <pthread.h>
#endif

#include "memory.h"

namespace GPULang
{
struct ThreadInfo
{
    size_t stackSize = 0; // 0 means OS default
};

struct Thread
{
#if __WIN32__
    HANDLE thread;
#else
    pthread_t thread;
#endif
    
    std::function<void()> l;
};

//------------------------------------------------------------------------------
/**
 */
void* threadFunc(void* arg)
{
    auto l = static_cast<std::function<void()>*>(arg);
    (*l)();
}

//------------------------------------------------------------------------------
/**
 */
template <typename LAMBDA>
Thread*
CreateThread(const ThreadInfo& info, LAMBDA&& lambda)
{
    Thread* ret = Alloc<Thread>();
    ret->l = std::move(lambda);
    
#if __WIN32__
    ret->thread = CreateThread(
        nullptr,
        info->stackSize > 0 ? info.stackSize : 0,
        threadFunc,
        &ret->l,
        0,               // Creation flags
        nullptr          // Thread ID (optional)
    );
#else
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    if (info.stackSize > 0)
        pthread_attr_setstacksize(&attr, info.stackSize);
    pthread_create(&ret->thread, &attr, threadFunc, &ret->l);
#endif
}

} // namespace GPULang
