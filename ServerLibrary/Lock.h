#pragma once
#include "../stdafx.h"

class Lock
{
private:
	lock_t			mutex_;
	wstr_t			Name_;
	size_t			lockID_;
	size_t			threadID_;

	wstr_t			lockingFILE_;
	int				lockingLINE_;

public:
	Lock(WCHAR* name);
	virtual ~Lock();

	const WCHAR* name();
	size_t lockID();

	lock_t& mutex();
	void lock(LPCWSTR fileName, int lineNo);
	void unlock();

	void setThreadID(size_t id);
	size_t threadID();
};

class LockSafeScope
{
	Lock			*lock_;
public:
	LockSafeScope(Lock *lock, LPCWSTR fileName, int lineNo);
	~LockSafeScope();
};

#define SAFE_LOCK(lock)			LockSafeScope __lockSafe(&lock, _W(__FILE__), __LINE__);

//Search Deadlock
class LockManager : public Singleton < LockManager >
{
	size_t				idSeed_;

public:
	LockManager();

	Lock* searchLockCycle(Lock *newLock);
	Lock* checkDaeadLock(Lock *lock);

	size_t generalID();
};