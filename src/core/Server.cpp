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

#include "peakengine/core/Server.hpp"
#include "peakengine/support/Thread.hpp"
#include "peakengine/network/NetworkData.hpp"
#include "peakengine/network/Connection.hpp"

namespace peak
{
	Server::Server(Engine *engine) : EntityManager(engine), thread(0),
		localconnection(0)
	{
	}
	Server::~Server()
	{
		if (thread)
			delete thread;
	}

	bool Server::init(BufferPointer serverdata, unsigned int port)
	{
		// Create network host
		// TODO
		// Load server data
		if (!load(serverdata))
			return false;
		// Create server thread
		stopping = false;
		thread = new Thread();
		thread->create(new ClassFunctor<Server>(this, &Server::runThread));
		return true;
	}
	bool Server::shutdown()
	{
		// Exit thread
		stopping = true;
		thread->wait();
	}

	void Server::addClient(Connection *connection)
	{
		localconnection = connection;
	}

	unsigned int Server::getTime()
	{
	}

	void Server::runThread()
	{
		while (!stopping)
		{
			// Incoming local connections
			if (localconnection != 0)
			{
				BufferPointer serverdata = onNewConnection(localconnection);
				if (serverdata)
				{
					clients.push_back((Connection*)localconnection);
					BufferPointer msg = new Buffer();
					msg->write8(EPT_InitialData);
					*msg.get() += *serverdata.get();
					localconnection->send(msg, true);
					onConnectionAccepted(localconnection);
					localconnection = 0;
				}
				else
				{
					localconnection = 0;
				}
			}
			// Incoming network connections
			// TODO
			// Receive data
			// TODO
			// Update entities
			// TODO
			// Send updates
			// TODO
		}
	}
}
