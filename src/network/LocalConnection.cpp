/*
Copyright (c) 2009, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "peakengine/network/LocalConnection.hpp"
#include "peakengine/support/ScopedLock.hpp"

namespace peak
{
	LocalConnection::LocalConnection() : other(0), mutex(0)
	{
	}
	LocalConnection::~LocalConnection()
	{
		close();
		if (mutex)
			delete mutex;
	}

	void LocalConnection::create(LocalConnection &a, LocalConnection &b)
	{
		Mutex *mutex = new Mutex();
		a.mutex = mutex;
		b.mutex = mutex;
		a.other = &b;
		b.other = &a;
	}

	bool LocalConnection::isConnected()
	{
		if (!mutex)
			return false;
		ScopedLock lock(*mutex);
		return other != 0;
	}
	void LocalConnection::close()
	{
		if (!mutex)
			return;
		ScopedLock lock(*mutex);
		if (other == 0)
			return;
		other->other = 0;
		other = 0;
		mutex = 0;
	}

	void LocalConnection::send(BufferPointer buffer, bool reliable)
	{
		if (!mutex)
			return;
		mutex->lock();
		if (other)
			other->received.push(new Buffer(*buffer.get()));
		mutex->unlock();
	}
	bool LocalConnection::hasData()
	{
		if (!mutex)
			return false;
		ScopedLock lock(*mutex);
		return received.size() > 0;
	}
	BufferPointer LocalConnection::receive()
	{
		if (!mutex)
			return 0;
		ScopedLock lock(*mutex);
		if (received.size() == 0)
			return 0;
		BufferPointer data = received.front();
		received.pop();
		return data;
	}
}