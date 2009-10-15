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
#include "peakengine/support/OS.hpp"

#include <iostream>

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
		time = 0;
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

	void Server::addEntity(Entity *entity)
	{
		// TODO: Owner?
		EntityManager::addEntity(entity);
		// Send the entity to all clients
		BufferPointer buffer = new Buffer();
		buffer->write8(EPT_EntityCreated);
		buffer->write16(entity->getID());
		buffer->writeString(entity->getType());
		entity->getState(buffer.get());
		for (unsigned int i = 0; i < clients.size(); i++)
			clients[i]->send(buffer, true);
	}
	void Server::removeEntity(Entity *entity)
	{
		EntityManager::removeEntity(entity);
		// Remove the entity from all clients
		BufferPointer buffer = new Buffer();
		buffer->write8(EPT_EntityDeleted);
		buffer->write16(entity->getID());
		for (unsigned int i = 0; i < clients.size(); i++)
			clients[i]->send(buffer, true);
	}

	void Server::addClient(Connection *connection)
	{
		localconnection = connection;
	}

	unsigned int Server::getTime()
	{
		return time;
	}

	void Server::runThread()
	{
		uint64_t lastframe = OS::getSystemTime();
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
			time++;
			update();
			// Send updates
			// TODO
			// 20 ms per frame
			lastframe = lastframe + 20000;
			uint64_t currenttime = OS::getSystemTime();
			if (currenttime < lastframe)
			{
				float percentage = (float)(lastframe - currenttime) / 200;
				//std::cout << "Server: " << lastframe - currenttime << std::endl;
				std::cout << std::fixed;
				std::streamsize prec = std::cout.precision(1);
				std::cout << "Server: " << percentage << "%" << std::endl;
				std::cout.precision(prec);
				OS::sleep(lastframe - currenttime);
			}
		}
	}
}
