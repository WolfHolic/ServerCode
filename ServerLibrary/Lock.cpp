#include "../stdafx.h"
#include "Lock.h"
#include "Thread.h"

#ifdef _DEBUG
#define STERN_MODE
#endif

const int INVALID_LINE = -1;

Lock::Lock(WCHAR *name)
{
	lockID_ = LockManager::getInstance().generalID();
	Name_ = name;

	lockingFILE_.clear();
	lockingLINE_ = INVALID_LINE;
}

Lock::~Lock()
{
	Name_.clear();
}

const WCHAR* Lock::name()
{
	return Name_.c_str();
}

size_t Lock::lockID()
{
	return lockID_;
}

lock_t& Lock::mutex()
{
	return mutex_;
}

void Lock::lock(LPCWSTR fileName, int lineNo)
{
	mutex_.lock();

	lockingFILE_ = fileName;
	lockingLINE_ = lineNo;
}

void Lock::setThreadID(size_t id)
{
	threadID_ = id;
}

size_t Lock::threadID()
{
	return threadID_;
}

//Search Deadlock

LockSafeScope::LockSafeScope(Lock* lock, LPCWSTR fileName, int lineNo)
{
	if (lock == nullptr)
	{
		return;
	}
	if (_shutdown == true)
	{
		return;
	}

	lock_ = lock;
	Lock *deadLock = LockManager::getInstance().checkDaeadLock(lock_);
	if (deadLock != nullptr)
	{
#ifdef STERN_MODE
		SErrLog(L"<%s>lock , <%s>lock is dead", deadLock->name(), lock->name());
#else
		std::lock(lock_ -> mutex(), deadLock->mutex());
#endif
		return;
	}

	lock_->lock*(fileName, lineNo);
	lock->setThreadID(GET_CURRENT_THREAD_ID());
}

LockSafeScope::~LockSafeScope()
{
	if (!lock_)
	{
		return;
	}
	if (_shutdown == true)
	{
		return;
	}
	lock_->unlock();
	lock_->setThreadID(0);
}

LockManager::LockManager()
{
	idSeed_ = 0;
}

Lock* LockManager::searchLockCycle(Lock* newLock)
{
	Thread *thread = ThreadManager::getInstance().at(GET_CURRENT_THREAD_ID());
	if (!thread)
	{
		return nullptr;
	}
	std::vector<Lock*>trace;

	trace.push_back(newLock);

	Lock *deadLock = nullptr;
	while (true)
	{
		Lock threadLock = thread->lock();
		if (threadLock == nullptr)
		{
			break;
		}
		if (threadLock->lockID() == trace[0]->lockID())
		{
			deadLock = threadLock;
			break;
		}
		trace.push_back(threadLock);
		thread = ThreadManager::getInstance().at(threadLock->threadId());
		if (!thread)
		{
			break;
		}
		trace.empty();
		return deadLock;
	}
}

Lock* LockManager::checkDaeadLock(Lock *newLock)
{
	Lock *deadLock = this->searchLockCycle(newLock);
	if (deadLock)
	{
		return deadLock;
	}

	return nullptr;
}

size_t LockManager::generalID()
{
	size_t id = idSeed_++;
	return id;
}