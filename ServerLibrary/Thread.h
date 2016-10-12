#pragma once
#include "../stdafx.h"

#define MAKE_THREAD(className, process) (new Thread(new thread_t(&className##::##process, this), L#className))
#define GET_CURRENT_THREAD_ID			std::this_thread::get_id().hash
class Lock;
typedef std::function<void(void *)>ThreadFunction;

class Thread
{
	size_t				id_;
	wstr_t				name_;
	thread_t			*thread_;
	Lock				*lock_;

public :
	Thread(thread_t *thread, wstr_t name);
	~Thread();

	size_t id();
	wstr_t &name();

	void setLock(Lock *lock);
	Lock *lock();
};

class ThreadManager : public Singleton < ThreadManager>
{
#ifdef THREAD_POOL_HASHMAP
	hash_map <size_t, Thread*> threadPool_;
#else
	map<size_t, Thread*>threadPool_;
#endif

public:
	~ThreadManager();

	void put(Thread *thread);
	void remove(size_t id);
	Thread *at(size_t id);
};