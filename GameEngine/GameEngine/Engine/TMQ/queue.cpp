#include "queue.hpp"
#include "dispatcher.hpp"

using namespace TMQ;

PtrQueue::PtrQueue(std::size_t chunkSize)
: _chunkSize(chunkSize)
, _data(nullptr)
, _cursor(0)
, _size(0)
, _to(0)
{
}


PtrQueue& PtrQueue::operator=(PtrQueue &&o)
{
	std::swap(_data, o._data);
	std::swap(_chunkSize, o._chunkSize);
	std::swap(_cursor, o._cursor);
	std::swap(_to, o._to);
	std::swap(_size, o._size);
//	o.clear();
	return *this;
}

PtrQueue::PtrQueue(PtrQueue &&o)
{
	std::swap(_data, o._data);
	std::swap(_chunkSize, o._chunkSize);
	std::swap(_cursor, o._cursor);
	std::swap(_to, o._to);
	std::swap(_size, o._size);
//	o.clear();
}

PtrQueue::~PtrQueue()
{
	if (_data != nullptr)
		free(_data);
}

void PtrQueue::pop()
{
	if (empty())
		return;

	char *tmp = _data;
	std::size_t soi = sizeof(std::size_t);

	tmp += _cursor;
	std::size_t s = *reinterpret_cast<std::size_t*>(tmp);
	_cursor += s + soi;
	tmp += soi;
	auto r = reinterpret_cast<MessageBase*>(tmp);
	r->~MessageBase();
}

MessageBase *PtrQueue::front()
{
	if (empty())
		return nullptr;

	char *tmp = _data;
	std::size_t soi = sizeof(std::size_t);

	tmp += _cursor;
	std::size_t s = *reinterpret_cast<std::size_t*>(tmp);
	tmp += soi;
	return ((MessageBase*)(tmp));
}

std::size_t PtrQueue::getFrontSize()
{
	if (empty())
		return 0;

	char *tmp = _data;
	std::size_t soi = sizeof(std::size_t);

	tmp += _cursor;
	std::size_t s = *reinterpret_cast<std::size_t*>(tmp);
	return s;
}


void PtrQueue::clear()
{
	_cursor = _to = 0;
}

void PtrQueue::eraseAll()
{
	while (!empty())
	{
		pop();
	}
	clear();
}

void PtrQueue::release()
{
	clear();
	if (_data != nullptr)
		free(_data);
	_data = nullptr;
}

bool PtrQueue::empty()
{
	if (_cursor >= _to)
	{
		_cursor = 0;
		_to = 0;
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////


ReleasableQueue::ReleasableQueue()
	: _millisecondToWait(1)
{
	_releasable = true;
}

void ReleasableQueue::launch()
{
	_writeCondition.notify_one();
}

//return true if ti's a priority queue
bool ReleasableQueue::getReadableQueue(TMQ::PtrQueue &q)
{
	std::unique_lock<std::mutex> lock(_mutex);
	if (!_readCondition.wait_for(lock, std::chrono::milliseconds(1), [this](){ return !_copy.empty(); }))
	{
		return false;
	}
	q = std::move(_copy);
	_copy.clear();
	lock.unlock();
	_writeCondition.notify_one();
	_releasable = true;
	return true;
}


void ReleasableQueue::clear()
{
	std::unique_lock<std::mutex> lock(_mutex);
	_queue.eraseAll();
}

bool ReleasableQueue::releaseReadability(ReleasableQueue::WaitType waitType)
{
	if (waitType == WaitType::NoWait)
	{
		if (!_releasable)
			return false;
		std::unique_lock<std::mutex> lock(_mutex);
		_copy = std::move(_queue);
		_queue.clear();
		_releasable = false;
		lock.unlock();
		_readCondition.notify_one();
		return true;
	}
	else if (waitType == WaitType::Block)
	{
		std::unique_lock<std::mutex> lock(_mutex);

		_writeCondition.wait(lock, [this]()
		{
			return (_copy.empty());
		});
		if (!_copy.empty())
			return false;
		_copy = std::move(_queue);
		_queue.clear();
		_releasable = false;
		lock.unlock();
		_readCondition.notify_one();
		return true;
	}
	else if (waitType == WaitType::Wait)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		if (!_writeCondition.wait_for(lock, std::chrono::microseconds(1), [this]()
		{
			return (_copy.empty());
		}))
		{
			return false;
		}
		_copy = std::move(_queue);
		_queue.clear();
		_releasable = false;
		lock.unlock();
		_readCondition.notify_one();
		return true;
	}
	return true;
}

void ReleasableQueue::setWaitingTime(std::size_t milliseconds)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_millisecondToWait = milliseconds;
}

std::size_t ReleasableQueue::getWaitingTime()
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _millisecondToWait;
}
